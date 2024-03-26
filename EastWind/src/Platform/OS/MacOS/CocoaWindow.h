#ifndef COCOAWINDOW_H
#define COCOAWINDOW_H

#include "EW_Window.h"
#include "Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace EastWind {

class CocoaWindow: public Window
{
public:
  CocoaWindow(const WindowProps& props);
  virtual ~CocoaWindow();

  virtual void OnUpdate() override;

  inline unsigned GetWidth() const override { return m_data.Width; };
  inline unsigned GetHeight() const override { return m_data.Width; };

  inline void SetEventCallback(const EventCallbackFn& callback) override {
    m_data.EventCallback = callback;
  }

  virtual void SetVSync(bool enabled) override;
  virtual bool IsVSync() const override;

  inline void* GetNativeWindow() const override { return m_window; }; 
private:
  virtual void Init(const WindowProps& props);
  virtual void Shutdown();

private:
  GLFWwindow* m_window;
  GraphicsContext* m_context;

  struct WindowData
  {
    std::string Title;
    unsigned Width, Height;
    bool VSync;

    EventCallbackFn EventCallback;
  };

  WindowData m_data;
};


}

#endif // !COCOAWINDOW_H
