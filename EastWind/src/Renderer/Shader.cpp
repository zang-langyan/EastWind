#include "EW_pch.h"
#include "EW_Core.h"
#include "EW_Log.h"

#include "Renderer.h"
#include "Shader.h"
#include "Platform/SDK/OpenGL/OpenGLShader.h"

namespace EastWind {

  /* Shader */
  Ref<Shader> Shader::Create(const std::string& path)
  {
    switch (Renderer::GetAPI())
    {
      case Renderer::API::None :
        return nullptr;
      case Renderer::API::OpenGL :
        return std::make_shared<OpenGLShader>(path);
      case Renderer::API::Vulkan :
        return nullptr;
      case Renderer::API::DX12 :
        return nullptr;
      case Renderer::API::Metal :
        return nullptr;
    }

    return nullptr;
  }

  Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
  {
    switch (Renderer::GetAPI())
    {
      case Renderer::API::None :
        return nullptr;
      case Renderer::API::OpenGL :
        return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
      case Renderer::API::Vulkan :
        return nullptr;
      case Renderer::API::DX12 :
        return nullptr;
      case Renderer::API::Metal :
        return nullptr;
    }

    return nullptr;
  }


  /* ShaderLibrary */
  void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
  {
    m_Shaders[name] = shader;
  }


  void ShaderLibrary::Add(const Ref<Shader>& shader)
  {
    m_Shaders[shader->GetName()] = shader;
  }

  Ref<Shader> ShaderLibrary::Load(const std::string& path)
  {
    auto shader = Shader::Create(path);
    Add(shader);
    return shader;
  }

  Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& path)
  {
    auto shader = Shader::Create(path);
    Add(name, shader);
    return shader;
  }

  Ref<Shader> ShaderLibrary::Get(const std::string& name)
  {
    EW_ASSERT(Exist(name), "Shader "+name+" Not Exist!");
    return m_Shaders[name];
  }


  bool ShaderLibrary::Exist(const std::string& name) const
  {
    return m_Shaders.find(name) != m_Shaders.end();
  }
}
