#ifndef EW_WINDOW_H
#define EW_WINDOW_H


#include "EW_Core.h"
#include "Event/Event.h"
#include "Renderer/GraphicsContext.h"

namespace EastWind {

struct WindowProps
{
  std::string Title;
  uint32_t Width;
  uint32_t Height;

  WindowProps(
      const std::string& title = "EastWind Engine",
      uint32_t width = 1280,
      uint32_t height = 720
  ): Title(title), Width(width), Height(height){} 
};


class Window
{
public:
  using EventCallbackFn = std::function<void(Event&)>;

  virtual ~Window(){}

  virtual void OnUpdate() = 0;


  virtual uint32_t GetWidth() const = 0;
  virtual uint32_t GetHeight() const = 0;


  virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
  virtual void SetVSync(bool enabled) = 0;
  virtual bool IsVSync() const = 0;


  virtual void* GetNativeWindow() const = 0; 
  virtual GraphicsContext* GetGraphicsContext() = 0;
  static Window* Create(const WindowProps& props = WindowProps());
};

}

#endif // !EW_WINDOW_H
