#include "EW_pch.h"
#include "EW_Window.h"

#include "Platform/OS/MacOS/CocoaWindow.h"


namespace EastWind{
  
  Window* Window::Create(const WindowProps& props)
  {
    return new CocoaWindow(props);
  }

}
