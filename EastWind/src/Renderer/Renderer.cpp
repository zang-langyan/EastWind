#include "EW_pch.h"

#include "Renderer.h"
#include "Platform/SDK/OpenGL/OpenGLRenderer.h"
#include "Platform/SDK/OpenGL/OpenGLShader.h"

namespace EastWind {

  /*
   * Platform SDK Renderer
   */
  Renderer::API Renderer::s_RendererAPI = Renderer::API::OpenGL;

  Renderer* Renderer::Create(){
    switch (Renderer::GetAPI())
    {
      case Renderer::API::None :
        return nullptr;
      case Renderer::API::OpenGL :
        return new OpenGLRenderer();
      case Renderer::API::Vulkan :
        return nullptr;
      case Renderer::API::DX12 :
        return nullptr;
      case Renderer::API::Metal :
        return nullptr;
    }

    return nullptr;
  }

  Renderer* Renderer::s_Renderer = Renderer::Create();


  /****************************************************/
  // Interface
  /****************************************************/
  Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

  void Renderer::ClearColor(const float (&color)[4])
  {
    s_Renderer->SetClearColor(color);
  }

  void Renderer::ClearBufferAndDepth()
  {
    s_Renderer->Clear();
  }

  void Renderer::BeginScene(Camera& camera)
  {
    m_SceneData->VPMatrix = camera.GetViewProjectionMatrix();
  }
  
  void Renderer::EndScene()
  {
  }

  void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<BufferState>& bufferState)
  {
    shader->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_VPMatrix", m_SceneData->VPMatrix);
    // std::cout << "VPMAT:" << m_SceneData->VPMatrix << std::endl;

    bufferState->Bind();
    s_Renderer->Draw(bufferState);
  }


}