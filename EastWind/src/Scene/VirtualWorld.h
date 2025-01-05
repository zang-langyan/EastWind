#ifndef VIRTUALWORLD_H
#define VIRTUALWORLD_H

#include "Scene/Scene.h"

namespace EastWind {

class VWorld
{
public:
  VWorld();
  ~VWorld();

private:
  std::vector<Scene> m_scenes;
};

}

#endif // !VIRTUALWORLD_H
