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

  inline uint32_t GetWidth() const override { return m_data.Width; };
  inline uint32_t GetHeight() const override { return m_data.Height; };

  inline void SetEventCallback(const EventCallbackFn& callback) override {
    m_data.EventCallback = callback;
  }

  virtual void SetVSync(bool enabled) override;
  virtual bool IsVSync() const override;

  inline void* GetNativeWindow() const override { return m_window; }; 
  inline GraphicsContext* GetGraphicsContext() override { return m_context; };
private:
  virtual void Init(const WindowProps& props);
  virtual void Shutdown();

private:
  GLFWwindow* m_window;
  GraphicsContext* m_context;

  struct WindowData
  {
    std::string Title;
    uint32_t Width, Height;
    uint32_t FrameWidth, FrameHeight;
    bool VSync;

    EventCallbackFn EventCallback;
  };

  WindowData m_data;
};


}

#endif // !COCOAWINDOW_H
