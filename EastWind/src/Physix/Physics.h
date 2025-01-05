#ifndef PHYSICS_H
#define PHYSICS_H

#include "EW_Core.h"
#include "Geometry/Ray.h"

namespace EastWind {

class Physics
{
public:
  virtual ~Physics(){};

  virtual void Initialize() const = 0;

public:
  enum class PhyBackEnd
  {
    None = 0,
    Bullet = 1,
    PhyX = 2
  };
  inline static PhyBackEnd GetPhysicsBackEnd(){ return s_phybackend; }

  static Ref<Physics> Create();

private:
  static PhyBackEnd s_phybackend;

protected:
  void Update_CursorRay(std::pair<uint32_t,uint32_t> window, Mat4 ViewMat, Mat4 ProjMat);
  Ray m_cursor_ray;
};

}

#endif // !PHYSICS_H
