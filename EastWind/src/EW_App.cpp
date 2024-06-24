#include "EW_pch.h"
#include "EW_App.h"

#include "Event/ApplicationEvent.h"
#include "EW_Log.h"
#include "Event/Event.h"

#include "Renderer/Renderer.h"
// #include "Renderer/Buffer.h"
// #include "Renderer/Shader.h"

#include <GLFW/glfw3.h>
// #include <OpenGL/OpenGL.h>
#include <glad/glad.h>

namespace EastWind {
  App* App::s_instance = nullptr; 


  App::App() 
  {
#ifdef EW_PLATFORM_OSX
    EW_INFO("EastWind Engine Built On Mac OS X");
#endif // EW_PLATFORM_OSX
#ifdef EW_PLATFORM_WINDOWS
    EW_INFO("EastWind Engine Built On Windows");
#endif // EW_PLATFORM_WINDOWS


    EW_ASSERT(!s_instance, "Application already exist!");
    s_instance = this;

    m_window = std::unique_ptr<Window>(Window::Create());
    m_window->SetEventCallback(BIND_EVENT_FN(App::OnEvent));  

    Renderer::Initialize();
  }

  App::~App()
  {

  }

  void App::PushLayer(Layer* layer)
  {
    m_layerStack.PushLayer(layer);
    layer->OnAttach();
  }

  void App::PushOverlay(Layer* layer)
  {
    m_layerStack.PushOverlay(layer);
    layer->OnAttach();
  }

  /* Event Callback Function */
  /* 
   * When GLFW Receives a Signal
   * It dispatch an Event and Bind an Event function (calling it atferwards)*/
  void App::OnEvent(Event& e)
  {
    EventDispatcher dispatcher(e);

    if (dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(App::OnWindowClose)))
    {
      EW_CORE_FATAL(e);
      return;
    }
    if (dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(App::OnWindowResize)))
    {
      // EW_CORE_TRACE(e);
    }
        
    for (auto itr = m_layerStack.end(); itr != m_layerStack.begin(); ){
      (*--itr)->OnEvent(e);
      if (e.Handled){
        // EW_CORE_TRACE(e);
        break;
      }
    }
  }

  bool App::OnWindowClose(WindowCloseEvent& e)
  {
    m_running = false;
    return true;
  }

  bool App::OnWindowResize(WindowResizeEvent& e)
  {
    if (0 == e.GetWidth() || 0 == e.GetHeight()){
      m_minimized = true;
      return false;
    }

    m_minimized = false;
    Renderer::SetViewport(0, 0, e.GetFrameWidth(), e.GetFrameHeight());
    return false;
  }

  void App::run()
  {
    EW_FATAL("App running!");
    while (m_running)
    {
      float time = glfwGetTime();
      Timestep timestep = time - m_lastFrameTime;
      m_lastFrameTime = time;

      if (!m_minimized) {
        for (Layer* layer: m_layerStack){
          layer->OnUpdate(timestep);
        }
      }
      
      m_window->OnUpdate();
    }
  }


  
} // EastWind
