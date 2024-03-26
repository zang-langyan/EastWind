#include "EW_pch.h"
#include "BufferState.h"

#include "Renderer.h"
#include "Platform/SDK/OpenGL/OpenGLVertexArray.h"

namespace EastWind {

  BufferState* BufferState::Create()
  {
    switch (Renderer::GetAPI())
    {
      case Renderer::API::None :
        return nullptr;
      case Renderer::API::OpenGL :
        return new OpenGLVertexArray();
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
