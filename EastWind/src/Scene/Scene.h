#ifndef SCENE_H
#define SCENE_H

#include "Geometry/Mesh.h"

namespace EastWind {

class Mesh;

class Scene
{
public:
  void AddMeshes(const Mesh& mesh) { m_meshes.emplace_back(mesh); }
  std::vector<Mesh>& GetMeshes() { return m_meshes; }

  static bool AssImp_Read(const std::string& path, Scene& scene);

private:
  std::vector<Mesh> m_meshes;
};


class SceneManager {
public:
    static SceneManager& instance() {
        static SceneManager ins;
        return ins;
    }

    void AddScene(const Scene& scene) { m_scenes.emplace_back(scene); }
    std::vector<Scene>& GetScenes() { return m_scenes; }
private:
    std::vector<Scene> m_scenes;
};

}

#endif // !SCENE_H
