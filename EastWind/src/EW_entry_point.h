#ifndef EW_ENTRY_POINT_H
#define EW_ENTRY_POINT_H

#include "EW_App.h"
#include "EW_Core.h"
#include "EW_Log.h"
extern EastWind::App* EastWind::CreateApp();


int main(int argc, char** argv)
{
  EW_CORE_INFO("Welcome to EastWind!");

  EastWind::App* app = EastWind::CreateApp();
  EW_CORE_WARN("App Initializing!");

  app->run();
  EW_CORE_FATAL("App Terminating!");
  delete app;
}

#endif // !EW_ENTRY_POINT_H
