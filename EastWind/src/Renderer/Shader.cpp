#include "EW_pch.h"
#include "EW_Log.h"

#include "Renderer.h"
#include "Shader.h"
#include "Platform/SDK/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

namespace EastWind {

  Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
  {
    switch (Renderer::GetAPI())
    {
      case Renderer::API::None :
        return nullptr;
      case Renderer::API::OpenGL :
        return new OpenGLShader(vertexSrc, fragmentSrc);
      case Renderer::API::Vulkan :
        return nullptr;
      case Renderer::API::DX12 :
        return nullptr;
      case Renderer::API::Metal :
        return nullptr;
    }

    return nullptr;
  }
  
}
