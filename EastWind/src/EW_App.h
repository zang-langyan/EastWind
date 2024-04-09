#ifndef EW_APP_H
#define EW_APP_H

#include "EW_Core.h"
#include "EW_Window.h"

#include "EW_Timestep.h"
#include "EW_LayerStack.h"

#include "Event/Event.h"
#include "Event/ApplicationEvent.h"

// #include "Renderer/Buffer.h"
// #include "Renderer/Shader.h"
// #include "Renderer/BufferState.h"
// #include "Renderer/Camera.h"

namespace EastWind {

class App
{
public:
  App();
  virtual ~App();

  void run();

  void OnEvent(Event& e);

  void PushLayer(Layer* layer);
  void PushOverlay(Layer* layer);

  inline Window& GetWindow() { return *m_window; }

  inline static App& Get() { return *s_instance; }
private:
  bool OnWindowClose(WindowCloseEvent& e);

  std::unique_ptr<Window> m_window;
  bool m_running = true;
  LayerStack m_layerStack;

  float m_lastFrameTime = 0.0f;

  static App* s_instance; 
};


App* CreateApp();

} // namespace EastWind


#endif // !EW_APP_H
