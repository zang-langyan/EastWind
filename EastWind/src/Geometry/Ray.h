#ifndef RAY_H
#define RAY_H

#include <EastWind_Math.h>
#include "Geometry/Mesh.h"

namespace EastWind {

class Ray
{
public:
  Ray() = default;
  Ray(const Ray& ray) : m_pos(ray.GetStartPoint()), m_dir(ray.GetDirection()) {};
  Ray(const Vec3& position, const Vec3& direction) : m_pos(position), m_dir(direction) {};

  Vec3 GetStartPoint() const { return m_pos; }
  Vec3 GetDirection() const { return m_dir; }

  static bool Hit(const Ray& ray, const Mesh& mesh);
private:
  Vec3 m_pos;
  Vec3 m_dir;
};

}


#endif // !RAY_H
