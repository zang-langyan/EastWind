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

};

class Cube: public Mesh
{
public:
  Cube();
};

}


#endif // !OBJECT_H
