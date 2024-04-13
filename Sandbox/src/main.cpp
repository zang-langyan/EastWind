#include <EastWind.h>

#include "SandBoxApp.hpp"

EastWind::App* EastWind::CreateApp()
{
  // return new App();
  return new Sandbox();
}

int main(int argc, char** argv)
{
  EW_INFO("Welcome to EastWind!");

  EastWind::App* app = EastWind::CreateApp();
  EW_WARN("App Initializing!");

  app->run();
  EW_FATAL("App Terminating!");
  delete app;
}

