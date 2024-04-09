#include "Texture.h"

#include "Renderer.h"
#include "Platform/SDK/OpenGL/OpenGLTexture.h"

namespace EastWind{

  Ref<Texture2D> Texture2D::Create(const std::string& path){
    switch (Renderer::GetAPI())
    {
      case Renderer::API::None :
        return nullptr;
      case Renderer::API::OpenGL :
        return std::make_shared<OpenGLTexture2D>(path);
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
