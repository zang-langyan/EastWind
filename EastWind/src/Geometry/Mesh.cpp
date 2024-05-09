#include "EW_pch.h"
#include "Mesh.h"

#include "EW_Core.h"
#include "EW_Log.h"
#include "Platform/SDK/OpenGL/OpenGLShader.h"


namespace EastWind {

  Mesh::Mesh()
  {
    Ref<Shader> basic_shader = Shader::Create(BASIC_SHADER_GLSL);
    Ref<Shader> basic_texture_shader = Shader::Create(BASIC_TEXTURE_SHADER_GLSL);
    m_ShaderLib = std::make_shared<ShaderLibrary>();
    m_ShaderLib->Add("BasicShader", basic_shader);
    m_ShaderLib->Add("BasicTextureShader", basic_texture_shader);
  }

  Mesh::Mesh(const std::string& OFF_FilePath)
  {
    // EW_CORE_WARN("Initializing Mesh!!!");
    MeshData data;
    bool ok = Read_OFF_File(OFF_FilePath, data);

    if (!ok){
      EW_CORE_WARN("Failed to Read Mesh Data from " + OFF_FilePath);
      EW_ASSERT(ok, "");
    }
    m_MeshData = std::make_shared<MeshData>(data);
    // m_MeshData.reset(&data);

    m_BufferState = EastWind::BufferState::Create();

    // Vertex Buffer
    Ref<VertexBuffer> vertexBuffer;
    BufferLayout layout = {
      { EastWind::ShaderDataType::Float3, "aPos"    },
      { EastWind::ShaderDataType::Float3, "aNormal" },
    };
    const int n_vertices = m_MeshData->vertices.size();
    float vertices[n_vertices*6];
    for (int i = 0; i < n_vertices; ++i){
      vertices[i*6]   = m_MeshData->vertices[i]->position(0)*10;
      vertices[i*6+1] = m_MeshData->vertices[i]->position(1)*10;
      vertices[i*6+2] = m_MeshData->vertices[i]->position(2)*10;
      vertices[i*6+3] = m_MeshData->vertices[i]->normal(0);
      vertices[i*6+4] = m_MeshData->vertices[i]->normal(1);
      vertices[i*6+5] = m_MeshData->vertices[i]->normal(2);
    }
    vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    vertexBuffer->SetLayout(layout);
    m_BufferState->AddVertexBuffer(vertexBuffer);

    // Index Buffer
    Ref<IndexBuffer> indexBuffer;
    const int n_indices = m_MeshData->faces.size();
    uint32_t indices[n_indices*3];
    for (int i = 0; i < n_indices; ++i){
      indices[i*3]   = m_MeshData->faces[i]->indices(0);
      indices[i*3+1] = m_MeshData->faces[i]->indices(1);
      indices[i*3+2] = m_MeshData->faces[i]->indices(2);
    }
    indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
    m_BufferState->SetIndexBuffer(indexBuffer);

    {
      Ref<Shader> basic_shader = Shader::Create(BASIC_SHADER_GLSL);
      Ref<Shader> basic_texture_shader = Shader::Create(BASIC_TEXTURE_SHADER_GLSL);
      m_ShaderLib = std::make_shared<ShaderLibrary>();
      m_ShaderLib->Add("BasicShader", basic_shader);
      m_ShaderLib->Add("BasicTextureShader", basic_texture_shader);
    }
  }


  bool Mesh::Read_OFF_File(const std::string& OFF_File_Path, MeshData& data)
  {
    int n_vertex, n_face, n_edge;
    std::string file_type;
  
    std::ifstream off_file(OFF_File_Path);

    if (!off_file.is_open()) {
      EW_ERROR("Cannot Open OFF/NOFF File: " + OFF_File_Path);
      return false;
    }

    std::string line;
    while (std::getline(off_file, line)){
      trim(line);
      if (line[0] == '#' || line == "") continue;
      
      if (line[0] == 'O' || line[0] == 'N'){
        file_type = line;
        continue;
      }

      std::stringstream ss(line);
      ss >> n_vertex >> n_face >> n_edge;
      break;
    }


    if (file_type == "OFF") {
      ReadVertexList(data, n_vertex, off_file);
    } else if (file_type == "NOFF") {
      ReadVertexListNoff(data, n_vertex, off_file);
    }
    ReadFaceList(data, n_face, off_file);
    ReadEdgeList(data, n_edge);


    EW_CORE_TRACE("CLOSE OFF FILE");
    off_file.close();
    return true;
  }


  void Mesh::ReadVertexList(MeshData& data, const int& n_vertex, std::ifstream& fs)
  {
    std::string line;
    float x, y, z;
    for (int i = 0; i < n_vertex && std::getline(fs, line); ++i){
      std::stringstream ss(line);
      ss >> x >> y >> z;
      Vertex* v = new Vertex;
      v->vid = i;
      v->position = Vec3({x, -z, y});
      data.vertices.push_back(v);
    }
  }

  void Mesh::ReadVertexListNoff(MeshData& data, const int& n_vertex, std::ifstream& fs)
  {
    std::string line;
    float x, y, z, nx, ny, nz;
    for (int i = 0; i < n_vertex && std::getline(fs, line); ++i){
      std::stringstream ss(line);
      ss >> x >> y >> z >> nx >> ny >> nz;
      Vertex* v = new Vertex;
      v->vid = i;
      v->position = Vec3({x, -z, y});
      v->normal = Vec3({nx, ny, nz});
      data.vertices.push_back(v);
    }
  }

  void Mesh::ReadFaceList(MeshData& data, const int& n_face, std::ifstream& fs)
  {
    std::string line;
    int face_size, a, b, c;
    for (int i = 0; i < n_face && std::getline(fs, line); ++i){
      std::stringstream ss(line);
      ss >> face_size >> a >> b >> c;
      Face* fc = new Face;
      fc->fid = i;
      fc->indices = Vec<int,3>({a,b,c});
      data.faces.push_back(fc);
    }
  }

  void Mesh::ReadEdgeList(MeshData& data, const int& n_edge)
  {
  }

  
  void Mesh::Draw()
  {
    UploadModelMat();
    // Renderer::Submit(m_ShaderLib->Get(m_ActiveShader), m_BufferState, Renderer::PrimitiveType::Dot);
    // Renderer::Submit(m_ShaderLib->Get(m_ActiveShader), m_BufferState, Renderer::PrimitiveType::Line);
    Renderer::Submit(m_ShaderLib->Get(m_ActiveShader), m_BufferState, Renderer::PrimitiveType::Triangle);
  }

  void Mesh::SetModelMatrix(const Mat4& modelmat)
  {
    m_ModelMatrix = modelmat;
  }

  void Mesh::UploadModelMat()
  {
    m_ShaderLib->Get(m_ActiveShader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(m_ShaderLib->Get(m_ActiveShader))->UploadUniformMat4("u_ModelMatrix", m_ModelMatrix);
  }

  void Mesh::SetShaderLib(ShaderLibrary *lib)
  {
    m_ShaderLib.reset(lib);
  }
  
  void Mesh::SetActiveShader(const std::string& name)
  {
    m_ActiveShader = name;
  }

}
