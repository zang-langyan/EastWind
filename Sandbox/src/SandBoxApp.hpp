#include <EastWind.h>
#include <EastWind_Math.h>
#include <EastWind_Graphics.h>

#include <Platform/SDK/OpenGL/OpenGLShader.h>

#include "ClearLayer.h"
#include "ImGui/ImGuiLayer.h"
#include "MeshLayer.h"
#include "TestLayer.h"

class Sandbox: public EastWind::App
{
public:
  Sandbox()
  {
    PushLayer(new ClearLayer());
    // PushLayer(new TestLayer());
    PushLayer(new MeshLayer());
    PushLayer(new ImGuiLayer());
  }

  ~Sandbox()
  {
  }
};

