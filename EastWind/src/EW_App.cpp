#include "EW_pch.h"
#include "EW_App.h"

#include "Event/ApplicationEvent.h"
#include "EW_Log.h"
#include "Event/Event.h"

#include "Renderer/Renderer.h"
// #include "Renderer/Buffer.h"
// #include "Renderer/Shader.h"

#include "Core/ReloadManager.h"

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

    ShaderLibrary::instance().RegistReloadFunc();
    std::condition_variable stop_reload_thrd;
    std::mutex stop_reload_mtx;
    std::thread reload_thrd = std::thread(
      // [&stop_reload_thrd, &stop_reload_mtx](){
      [&](){
        m_window->GetGraphicsContext()->Init();
        EW_TRACE("Starting ReloadManager Thread...");
        while (true){
          // EW_INFO("ReloadManager Start...");
          if (ReloadManager::instance().reload() != 0) {
            EW_FATAL("ReloadManager reload Failed");
            exit(-1);
          }
          // m_window->GetGraphicsContext()->SwapBuffers();
          // EW_ERROR("ReloadManager Finished");
          // std::this_thread::sleep_until(
          //   std::chrono::steady_clock::now() + std::chrono::milliseconds(1000) // sleep for one second
          // );
          std::unique_lock<std::mutex> lck(stop_reload_mtx);
          if (stop_reload_thrd.wait_for(lck, std::chrono::milliseconds(1000)) == std::cv_status::no_timeout) {
            break;
          }
        }
      }
    );
    reload_thrd.detach();

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
    stop_reload_thrd.notify_all();
  }


  
} // EastWind
