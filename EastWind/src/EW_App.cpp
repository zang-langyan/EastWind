#include "EW_pch.h"
#include "EW_App.h"

#include "Event/ApplicationEvent.h"
#include "EW_Log.h"
#include "Event/Event.h"

// #include "Renderer/Renderer.h"
// #include "Renderer/Buffer.h"
// #include "Renderer/Shader.h"

#include <GLFW/glfw3.h>
// #include <OpenGL/OpenGL.h>
#include <glad/glad.h>

namespace EastWind {
  App* App::s_instance = nullptr; 

#define BIND_EVENT_FN(x) std::bind(&App::x, this, std::placeholders::_1)

  App::App() 
    
  {
    EW_ASSERT(!s_instance, "Application already exist!");
    s_instance = this;

    m_window = std::unique_ptr<Window>(Window::Create());
    m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));  
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

  void App::OnEvent(Event& e)
  {
    EventDispatcher dispatcher(e);

    if (dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose)))
    {
      EW_CORE_FATAL(e);
      return;
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

  void App::run()
  {
    EW_FATAL("App running!");
    while (m_running)
    {
      float time = glfwGetTime();
      Timestep timestep = time - m_lastFrameTime;
      m_lastFrameTime = time;
      for (Layer* layer: m_layerStack){
        layer->OnUpdate(timestep);
      }
      
      m_window->OnUpdate();
    }
  }


  
} // EastWind
