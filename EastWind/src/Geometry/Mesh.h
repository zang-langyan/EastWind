#ifndef MESH_H
#define MESH_H

#include "EW_Core.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/BufferState.h"
#include "Renderer/Buffer.h"
#include "Renderer/Camera.h"
#include "Renderer/Texture.h"

namespace EastWind {

struct Vertex;
struct Edge;
struct Face;
struct HalfEdge;

typedef int Id;

struct Vertex {
  Id vid;
  Vec3 position;
  Vec3 normal;
  HalfEdge* halfedge;
};

struct Edge {
  Id eid;
  HalfEdge* halfedge;
};

struct Face {
  Id fid;
  Vec<int,3> indices;
  Vertex* va;
  Vertex* vb;
  Vertex* vc;
  Vec3 fnormal;
  HalfEdge* halfedge;

  bool contain(const Vec3& p) const {
    float s1 = (p - va->position) * (vb->position - va->position);
    float s2 = (p - vb->position) * (vc->position - vb->position);
    float s3 = (p - vc->position) * (va->position - vc->position);
    return (s1 > 0 && s2 > 0 && s3 > 0) || 
           (s1 < 0 && s2 < 0 && s3 < 0);
  };
};

struct HalfEdge{
  Id hid;
  HalfEdge* twin;
  HalfEdge* next;
  Vertex* half_va;
  Vertex* half_vb;
  Edge* edge;
  Face* face;
};

class Ray;

struct MeshData {
  std::vector<Vertex*> vertices;
  std::vector<Edge*> edges;
  std::vector<Face*> faces;
  Vec3 central;
};

class Mesh
{
public:
  Mesh();
  Mesh(const std::string& OFF_FilePath);

  Ref<MeshData> GetMeshData() { return m_MeshData; }

  virtual void Draw(Renderer::PrimitiveType primitive_type = Renderer::PrimitiveType::Triangle);
  void SetModelMatrix(const Mat4& modelmat);

  void SetActiveShader(const std::string& name);
  Ref<Shader> GetActiveShader() { return ShaderLibrary::instance().Get(m_ActiveShader); }

public:
  inline void AddVertex(Vec3 vert);
  inline void AddVertex(Vec3 vert, Vec3 normal);
  inline void AddFace(Vec<int,3> face);

  void BuildBuffer();

protected:
  friend Ray;
  Ref<BufferState> m_BufferState;
  Ref<MeshData> m_MeshData;
  
  std::string m_ActiveShader = "BasicShader";

  Mat4 m_ModelMatrix;
  void UploadModelMat();

private:
  static bool Read_OFF_File(const std::string& OFF_File_Path, MeshData& data);
  static void ReadVertexList(MeshData& data, const int& n_vertex, std::ifstream& fs);
  static void ReadVertexListNoff(MeshData& data, const int& n_vertex, std::ifstream& fs);
  static void ReadFaceList(MeshData& data, const int& n_face, std::ifstream& fs);
  static void ReadEdgeList(MeshData& data, const int& n_edge);
};
}

#endif // !MESH_H
