#include "Ray.h"

namespace EastWind {

  bool Ray::Hit(const Face& f, const Mat4& modelmat) const 
  {
    Mat4 mm_inv = modelmat.Inverse();
    Mat4 Q = mm_inv.Transpose();
    Vec3 transformed_normal = Vec3(Q * Vec4(f.fnormal, 0.f));
    transformed_normal.normalize();
    float denom = transformed_normal * m_dir;
    if (fabs(denom) < 1e-6) return false; // Ray is parallel to face
    float D = Vec3(modelmat * Vec4(f.fv_a->position, 1.f)) * transformed_normal;
    float t = (D - transformed_normal * m_pos) / denom;
    if (t < 0) return false; // Intersection is behind the ray origin
    Vec3 P = m_pos + t * m_dir;
    Vec3 P_local = Vec3(mm_inv * Vec4(P, 1.f));
    return f.contain(P_local);
  }

  bool Ray::Hit(const Face* f, const Mat4& modelmat) const
  {
    Mat4 mm_inv = modelmat.Inverse();
    Mat4 Q = mm_inv.Transpose();
    Vec3 transformed_normal = Vec3(Q * Vec4(f->fnormal, 0.f));
    transformed_normal.normalize();
    float denom = transformed_normal * m_dir;
    if (fabs(denom) < 1e-6) return false; // Ray is parallel to face
    float D = Vec3(modelmat * Vec4(f->fv_a->position, 1.f)) * transformed_normal;
    float t = (D - transformed_normal * m_pos) / denom;
    if (t < 0) return false; // Intersection is behind the ray origin
    Vec3 P = m_pos + t * m_dir;
    Vec3 P_local = Vec3(mm_inv * Vec4(P, 1.f));
    return f->contain(P_local);
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
