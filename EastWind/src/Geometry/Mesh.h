#ifndef MESH_H
#define MESH_H

#include "EW_Core.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/BufferState.h"
#include "Renderer/Buffer.h"
#include "Renderer/Camera.h"
#include "Renderer/Texture.h"
#include "Scene/Scene.h"

namespace EastWind {

struct Vertex;
struct Edge;
struct Face;
struct HalfEdge;

typedef int Id;

struct Vertex {
  Id vid;
  Vec3 position;
  Vec3 vnormal;
  Vec3 vtangent;
  Vec3 vbitangent;
  Vec4 vcolor;
  Vec3 vtexCoord;
  std::vector<HalfEdge*> vhes;
};

struct Edge {
  Id eid;
  Vertex* ev_a;
  Vertex* ev_b;
  HalfEdge* ehe_a;
  HalfEdge* ehe_b;
};

struct Face {
  Id fid;
  Vec<unsigned int,3> indices;
  Vertex* fv_a;
  Vertex* fv_b;
  Vertex* fv_c;
  Vec3 fnormal;
  HalfEdge* fhe_a;
  HalfEdge* fhe_b;
  HalfEdge* fhe_c;

  bool contain(const Vec3& p) const {
    if (!fv_a || !fv_b || !fv_c){
      EW_FATAL("Face does not contain Vertex*");
      return false;
    }
    float s1 = (p - fv_a->position) * (fv_b->position - fv_a->position);
    float s2 = (p - fv_b->position) * (fv_c->position - fv_b->position);
    float s3 = (p - fv_c->position) * (fv_a->position - fv_c->position);
    return (s1 > 0 && s2 > 0 && s3 > 0) || 
           (s1 < 0 && s2 < 0 && s3 < 0);
  };
};

struct HalfEdge{
  Id hid;
  HalfEdge* twin;
  HalfEdge* next;
  Vertex* hv_a;
  Vertex* hv_b;
  Edge* he;
  Face* hf;
};

class Ray;
class Scene;

struct MeshData {
  std::vector<Vertex*> vertices;
  std::vector<Face*> faces;
  std::vector<Edge*> edges;
  std::vector<HalfEdge*> halfedges;
  Vec3 central;
};

struct MeshBufferData {
  float* vertices = nullptr;
  uint32_t* indices = nullptr;
  ~MeshBufferData() {
    if (vertices) {
      delete[] vertices;
    }
    if (indices) {
      delete[] indices;
    }
  }
};

class Mesh
{
public:
  Mesh();
  Mesh(const std::string& OFF_FilePath);

  /* Mesh Building */
  Ref<MeshData> GetMeshData() { return m_MeshData; }
  void AddVertex(Vec3 vert);
  void AddVertex(Vec3 vert, Vec3 normal);
  void AddVertex(Vec3 vert, Vec3 normal, Vec3 tangent, Vec4 color);
  void AddFace(Vec<unsigned int,3> face);
  
  /* Renderer Functions */
  virtual void Draw(Renderer::PrimitiveType primitive_type = Renderer::PrimitiveType::Triangle);
  void SetModelMatrix(const Mat4& modelmat);
  void ApplyModelMatrix();
  void SetActiveShader(const std::string& name);
  Ref<Shader> GetActiveShader() { return ShaderLibrary::instance().Get(m_ActiveShader); }
  
  void SetVertexColor(std::size_t index, const Vec4& color);
  void SetColor(const Vec4& color);

  /* Buffer Building */
  virtual void PrepareBufferData();
  virtual void BuildBuffer();
  void SetBufferLayout(const BufferLayout& layout) { m_BufferLayout = layout; }
  void SetBufferLayout(const std::initializer_list<BufferElement>& elements) { m_BufferLayout = elements; }
  const BufferLayout& GetBufferLayout() { return m_BufferLayout; }

  /* Mesh Picking */
  void SetHit(bool hit) { m_ishit = hit; }
  bool IsHit() const { return m_ishit; }
protected:
  friend Ray;
  friend Scene;

protected:
  Ref<BufferState> m_BufferState;
  BufferLayout m_BufferLayout;

  Ref<MeshData> m_MeshData;
  Ref<MeshBufferData> m_MeshBufferData;

  std::string m_ActiveShader = "BasicShader";

  Mat4 m_ModelMatrix;
  void UploadModelMat();

private:
  bool m_ishit = false;
private:
  static bool Read_OFF_File(const std::string& OFF_File_Path, MeshData& data);
  static void ReadVertexList(MeshData& data, const int& n_vertex, std::ifstream& fs);
  static void ReadVertexListNoff(MeshData& data, const int& n_vertex, std::ifstream& fs);
  static void ReadFaceList(MeshData& data, const int& n_face, std::ifstream& fs);
  static void ReadEdgeList(MeshData& data, const int& n_edge);
};
}

#endif // !MESH_H
