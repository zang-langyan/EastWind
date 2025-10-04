#ifndef OBJECT_H
#define OBJECT_H

#include "Geometry/Mesh.h"

namespace EastWind {

class Object: public Mesh
{
public:
  Object(const std::string& OFF_FilePath): Mesh(OFF_FilePath) {} 
};

class Plane: public Mesh
{
public:
  Plane();
};

class Cube: public Mesh
{
public:
  Cube();
};

class Sphere: public Mesh
{
public:
  Sphere(float radius, int sectors, int stacks);

  float GetRadius() const { return r; }
  int GetSectors() const { return sectors; }
  int GetStacks() const { return stacks; }
private:
  float r;
  int sectors;
  int stacks;
};
}


#endif // !OBJECT_H
