#include "Buffer.h"
#include "Renderer.h"

#include "Platform/SDK/OpenGL/OpenGLBuffer.h"


namespace EastWind {


  VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
  {
    switch (Renderer::GetAPI())
    {
      case Renderer::API::None :
        return nullptr;
      case Renderer::API::OpenGL :
        return new OpenGLVertexBuffer(vertices, size);
      case Renderer::API::Vulkan :
        return nullptr;
      case Renderer::API::DX12 :
        return nullptr;
      case Renderer::API::Metal :
        return nullptr;
    }

    return nullptr;
  }

  IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
  {
    switch (Renderer::GetAPI())
    {
      case Renderer::API::None :
        return nullptr;
      case Renderer::API::OpenGL :
        return new OpenGLIndexBuffer(indices, count);
      case Renderer::API::Vulkan :
        return nullptr;
      case Renderer::API::DX12 :
        return nullptr;
      case Renderer::API::Metal :
        return nullptr;
    }

    return nullptr;
  }

  Ref<FrameBuffer> FrameBuffer::Create(float width, float height)
  {
    switch (Renderer::GetAPI())
    {
      case Renderer::API::None :
        return nullptr;
      case Renderer::API::OpenGL :
        return std::make_shared<OpenGLFrameBuffer>(width, height);
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

