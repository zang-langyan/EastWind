#include "EW_pch.h"

#include "OpenGLContext.h"
#include "EW_Log.h"
#include "EW_Core.h"

#include <glad/glad.h>


namespace EastWind {

  OpenGLContext::OpenGLContext(GLFWwindow* windowHandle): m_windowHandle(windowHandle)
  {
  }


  void OpenGLContext::Init()
  {
    glfwMakeContextCurrent(m_windowHandle);

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    EW_ASSERT(status, "Failed to initialize Glad!");

    EW_CORE_WARN("OpenGL Info:");
    EW_CORE_TRACE(glGetString(GL_VENDOR));
    EW_CORE_TRACE(glGetString(GL_RENDERER));
    EW_CORE_TRACE(glGetString(GL_VERSION));
  }

  void OpenGLContext::MakeCurrentContext() {
    glfwMakeContextCurrent(m_windowHandle);
  }

  void OpenGLContext::MakeNonCurrentContext() {
    glfwMakeContextCurrent(NULL);
  }

  void OpenGLContext::SwapBuffers()
  {
    glfwSwapBuffers(m_windowHandle);
  }
}
