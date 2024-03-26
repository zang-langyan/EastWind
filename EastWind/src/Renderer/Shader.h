#ifndef SHADER_H
#define SHADER_H

#include <EastWind_Math.h>
#include <EastWind_Graphics.h>

namespace EastWind {

class Shader
{
public:
  virtual ~Shader() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
};

}

#endif // !SHADER_H
