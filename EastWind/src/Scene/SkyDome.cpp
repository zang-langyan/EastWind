#include "EW_pch.h"
#include "SkyDome.h"
#include "Platform/SDK/OpenGL/OpenGLShader.h"

namespace EastWind {

  SkyDome::SkyDome(const std::string& hdri_img_path)
    : Sphere(100.f, 48, 48)
  {
    m_texture = Texture2D::Create(hdri_img_path); 
    auto shader = GetActiveShader();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt("u_Texture", 0);
  }

  void SkyDome::SetHDRImg(const std::string& path)
  {
    m_texture = Texture2D::Create(path); 
    auto shader = GetActiveShader();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt("u_Texture", 0);
  }

  void SkyDome::Draw()
  {
    UploadModelMat();
    m_texture->Bind();
    Renderer::Submit(m_ShaderLib->Get(m_ActiveShader), m_BufferState, Renderer::PrimitiveType::Triangle);
  }
}
