#ifndef SHADER_H
#define SHADER_H

#include "EW_Core.h"
#include <EastWind_Math.h>
#include <EastWind_Graphics.h>
#include "EW_Log.h"

#include "Core/ReloadManager.h"

namespace EastWind {

class Shader
{
public:
  virtual ~Shader() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual const std::string GetName() const = 0;

  static Ref<Shader> Create(const std::string& path);
  static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

  virtual int reload() = 0;
  virtual bool need_reload() { return false; }
};

class ShaderLibrary
{
public:
  static ShaderLibrary& instance() {
    static ShaderLibrary ins;
    if (ins.initialized()) {
      return ins;
    }
    Ref<Shader> basic_shader = Shader::Create(BASIC_SHADER_GLSL);
    Ref<Shader> basic_texture_shader = Shader::Create(BASIC_TEXTURE_SHADER_GLSL);
    ins.Add("BasicShader", basic_shader);
    ins.Add("BasicTextureShader", basic_texture_shader);
    EW_WARN("Building Shader Library: " << BASIC_SHADER_GLSL << ", " << BASIC_TEXTURE_SHADER_GLSL);
    ins.setInitialized();
    return ins;
  }

  ShaderLibrary(){};
  ShaderLibrary(const std::string& path){
    auto shader = Shader::Create(path);
    m_Shaders[shader->GetName()] = shader;
  }
  ShaderLibrary(const std::string& vertexSrc, const std::string& fragmentSrc){
    auto shader = Shader::Create("BasicShader", vertexSrc, fragmentSrc);
    m_Shaders[shader->GetName()] = shader;
  }
  ShaderLibrary(Ref<Shader>& shader){
    m_Shaders[shader->GetName()] = shader;
  }

  void Add(const std::string& name, const Ref<Shader>& shader);
  void Add(const Ref<Shader>& shader);

  Ref<Shader> Load(const std::string& path);
  Ref<Shader> Load(const std::string& name, const std::string& path);

  Ref<Shader> Get(const std::string& name);

  int reload() {
    for (auto&& [shader_name, shader_ptr] : m_Shaders) {
      if (!shader_ptr->need_reload()) {
        continue;
      }
      /* Lock Context */
      std::unique_lock<std::mutex> context_switch_lck(ReloadManager::instance().get_context_switch_mtx());
      ReloadManager::instance().get_context_switch_cv().wait(
        context_switch_lck, 
        [&]{return !ReloadManager::instance().get_main_thread_context_ready();}
      );
      ReloadManager::instance().set_side_thread_context_sleep(false);

      ReloadManager::instance().get_App()->GetWindow().GetGraphicsContext()->MakeCurrentContext();
      int ret = shader_ptr->reload();
      ReloadManager::instance().get_App()->GetWindow().GetGraphicsContext()->MakeNonCurrentContext();
      
      /* Unlock Context */
      ReloadManager::instance().set_main_thread_context_ready(true);
      ReloadManager::instance().set_side_thread_context_sleep(true);
      ReloadManager::instance().get_context_switch_cv().notify_one();
      
      if (ret != 0) {
        return ret;
      }
    }
    return 0;
  }

  void RegistReloadFunc() {
    ReloadManager::instance().regist("ShaderLibrary", [=]()->int{ return this->reload(); });
  }

  bool Exist(const std::string& name) const;

  bool initialized() const { return is_initialized; }
  bool setInitialized() { return is_initialized = true; }
private:
  std::unordered_map<std::string, Ref<Shader>> m_Shaders;
  bool is_initialized = false;
};

}

#endif // !SHADER_H
