#include "EW_pch.h"
#include "EW_Core.h"
#include "EW_Log.h"

#include "OpenGLShader.h"

#include <cstring>
#include <fstream>
#include <glad/glad.h>

namespace EastWind {

  OpenGLShader::OpenGLShader(const std::string& path)
  {
    std::string source = ReadFile(path);
    auto shaderSrc = PreProcess(source);
    Compile(shaderSrc, m_rendererId);

    // Get the Name of the Shader
    auto lastSlash = path.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = path.rfind('.');
    auto nameLen = lastDot == std::string::npos ? path.length() - lastSlash : lastDot - lastSlash;
    m_name = path.substr(lastSlash, nameLen);
  }

  OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    : m_name(name)
  {
    std::unordered_map<OpenGLShaderType, std::string> shaderSrc;
    shaderSrc[OpenGLShaderType::Vertex] = vertexSrc;
    shaderSrc[OpenGLShaderType::Fragment] = fragmentSrc;
    Compile(shaderSrc, m_rendererId);
  }

  OpenGLShader::~OpenGLShader()
  {
    glDeleteProgram(m_rendererId);
  }


  std::string OpenGLShader::ReadFile(const std::string& path)
  {
    std::string source;
    std::ifstream in(path, std::ios::binary);
    if (in){
      in.seekg(0, std::ios::end);
      source.resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&source[0], source.size());
      in.close();
    }else{
      EW_ERROR("Cannot Open File "+path);
    }
    return source;
  }


  OpenGLShader::OpenGLShaderType OpenGLShader::OpenGLShaderTypeMapping(const std::string& type)
  {
    if ("vertex" == type) {
      return OpenGLShaderType::Vertex;
    } else if ("fragment" == type) {
      return OpenGLShaderType::Fragment;
    } else if ("pixel" == type) {
      return OpenGLShaderType::Pixel;
    }
    return OpenGLShaderType::None;
  }

  std::string OpenGLShader::OpenGLShaderTypeMapping(const OpenGLShaderType& type)
  {
    switch(type){
      case OpenGLShaderType::Vertex: return "VERTEX SHADER";
      case OpenGLShaderType::Fragment: return "FRAGMENT SHADER";
      default: return "NOT SUPPORTED SHADER";
    }
  }

  std::unordered_map<OpenGLShader::OpenGLShaderType, std::string> OpenGLShader::PreProcess(const std::string& source)
  {
    std::unordered_map<OpenGLShaderType, std::string> shaderSrc;

    const char* typeToken = "#type";
    size_t typeTokenLen = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);
    while (pos != std::string::npos){
#ifdef EW_PLATFORM_WINDOWS
      size_t eol = source.find_first_of("\r\n", pos);
#else
      size_t eol = source.find_first_of("\n", pos);
#endif
      EW_ASSERT(eol != std::string::npos, "Syntax Error! Must Declare The Type!");
      size_t begin = pos + typeTokenLen + 1;
      std::string type = source.substr(begin, eol-begin);
      EW_ASSERT(OpenGLShaderType::None != OpenGLShaderTypeMapping(type), "Shader Type Not Supported!"); 

#ifdef EW_PLATFORM_WINDOWS
      size_t nextLinePos = source.find_first_not_of("\r\n", eol);
#else
      size_t nextLinePos = source.find_first_not_of("\n", eol);
#endif
      pos = source.find(typeToken, nextLinePos);
      shaderSrc[OpenGLShaderTypeMapping(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size()-1 : nextLinePos));
    }

    return shaderSrc;
  }

  void OpenGLShader::Compile(const std::unordered_map<OpenGLShaderType, std::string>& shaderSource, uint32_t& rendererId)
  {
    int  success;
    char infoLog[512];

    unsigned int shaderProgram = glCreateProgram();
    rendererId = shaderProgram;

    std::vector<GLuint> openglShaderIDs;
    openglShaderIDs.reserve(shaderSource.size());
    for (auto&& [type, src]: shaderSource){
      auto shader = glCreateShader(type);
      const char* src_cstr = src.c_str();
      glShaderSource(shader, 1, &src_cstr, NULL);
      glCompileShader(shader);

      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if(!success)
      {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);

        glDeleteShader(shader);
        for (auto id : openglShaderIDs){
          glDeleteShader(id);
        }

        EW_FATAL(infoLog);
        EW_ASSERT(false, "ERROR::SHADER::"+OpenGLShaderTypeMapping(type)+"::COMPILATION_FAILED");
        break;
      }

      glAttachShader(shaderProgram, shader);
      openglShaderIDs.push_back(shader);
    }

    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

      glDeleteProgram(shaderProgram);
      for (auto id : openglShaderIDs){
        glDeleteShader(id);
      }

      EW_FATAL(infoLog);
      EW_ASSERT(false, "ERROR::SHADER::PROGRAM::LINK_FAILED");
      return;
    }

    for (auto id : openglShaderIDs){
      glDeleteShader(id);
    }
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
