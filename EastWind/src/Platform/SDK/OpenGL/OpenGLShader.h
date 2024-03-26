#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H

#include <EastWind_Math.h>
#include "Renderer/Shader.h"

namespace EastWind {

class OpenGLShader : public Shader
{
public:
  OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
  virtual ~OpenGLShader();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  void UploadUniformInt(const std::string& name, int value);

  void UploadUniformFloat(const std::string& name, float value);
  void UploadUniformFloat2(const std::string& name, const Vec<float,2>& value);
  void UploadUniformFloat3(const std::string& name, const Vec<float,3>& value);
  void UploadUniformFloat4(const std::string& name, const Vec<float,4>& value);

  void UploadUniformMat3(const std::string& name, const Mat<float,3,3>& mat);
  void UploadUniformMat4(const std::string& name, const Mat<float,4,4>& mat);

private:
  uint32_t m_rendererId;
};

}

#endif // !OPENGLSHADER_H
