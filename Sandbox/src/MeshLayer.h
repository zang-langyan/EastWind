#include "Geometry/Object.h"
#include <EastWind.h>
#include <EastWind_Math.h>
#include <EastWind_Graphics.h>
#include <cmath>


class MeshLayer: public EastWind::Layer
{
public:
  MeshLayer(float* r, float* g, float* b)
    : Layer("Mesh"),
      m_Camera(1280.f/720.f),
      m_rabbit(RABBIT_OFF_FILE_PATH),
      m_sphere(2.f, 3, 30),
      r(r),g(g),b(b)
  {
    
  }

  void OnUpdate(EastWind::Timestep ts) override
  {
    m_Camera.OnUpdate(ts);
    // if (EastWind::Input::IsMouseButtonPressed(EW_MOUSE_BUTTON_MIDDLE)){
    //   auto [x, y] = EastWind::Input::GetMousePosition(); 
    //   auto z = m_Camera.GetPosture().shoot;
    //   // std::cout << "x: " << x << ", y: " << y << std::endl;
    //   // std::cout << "z: " << z << std::endl;
    //   
    //   float dx = x - m_MouseX;
    //   float radian = EastWind::degree2radian(dx);
    //   m_Camera.CamRotateZ(radian);
    // } else {
    //   auto [x, y] = EastWind::Input::GetMousePosition(); 
    //   m_MouseX = x;
    //   m_MouseY = y;
    // }
    //
    // {
    //   float camMovingSpeed = 1.f;
    //   if (EastWind::Input::IsKeyPressed(EW_KEY_W))
    //     m_Camera.Translate(EastWind::Vec<float,3>({0,0,-ts*camMovingSpeed}));
    //   else if (EastWind::Input::IsKeyPressed(EW_KEY_S))
    //     m_Camera.Translate(EastWind::Vec<float,3>({0,0,ts*camMovingSpeed}));
    //   else if (EastWind::Input::IsKeyPressed(EW_KEY_A))
    //     // m_Camera.Translate(EastWind::Vec<float,3>({-ts*camMovingSpeed,0,0}));
    //     m_Camera.CamRotateZ(ts*camMovingSpeed);
    //   else if (EastWind::Input::IsKeyPressed(EW_KEY_D))
    //     // m_Camera.Translate(EastWind::Vec<float,3>({ts*camMovingSpeed,0,0}));
    //     m_Camera.CamRotateZ(-ts*camMovingSpeed);
    // }

    if (EastWind::Input::IsKeyPressed(EW_KEY_LEFT_SUPER) && EastWind::Input::IsKeyPressed(EW_KEY_EQUAL)){
      m_ModelMatrix(0,0) += ts;
      m_ModelMatrix(1,1) += ts;
    } else if (EastWind::Input::IsKeyPressed(EW_KEY_LEFT_SUPER) && EastWind::Input::IsKeyPressed(EW_KEY_MINUS)){
      m_ModelMatrix(0,0) = m_ModelMatrix(0,0)-ts >= 0 ? m_ModelMatrix(0,0)-ts : 0;
      m_ModelMatrix(1,1) = m_ModelMatrix(1,1)-ts >= 0 ? m_ModelMatrix(1,1)-ts : 0;
    }

    EastWind::Renderer::ClearColor({*r, *g, *b, 1.f});
    EastWind::Renderer::ClearBufferAndDepth();

    // m_Camera.SetPosition(EastWind::Vec<float,4>({0.f,0.f,0.f,1.f}));
    // m_Camera.RotateY(EastWind::PI/1800.f);

    // --------- Scene Begin ---------
    EastWind::Renderer::BeginScene(m_Camera.GetCamera());

    // EastWind::Renderer::Submit(m_MeshShader, m_MeshVA);

    EastWind::Renderer::DepthTest(false);
    EastWind::Renderer::CullFace(false);
    // m_skydome.SetModelMatrix(EastWind::rotateX(-PI/2.f));
    m_skydome.Draw();
    EastWind::Renderer::DepthTest(true);
    EastWind::Renderer::CullFace(true);
    m_rabbit.Draw();
    // m_sphere.SetActiveShader("BasicShader");
    // m_sphere.Draw();
    // m_cube.Draw();

    EastWind::Renderer::EndScene();
    // --------- Scene End ---------
  }

  void OnEvent(EastWind::Event& e) override
  {
    m_Camera.OnEvent(e);
  }

private:
  // Buffer Area
  EastWind::Ref<EastWind::BufferState> m_MeshVA;

  // Shader
  EastWind::Ref<EastWind::Shader> m_MeshShader;

  // Camera
  EastWind::CameraController m_Camera;

  // Objects
  EastWind::Mesh m_rabbit;
  // EastWind::Cube m_cube;
  EastWind::Sphere m_sphere;

  // HDRi Light Background SkyDome
  EastWind::SkyDome m_skydome;

  // Current Mouse Position
  float m_MouseX, m_MouseY;

  // Model Matrix
  EastWind::Mat<float,4,4> m_ModelMatrix;

  // Background Color
  float *r, *g, *b;
};

