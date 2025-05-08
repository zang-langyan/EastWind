#include <EastWind.h>

#include "ConfigManager.h"
#include "SandBoxApp.hpp"

EastWind::App* EastWind::CreateApp()
{
  // return new App();
  return new Sandbox();
}

int main(int argc, char** argv)
{
  EW_INFO("Welcome to EastWind!");

  // Read Configs
  ConfigManager::global_instance().read_yaml_config("resource_cfg", "./config/resource.yaml");

  EastWind::App* app = EastWind::CreateApp();
  EW_WARN("App Initializing!");

  app->run();
  EW_FATAL("App Terminating!");
  delete app;
}

