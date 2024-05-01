#ifndef SHADER_H
#define SHADER_H

#include "EW_Core.h"
#include <EastWind_Math.h>
#include <EastWind_Graphics.h>

namespace EastWind {

class Shader
{
public:
  virtual ~Shader() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual const std::string GetName() const = 0;

  static Ref<Shader> Create(const std::string& path);
  static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
};

class ShaderLibrary
{
public:
  ShaderLibrary(){};
  ShaderLibrary(const std::string& path){
    auto shader = Shader::Create(path);
    m_Shaders[shader->GetName()] = shader;
  }
  ShaderLibrary(const std::string& vertexSrc, const std::string& fragmentSrc){
    auto shader = Shader::Create("BasicShader", vertexSrc, fragmentSrc);
    m_Shaders[shader->GetName()] = shader;
  }
  ShaderLibrary(Ref<Shader>& shader){
    m_Shaders[shader->GetName()] = shader;
  }

  void Add(const std::string& name, const Ref<Shader>& shader);
  void Add(const Ref<Shader>& shader);

  Ref<Shader> Load(const std::string& path);
  Ref<Shader> Load(const std::string& name, const std::string& path);

  Ref<Shader> Get(const std::string& name);


  bool Exist(const std::string& name) const;

private:
  std::unordered_map<std::string, Ref<Shader>> m_Shaders;
};

}

#endif // !SHADER_H
