#include "EW_pch.h"
#include "Mesh.h"

#include "EW_Core.h"
#include "EW_Log.h"
#include "Platform/SDK/OpenGL/OpenGLShader.h"

namespace EastWind {

  static const BufferLayout DEFAULT_LAYOUT = {
    { EastWind::ShaderDataType::Float3, "vPos"    },
    { EastWind::ShaderDataType::Float3, "vNormal" },
    { EastWind::ShaderDataType::Float3, "vTangent" },
    { EastWind::ShaderDataType::Float4, "vColor" },
  };

  void Mesh::BuildBuffer()
  {
    // Vertex Buffer
    Ref<VertexBuffer> vertexBuffer;
    const int n_vertices = m_MeshData->vertices.size();
    constexpr int size_per_vertex = 3 + 3 + 3 + 4;
    float* vertices = new float[n_vertices * size_per_vertex];
    for (int i = 0; i < n_vertices; ++i){
      vertices[i*size_per_vertex]   = m_MeshData->vertices[i]->position(0);
      vertices[i*size_per_vertex+1] = m_MeshData->vertices[i]->position(1);
      vertices[i*size_per_vertex+2] = m_MeshData->vertices[i]->position(2);

      vertices[i*size_per_vertex+3] = m_MeshData->vertices[i]->vnormal(0);
      vertices[i*size_per_vertex+4] = m_MeshData->vertices[i]->vnormal(1);
      vertices[i*size_per_vertex+5] = m_MeshData->vertices[i]->vnormal(2);

      vertices[i*size_per_vertex+6] = m_MeshData->vertices[i]->vtangent(0);
      vertices[i*size_per_vertex+7] = m_MeshData->vertices[i]->vtangent(1);
      vertices[i*size_per_vertex+8] = m_MeshData->vertices[i]->vtangent(2);

      vertices[i*size_per_vertex+9]  = m_MeshData->vertices[i]->vcolor(0);
      vertices[i*size_per_vertex+10] = m_MeshData->vertices[i]->vcolor(1);
      vertices[i*size_per_vertex+11] = m_MeshData->vertices[i]->vcolor(2);
      vertices[i*size_per_vertex+12] = m_MeshData->vertices[i]->vcolor(3);
    }
    // vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(float) * n_vertices * size_per_vertex));
    // EW_CORE_ERROR("sizeof(vertices): " << sizeof(vertices));
    vertexBuffer->SetLayout(m_BufferLayout);
    m_BufferState->AddVertexBuffer(vertexBuffer);
    // delete[] vertices;

    // Index Buffer
    Ref<IndexBuffer> indexBuffer;
    const int n_indices = m_MeshData->faces.size();
    uint32_t* indices =  new uint32_t[n_indices*3];
    for (int i = 0; i < n_indices; ++i){
      indices[i*3]   = m_MeshData->faces[i]->indices(0);
      indices[i*3+1] = m_MeshData->faces[i]->indices(1);
      indices[i*3+2] = m_MeshData->faces[i]->indices(2);
    }
    // indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
    indexBuffer.reset(IndexBuffer::Create(indices, n_indices * 3));
    // EW_CORE_ERROR("sizeof(indices): " << sizeof(indices));
    m_BufferState->SetIndexBuffer(indexBuffer);
    // delete[] indices;
  }

  Mesh::Mesh()
  {
    MeshData data;
    m_MeshData = std::make_shared<MeshData>(data);
    
    m_BufferLayout = DEFAULT_LAYOUT;

    m_BufferState = EastWind::BufferState::Create();
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

    m_BufferLayout = DEFAULT_LAYOUT;

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
    v->vnormal = normal;
    m_MeshData->vertices.push_back(v);
  }

  inline void Mesh::AddFace(Vec<unsigned int,3> face)
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
      v->position = Vec3({x, y, z});
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
      v->position = Vec3({x, y, z})*10.f;
      v->vnormal = Vec3({nx, ny, nz});
      data.vertices.push_back(v);
    }
  }

  void Mesh::ReadFaceList(MeshData& data, const int& n_face, std::ifstream& fs)
  {
    std::string line;
    unsigned int face_size, a, b, c;
    for (int i = 0; i < n_face && std::getline(fs, line); ++i){
      std::stringstream ss(line);
      ss >> face_size >> a >> b >> c;
      Face* fc = new Face;
      fc->fid = i;
      fc->indices = Vec<unsigned int,3>({a,b,c});
      fc->fv_a = data.vertices[a];
      fc->fv_b = data.vertices[b];
      fc->fv_c = data.vertices[c];
      Vec3 ba = fc->fv_b->position - fc->fv_a->position, ca = fc->fv_c->position - fc->fv_a->position;
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
