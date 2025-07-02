#include "Ray.h"

namespace EastWind {

  bool Ray::Hit(const Face& f) const 
  {
    float D = f.fv_a->position * f.fnormal;
    float t = (D - f.fnormal * m_pos) / (f.fnormal * m_dir);
    Vec3 P = m_pos + t * m_dir;
    return f.contain(P);
  }

  bool Ray::Hit(const Face* f) const
  {
    float D = f->fv_a->position * f->fnormal;
    float t = (D - f->fnormal * m_pos) / (f->fnormal * m_dir);
    Vec3 P = m_pos + t * m_dir;
    return f->contain(P);
  }

  bool Ray::Hit(const Mesh& mesh) const
  {
    // TODO: Parallel Computing
    for (Face* f : mesh.m_MeshData->faces) {
      if (Hit(f)) {
        return true;
      }
    }
    return false;
  }

}
