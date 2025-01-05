#include <EastWind.h>
#include <EastWind_Math.h>
#include <EastWind_Graphics.h>

#include <Platform/SDK/OpenGL/OpenGLShader.h>

#include "ImGui/ImGuiLayer.h"
#include "MeshLayer.h"
#include "TestLayer.h"

class Sandbox: public EastWind::App
{
public:
  Sandbox()
  {
    PushLayer(new TestLayer(&r,&g,&b));
    PushLayer(new MeshLayer(&r,&g,&b));
    PushLayer(new ImGuiLayer(&r,&g,&b));
  }

  ~Sandbox()
  {
  }

private:
  float r = 0.f,g = 0.f,b = 0.f;
};

