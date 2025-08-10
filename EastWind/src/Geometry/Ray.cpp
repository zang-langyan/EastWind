#include "Ray.h"

namespace EastWind {

  bool Ray::Hit(const Face& f) const 
  {
    float denom = f.fnormal * m_dir;
    if (fabs(denom) < 1e-6) return false; // Ray is parallel to face
    float D = f.fv_a->position * f.fnormal;
    float t = (D - f.fnormal * m_pos) / denom;
    if (t < 0) return false; // Intersection is behind the ray origin
    Vec3 P = m_pos + t * m_dir;
    return f.contain(P);
  }

  bool Ray::Hit(const Face* f) const
  {
    float denom = f->fnormal * m_dir;
    if (fabs(denom) < 1e-6) return false; // Ray is parallel to face
    float D = f->fv_a->position * f->fnormal;
    float t = (D - f->fnormal * m_pos) / denom;
    if (t < 0) return false; // Intersection is behind the ray origin
    Vec3 P = m_pos + t * m_dir;
    return f->contain(P);
  }

  bool Ray::Hit(const Mesh& mesh) const
  {
    // Use early exit without atomic for better performance
    #pragma omp parallel for
    for (int i = 0; i < mesh.m_MeshData->faces.size(); ++i) {
      if (Hit(mesh.m_MeshData->faces[i])) {
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
      if (Hit(mesh->m_MeshData->faces[i])) {
        // Set hit and break out of the loop
        #pragma omp flush
        return true;
      }
    }
    return false;
  }

}
