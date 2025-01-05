#ifndef SCENE_H
#define SCENE_H

#include "Geometry/Mesh.h"

namespace EastWind {

class Scene
{
public:
  Scene();
  ~Scene();

  void AddStaticMeshes(const Mesh& mesh) { m_static_meshes.push_back(mesh); }
  std::vector<Mesh>& GetStaticMeshes() { return m_static_meshes; }

  void AddDynamicMeshes(const Mesh& mesh) { m_dynamic_meshes.push_back(mesh); }
  std::vector<Mesh>& GetDynamicMeshes() { return m_dynamic_meshes; }
private:
  static bool AssImp_Read(const std::string& path, Scene& scene);

private:
  std::vector<Mesh> m_static_meshes;
  std::vector<Mesh> m_dynamic_meshes;
};

}

#endif // !SCENE_H
