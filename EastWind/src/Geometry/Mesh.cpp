#include "EW_pch.h"
#include "Mesh.h"

#include "EW_Core.h"
#include "EW_Log.h"
#include "Platform/SDK/OpenGL/OpenGLShader.h"

namespace EastWind {

  void Mesh::BuildBuffer()
  {
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
      // vertices[i*6]   = m_MeshData->vertices[i]->position(0);
      // vertices[i*6+1] = m_MeshData->vertices[i]->position(1);
      // vertices[i*6+2] = m_MeshData->vertices[i]->position(2);

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
  }

  Mesh::Mesh()
  {
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

    {
      BuildBuffer();
    }
  }

  inline void Mesh::AddVertex(Vec3 vert)
  {
    Vertex* v = new Vertex;
    v->vid = m_MeshData->vertices.size();
    v->position = vert;
    m_MeshData->vertices.push_back(v);
  }

  inline void Mesh::AddVertex(Vec3 vert, Vec3 normal)
  {
    Vertex* v = new Vertex;
    v->vid = m_MeshData->vertices.size();
    v->position = vert;
    v->normal = normal;
    m_MeshData->vertices.push_back(v);
  }

  inline void Mesh::AddFace(Vec<int,3> face)
  {
      Face* fc = new Face;
      fc->fid = m_MeshData->faces.size();
      fc->indices = face;
      m_MeshData->faces.push_back(fc);
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
      fc->va = data.vertices[a];
      fc->vb = data.vertices[b];
      fc->vc = data.vertices[c];
      Vec3 ba = fc->vb->position - fc->va->position, ca = fc->vc->position - fc->va->position;
      fc->fnormal = ba.cross(ca);
      fc->fnormal.normalize();
      data.faces.push_back(fc);
    }
  }

  void Mesh::ReadEdgeList(MeshData& data, const int& n_edge)
  {
  }

  
  void Mesh::Draw(Renderer::PrimitiveType primitive_type)
  {
    UploadModelMat();
    Renderer::Submit(ShaderLibrary::instance().Get(m_ActiveShader), m_BufferState, primitive_type);
    // Renderer::Submit(ShaderLibrary::instance().Get(m_ActiveShader), m_BufferState, Renderer::PrimitiveType::Dot);
    // Renderer::Submit(ShaderLibrary::instance().Get(m_ActiveShader), m_BufferState, Renderer::PrimitiveType::Line);
    // Renderer::Submit(ShaderLibrary::instance().Get(m_ActiveShader), m_BufferState, Renderer::PrimitiveType::Triangle);
  }

  void Mesh::SetModelMatrix(const Mat4& modelmat)
  {
    m_ModelMatrix = modelmat;
  }

  void Mesh::UploadModelMat()
  {
    ShaderLibrary::instance().Get(m_ActiveShader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(ShaderLibrary::instance().Get(m_ActiveShader))->UploadUniformMat4("u_ModelMatrix", m_ModelMatrix);
  }
  
  void Mesh::SetActiveShader(const std::string& name)
  {
    m_ActiveShader = name;
  }

}
