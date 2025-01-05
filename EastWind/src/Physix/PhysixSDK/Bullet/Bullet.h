#ifndef BULLET_H
#define BULLET_H

#include "Physix/Physics.h"

#include <btBulletDynamicsCommon.h>

namespace EastWind {

class BulletPhyx: public Physics
{
public:
  BulletPhyx();
  virtual ~BulletPhyx(){};

  virtual void Initialize() const override;

};

}

#endif // !BULLET_H
