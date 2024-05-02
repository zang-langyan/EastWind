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
  Vec3 fnormal;
  HalfEdge* halfedge;
};

struct HalfEdge{
  Id hid;
  HalfEdge* twin;
  HalfEdge* next;
  Vertex* vertex;
  Edge* edge;
  Face* face;
};


class Mesh
{
public:
  Mesh();
  Mesh(const std::string& OFF_FilePath);

  virtual void Draw();
  void SetModelMatrix(const Mat4& modelmat);

  void SetShaderLib(ShaderLibrary *lib);
  void SetActiveShader(const std::string& name);
protected:
  struct MeshData {
    std::vector<Vertex*> vertices;
    std::vector<Edge*> edges;
    std::vector<Face*> faces;
    Vec3 central;
  };

  Ref<EastWind::BufferState> m_BufferState;
  Ref<MeshData> m_MeshData;
  
  Ref<ShaderLibrary> m_ShaderLib;
  std::string m_ActiveShader = "BasicShader";

  Mat4 m_ModelMatrix;

private:
  static bool Read_OFF_File(const std::string& OFF_File_Path, MeshData& data);
  static void ReadVertexList(MeshData& data, const int& n_vertex, std::ifstream& fs);
  static void ReadVertexListNoff(MeshData& data, const int& n_vertex, std::ifstream& fs);
  static void ReadFaceList(MeshData& data, const int& n_face, std::ifstream& fs);
  static void ReadEdgeList(MeshData& data, const int& n_edge);

  void UploadModelMat();
};
}

#endif // !MESH_H
