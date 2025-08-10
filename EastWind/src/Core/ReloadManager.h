#ifndef RELOAD_MANAGER_H
#define RELOAD_MANAGER_H

#include <string>
#include <unordered_map>
#include <functional>

#include "EW_Log.h"
#include "EW_App.h"
namespace EastWind {

class ReloadManager {
public:
  static ReloadManager& instance() {
    static ReloadManager ins;
    return ins;
  }

  void regist(std::string name, std::function<int()>&& reload_f) {
    std::unordered_map<std::string, std::function<int()>>::iterator itr =
      _reload_func.find(name);
    if (itr != _reload_func.end()) {
      EW_CORE_FATAL("Cache for " << name << "is already registered! SKIP CURRENT regist...");
      return;
    }
    _reload_func[name] = reload_f;
  }

  int reload() {
    for (const auto& [name, reload_f] : _reload_func) {
      int err_code = reload_f();
      if (err_code != 0) {
        EW_CORE_FATAL("Fail to Reload: " << name);
        return err_code;
      }
    }
    return 0;
  }

  void set_App(App* a) { app = a; }
  App* get_App() { return app; }

private:
  std::unordered_map<std::string, std::function<int()>> _reload_func;
  App* app;

public: /* Shader Hot Reload */ 
  std::condition_variable& get_context_switch_cv() { return context_switch_cv; }
  std::mutex& get_context_switch_mtx() { return context_switch_mtx; }

  void set_main_thread_context_ready(bool is_ready) { main_thread_context_ready = is_ready; }
  bool get_main_thread_context_ready() const { return main_thread_context_ready; }
  void set_side_thread_context_sleep(bool is_sleep) { side_thread_context_sleep = is_sleep; }
  bool get_side_thread_context_sleep() const { return side_thread_context_sleep; }

private: 
  std::condition_variable context_switch_cv;
  std::mutex context_switch_mtx;
  bool main_thread_context_ready;
  bool side_thread_context_sleep;
};

}

#endif