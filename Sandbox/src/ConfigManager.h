#ifndef SANDBOX_CONFIGMANAGER_H
#define SANDBOX_CONFIGMANAGER_H

#include <unordered_map>
#include <string>
#include <yaml-cpp/yaml.h>

class ConfigManager {
public:
    static ConfigManager& global_instance() {
        static ConfigManager cfg_manager;
        return cfg_manager;
    }

    bool read_yaml_config(const std::string& name, const std::string& cfg_path) {
        YAML::Node config = YAML::LoadFile(cfg_path);
        yaml_mp.emplace(name, config);
        return true;
    }


    YAML::Node& get_config_by_name(const std::string& name) {
        return yaml_mp[name];
    }
private:
    std::unordered_map<std::string, YAML::Node> yaml_mp;
};

#endif // SANDBOX_CONFIGMANAGER_H