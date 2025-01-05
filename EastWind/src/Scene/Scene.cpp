#include "EW_pch.h"
#include "EW_Log.h"

#include "Scene/Scene.h"

/* assimp include files. These three are usually needed. */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace EastWind {

  bool Scene::AssImp_Read(const std::string& path, Scene& data)
  {
    /* C Style */
    // const C_STRUCT aiScene* scene = aiImportFile(path.c_str(),aiProcessPreset_TargetRealtime_MaxQuality);
    // aiReleaseImport(scene);

    /* C++ Style */
    Assimp::Importer assimporter;
    const aiScene* scene = assimporter.ReadFile(
      path, 
      aiProcess_CalcTangentSpace       |
      aiProcess_Triangulate            |
      aiProcess_JoinIdenticalVertices  |
      aiProcess_SortByPType
    );

    if (nullptr == scene){
      EW_ERROR("Cannot Load Asset File: " + path);
      return false;
    }

    if (!scene->HasMeshes()) {
      EW_WARN("No Meshes in the Scene File: " + path);
      return true;
    }

    for (unsigned i = 0; i < scene->mNumMeshes; ++i){
      const aiMesh* mesh = scene->mMeshes[i];
      for (unsigned vi = 0; vi < mesh->mNumVertices; ++vi){
          mesh->mVertices[vi].x; 
      }
    }
    return true;
  }

}
