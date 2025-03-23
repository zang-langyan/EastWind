#ifndef RELOAD_MANAGER_H
#define RELOAD_MANAGER_H

#include <string>
#include <unordered_map>
#include <functional>

#include "EW_Log.h"
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

private:
  std::unordered_map<std::string, std::function<int()>> _reload_func;
};

}

#endif