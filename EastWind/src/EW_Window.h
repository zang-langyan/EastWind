#ifndef EW_WINDOW_H
#define EW_WINDOW_H


#include "EW_Core.h"
#include "Event/Event.h"


namespace EastWind {

struct WindowProps
{
  std::string Title;
  unsigned Width;
  unsigned Height;

  WindowProps(
      const std::string& title = "东风引擎",
      unsigned width = 1280,
      unsigned height = 720
  ): Title(title), Width(width), Height(height){} 
};


class Window
{
public:
  using EventCallbackFn = std::function<void(Event&)>;

  virtual ~Window(){}

  virtual void OnUpdate() = 0;


  virtual unsigned GetWidth() const = 0;
  virtual unsigned GetHeight() const = 0;


  virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
  virtual void SetVSync(bool enabled) = 0;
  virtual bool IsVSync() const = 0;


  virtual void* GetNativeWindow() const = 0; 

  static Window* Create(const WindowProps& props = WindowProps());
};

}

#endif // !EW_WINDOW_H
