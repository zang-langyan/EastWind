#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H

#include <EastWind_Math.h>
#include "Renderer/Shader.h"

#include <glad/glad.h>

namespace EastWind {

class OpenGLShader : public Shader
{
public:
enum OpenGLShaderType{
  None = 0,
  Vertex = GL_VERTEX_SHADER,
  Fragment = GL_FRAGMENT_SHADER,
  Pixel = GL_FRAGMENT_SHADER
};
static OpenGLShaderType OpenGLShaderTypeMapping(const std::string& type);
static std::string OpenGLShaderTypeMapping(const OpenGLShaderType& type);

public:
  OpenGLShader(const std::string& path);
  OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
  virtual ~OpenGLShader();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual const std::string GetName() const override { return m_name; };

public:
  void UploadUniformInt(const std::string& name, int value);

  void UploadUniformFloat(const std::string& name, float value);
  void UploadUniformFloat2(const std::string& name, const Vec<float,2>& value);
  void UploadUniformFloat3(const std::string& name, const Vec<float,3>& value);
  void UploadUniformFloat4(const std::string& name, const Vec<float,4>& value);

  void UploadUniformMat3(const std::string& name, const Mat<float,3,3>& mat);
  void UploadUniformMat4(const std::string& name, const Mat<float,4,4>& mat);

private:
  static std::string ReadFile(const std::string& path);
  static std::unordered_map<OpenGLShaderType, std::string> PreProcess(const std::string& source);
  static void Compile(const std::unordered_map<OpenGLShaderType, std::string>& shaderSource, uint32_t& rendererId);

private:
  uint32_t m_rendererId;
  std::string m_name;
};

}

#endif // !OPENGLSHADER_H
