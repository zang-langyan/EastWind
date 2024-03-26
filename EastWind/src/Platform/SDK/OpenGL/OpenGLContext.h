#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H

#include "Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace EastWind {

class OpenGLContext: public GraphicsContext 
{
public:
  OpenGLContext(GLFWwindow* windowHandle);
  virtual void Init() override;
  virtual void SwapBuffers() override;

private:
  GLFWwindow* m_windowHandle;
};
  
}


#endif // !OPENGLCONTEXT_H
