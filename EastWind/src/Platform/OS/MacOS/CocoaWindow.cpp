#include "EW_pch.h"
#include "EW_Core.h"
#include "EW_Log.h"
#include "EW_Window.h"

#include "Event/ApplicationEvent.h"
#include "Event/MouseEvent.h"
#include "Event/KeyEvent.h"

#include "CocoaWindow.h"


#include "Platform/SDK/OpenGL/OpenGLContext.h"

namespace EastWind {

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char* description)
{
  EW_CORE_ERROR("GLFW ERROR ("+std::to_string(error)+"): "+description);
}

// [[ LEGACY ]]
// Window* Window::Create(const WindowProps& props)
// {
//   return new CocoaWindow(props);
// }

CocoaWindow::CocoaWindow(const WindowProps& props)
{
  Init(props);
}

CocoaWindow::~CocoaWindow()
{
  Shutdown();
}

void CocoaWindow::Init(const WindowProps& props)
{
  m_data.Title = props.Title;
  m_data.Width = props.Width;
  m_data.Height = props.Height;

  EW_CORE_INFO("Creating Cocoa Window: "+m_data.Title+"   "+std::to_string(m_data.Width)+", "+std::to_string(m_data.Height));

  if (!s_GLFWInitialized){
    int success = glfwInit();
    EW_ASSERT(success, "Could not initialize GLFW!");

    // GLFW Error Callback Set
    glfwSetErrorCallback(GLFWErrorCallback);

    s_GLFWInitialized = true;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  

  m_window = glfwCreateWindow(
        (int)props.Width, 
        (int)props.Height,
        m_data.Title.c_str(),
        nullptr,
        nullptr
      );


  m_context = new OpenGLContext(m_window);
  m_context->Init();
  
  glfwSetWindowUserPointer(m_window, &m_data); // Bind WindowData to glfw window
  SetVSync(true);

  // Set GLFW callbacks
  glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

#ifdef EW_PLATFORM_OSX
    // MacOs window coordinates and pixel coordinates are not the same
    // window size glfwGetWindowSize
    // pixel size glfwGetFramebufferSize
    glfwGetFramebufferSize(window, &width, &height);
#endif
    data.Width = width;
    data.Height = height;

    WindowResizeEvent event(width, height);
    data.EventCallback(event);
  });

  glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window){
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    WindowCloseEvent event;
    data.EventCallback(event);
  });

  glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    switch (action)
    {
      case GLFW_PRESS:
      {
        KeyPressedEvent event(key, 0);
        data.EventCallback(event);
        break;
      }
      case GLFW_RELEASE:
      {
        KeyReleasedEvent event(key);
        data.EventCallback(event);
        break;
      }
      case GLFW_REPEAT:
      {
        KeyPressedEvent event(key, 1);
        data.EventCallback(event);
        break;
      }
    }

  });

  glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods){
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    switch (action)
    {
      case GLFW_PRESS:
      {
        MouseButtonPressedEvent event(button);
        data.EventCallback(event);
        break;
      }
      case GLFW_RELEASE:
      {
        MouseButtonReleasedEvent event(button);
        data.EventCallback(event);
        break;
      }
    }
  });

  glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset){
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    MouseScrolledEvent event((float)xOffset, (float)yOffset);
    data.EventCallback(event);
  });

  glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos){
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    MouseMovedEvent event((float)xPos, (float)yPos);
    data.EventCallback(event);
  });
}


void CocoaWindow::Shutdown()
{
  glfwDestroyWindow(m_window);
}

void CocoaWindow::OnUpdate()
{
  glfwPollEvents();
  m_context->SwapBuffers();
}

void CocoaWindow::SetVSync(bool enabled)
{
  if (enabled){
    glfwSwapInterval(1);
  }else{
    glfwSwapInterval(0);
  }

  m_data.VSync = enabled;
}

bool CocoaWindow::IsVSync() const
{
  return m_data.VSync;
}

}
