#ifndef RAY_H
#define RAY_H

#include <EastWind_Math.h>
#include "Geometry/Mesh.h"

namespace EastWind {

class Mesh;

class Ray
{
public:
  // Ray() = default;
  Ray(const Vec3& position, const Vec3& direction) : m_pos(position), m_dir(direction) {};

  Ray(const Ray& ray) : m_pos(ray.GetOrigin()), m_dir(ray.GetDirection()) {};
  Ray& operator=(const Ray& ray){
    this->m_pos=ray.GetOrigin();
    this->m_dir=ray.GetDirection();
    return *this;
  }

  Ray(const Ray&& ray) : m_pos(ray.GetOrigin()), m_dir(ray.GetDirection()) {};
  Ray& operator=(const Ray&& ray){
    this->m_pos=ray.GetOrigin();
    this->m_dir=ray.GetDirection();
    return *this;
  }

  Vec3 GetOrigin() const { return m_pos; }
  Vec3 GetDirection() const { return m_dir; }
  void SetOrigin(const Vec3& o) { m_pos = o; }
  void SetDirection(const Vec3& d) { m_dir = d; }

  bool Hit(const Mesh& mesh) const ;
  bool Hit(const Mesh* mesh) const ;
  bool Hit(const Face& face) const ;
  bool Hit(const Face* face) const ;
private:
  Vec3 m_pos;
  Vec3 m_dir;
};

}


#endif // !RAY_H
