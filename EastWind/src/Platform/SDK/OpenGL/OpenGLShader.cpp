#include "EW_pch.h"
#include "EW_Log.h"

#include "OpenGLShader.h"

#include <glad/glad.h>

namespace EastWind {

  OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
  {
    /**************************************/
    int  success;
    char infoLog[512];


    const char* vertexShaderSource = vertexSrc.c_str();
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      EW_FATAL("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
      EW_FATAL(infoLog);
    }

    
    const char* fragmentShaderSource = fragmentSrc.c_str();
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      EW_FATAL("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");
      EW_FATAL(infoLog);
    }


    unsigned int shaderProgram;
    m_rendererId = glCreateProgram();
    shaderProgram = m_rendererId;
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      EW_FATAL("ERROR::SHADER::PROGRAM::LINK_FAILED");
      EW_FATAL(infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    /**************************************/
  }

  OpenGLShader::~OpenGLShader()
  {
    glDeleteProgram(m_rendererId);
  }

  void OpenGLShader::Bind() const 
  {
    glUseProgram(m_rendererId);
  }

  void OpenGLShader::Unbind() const
  {
    glUseProgram(0);
  }

  void OpenGLShader::UploadUniformInt(const std::string& name, int value)
  {
    unsigned int location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform1i(location, value);
  }


  void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
  {
    unsigned int location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform1f(location, value);
  }

  void OpenGLShader::UploadUniformFloat2(const std::string& name, const Vec<float,2>& value)
  {
    unsigned int location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform2f(location, value(0), value(1));
  }

  void OpenGLShader::UploadUniformFloat3(const std::string& name, const Vec<float,3>& value)
  {
    unsigned int location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform3f(location, value(0), value(1), value(2));
  }

  void OpenGLShader::UploadUniformFloat4(const std::string& name, const Vec<float,4>& value)
  {
    unsigned int location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniform4f(location, value(0), value(1), value(2), value(3));
  }


  void OpenGLShader::UploadUniformMat3(const std::string& name, const Mat<float,3,3>& mat)
  {
    unsigned int location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, mat.get_ptr());
  }

  void OpenGLShader::UploadUniformMat4(const std::string& name, const Mat<float,4,4>& mat)
  {
    unsigned int location = glGetUniformLocation(m_rendererId, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, mat.get_ptr());
  }


}
