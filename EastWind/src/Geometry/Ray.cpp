#include "Ray.h"

namespace EastWind {

  bool Ray::Hit(const Face& f, const Mat4& modelmat) const 
  {
    Mat4 Q = modelmat.Inverse().Transpose();
    float denom = Vec3(Q * Vec4(f.fnormal, 1.f)) * m_dir;
    if (fabs(denom) < 1e-6) return false; // Ray is parallel to face
    float D = Vec3(modelmat * Vec4(f.fv_a->position, 1.f)) * Vec3(Q * Vec4(f.fnormal, 1.f));
    float t = (D - Vec3(Q * Vec4(f.fnormal, 1.f)) * m_pos) / denom;
    if (t < 0) return false; // Intersection is behind the ray origin
    Vec3 P = m_pos + t * m_dir;
    return f.contain(P);
  }

  bool Ray::Hit(const Face* f, const Mat4& modelmat) const
  {
    Mat4 Q = modelmat.Inverse().Transpose();
    float denom = Vec3(Q * Vec4(f->fnormal, 1.f)) * m_dir;
    if (fabs(denom) < 1e-6) return false; // Ray is parallel to face
    float D = Vec3(modelmat * Vec4(f->fv_a->position, 1.f)) * Vec3(Q * Vec4(f->fnormal, 1.f));
    float t = (D - Vec3(Q * Vec4(f->fnormal, 1.f)) * m_pos) / denom;
    if (t < 0) return false; // Intersection is behind the ray origin
    Vec3 P = m_pos + t * m_dir;
    return f->contain(P);
  }

  bool Ray::Hit(const Mesh& mesh) const
  {
    // Use early exit without atomic for better performance
    #pragma omp parallel for
    for (int i = 0; i < mesh.m_MeshData->faces.size(); ++i) {
      if ( Hit(mesh.m_MeshData->faces[i], mesh.m_ModelMatrix) ) {
        // Set hit and break out of the loop
        #pragma omp flush
        return true;
      }
    }
    return false;
  }

  bool Ray::Hit(const Mesh* mesh) const
  {
    // Use early exit without atomic for better performance
    #pragma omp parallel for
    for (int i = 0; i < mesh->m_MeshData->faces.size(); ++i) {
      if ( Hit(mesh->m_MeshData->faces[i], mesh->m_ModelMatrix) ) {
        // Set hit and break out of the loop
        #pragma omp flush
        return true;
      }
    }
    return false;
  }

}
