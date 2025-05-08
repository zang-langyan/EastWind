#include "EW_pch.h"
#include "EW_Log.h"

#include "Scene/Scene.h"

/* assimp include files. These three are usually needed. */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace EastWind {

  bool Scene::AssImp_Read(const std::string& path, Scene& scene)
  {
    /* C Style */
    // const C_STRUCT aiScene* ai_scene = aiImportFile(path.c_str(),aiProcessPreset_TargetRealtime_MaxQuality);
    // aiReleaseImport(ai_scene);

    /* C++ Style */
    Assimp::Importer assimporter;
    const aiScene* ai_scene = assimporter.ReadFile(
      path, 
      aiProcess_CalcTangentSpace       |
      aiProcess_Triangulate            |
      aiProcess_JoinIdenticalVertices  |
      aiProcess_SortByPType
    );

    if (nullptr == ai_scene){
      EW_ERROR("Cannot Load Asset File: " + path);
      return false;
    }

    if (!ai_scene->HasMeshes()) {
      EW_WARN("No Meshes in the Scene File: " + path);
      return true;
    }

    for (unsigned i = 0; i < ai_scene->mNumMeshes; ++i){
      Mesh mesh;
      const aiMesh* ai_mesh = ai_scene->mMeshes[i];
      for (unsigned vi = 0; vi < ai_mesh->mNumVertices; ++vi){
        Vertex* v = new Vertex;
        v->vid = vi;
        v->position = {
          ai_mesh->mVertices[vi].x,
          ai_mesh->mVertices[vi].y,
          ai_mesh->mVertices[vi].z
        };
        if (ai_mesh->mNormals) {
          v->vnormal = {
            ai_mesh->mNormals[vi].x,
            ai_mesh->mNormals[vi].y,
            ai_mesh->mNormals[vi].z
          };
        }
        if (ai_mesh->mTangents) { 
          /* According to assimp:
          * If the mesh contains tangents, it automatically also contains bitangents.
          */
          v->vtangent = {
            ai_mesh->mTangents[vi].x,
            ai_mesh->mTangents[vi].y,
            ai_mesh->mTangents[vi].z
          };
          v->vbitangent = {
            ai_mesh->mBitangents[vi].x,
            ai_mesh->mBitangents[vi].y,
            ai_mesh->mBitangents[vi].z
          };
        }
        if (ai_mesh->mColors) {
          for (int ci = 0; ci < AI_MAX_NUMBER_OF_COLOR_SETS; ++ci) {
            if (ai_mesh->mColors[ci]) { // color set
              v->vcolor = {
                ai_mesh->mColors[ci][vi].r,
                ai_mesh->mColors[ci][vi].g,
                ai_mesh->mColors[ci][vi].b,
                ai_mesh->mColors[ci][vi].a
              };
              break;
            }
          }
        }
        if (ai_mesh->mTextureCoords) {
          for (int tci = 0; tci < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++tci) {
            if (ai_mesh->mTextureCoords[tci]) { // color set
              v->vtexCoord = {
                ai_mesh->mTextureCoords[tci][vi].x,
                ai_mesh->mTextureCoords[tci][vi].y,
                ai_mesh->mTextureCoords[tci][vi].z
              };
              break;
            }
          }
        }

        mesh.m_MeshData->vertices.emplace_back(v);
      }

      for (unsigned fi = 0; fi < ai_mesh->mNumFaces; ++fi){
        Face* fc = new Face;
        /* According to assimp:
        * Together with the #aiProcess_Triangulate flag you can then be sure that
        * #aiFace::mNumIndices is always 3.
        * */
        fc->fid = fi;
        fc->indices = {
          ai_mesh->mFaces[fi].mIndices[0],
          ai_mesh->mFaces[fi].mIndices[1],
          ai_mesh->mFaces[fi].mIndices[2]
        };
        fc->fv_a = mesh.m_MeshData->vertices[fc->indices(0)];
        fc->fv_b = mesh.m_MeshData->vertices[fc->indices(1)];
        fc->fv_c = mesh.m_MeshData->vertices[fc->indices(2)];
        Vec3 ba = fc->fv_b->position - fc->fv_a->position, ca = fc->fv_c->position - fc->fv_a->position;
        fc->fnormal = ba.cross(ca);
        fc->fnormal.normalize();

        mesh.m_MeshData->faces.emplace_back(fc);
      }

      {
        mesh.BuildBuffer();
      }
      scene.AddMeshes(mesh);
    }
    return true;
  }

}
