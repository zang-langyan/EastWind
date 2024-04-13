#include "EW_pch.h"
#include "EW_Window.h"

#include "Platform/OS/MacOS/CocoaWindow.h"


namespace EastWind{
  
  Window* Window::Create(const WindowProps& props)
  {
#ifdef EW_PLATFORM_OSX
    return new CocoaWindow(props);
#endif
#ifdef EW_PLATFORM_WINDOWS
    // TODO: Windows Platform Window Creation
    return new CocoaWindow(props);
#endif // DEBUG
  }

}
