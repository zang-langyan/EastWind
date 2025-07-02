#include "Geometry/Object.h"
#include <EastWind.h>
#include <EastWind_Math.h>
#include <EastWind_Graphics.h>
#include <cmath>

#include <yaml-cpp/yaml.h>

class MeshLayer: public EastWind::Layer
{
public:
  MeshLayer(float* r, float* g, float* b)
    : Layer("Mesh"),
      m_rabbit(RABBIT_OFF_FILE_PATH),
      m_sphere(1.f, 50, 50),
      r(r),g(g),b(b)
  {
    EW_WARN("Rabbit mesh path: " << RABBIT_OFF_FILE_PATH);
    auto scale_mat = EastWind::Mat4(.3f);
    scale_mat(3,3) = 1.f;
    auto trans_mat = EastWind::translate(EastWind::Vec3({0.f,0.f,-1.f}));
    auto rot_mat = EastWind::rotateX(-PI / 2.f) * EastWind::rotateY(-PI / 2.f) * EastWind::rotateZ(-PI / 6.f) ;
    auto model_mat = scale_mat * trans_mat * rot_mat;
    m_plane.SetModelMatrix(model_mat);
    EW_ERROR(model_mat);

    m_sphere.SetModelMatrix(trans_mat);

    // load shaders
    YAML::Node& resource_cfg = ConfigManager::global_instance().get_config_by_name("resource_cfg");
    for (int i = 0; i < resource_cfg["shaders"].size(); ++i) {
      const std::string& name = resource_cfg["shaders"][i]["name"].as<std::string>();
      const std::string& path = resource_cfg["shaders"][i]["path"].as<std::string>();
      EastWind::Ref<EastWind::Shader> shader = EastWind::Shader::Create(path);
      EastWind::ShaderLibrary::instance().Add(name, shader);
    }

    for (int i = 0; i < resource_cfg["meshes"].size(); ++i) {
      // const std::string& name = resource_cfg["meshes"][i]["name"].as<std::string>();
      const std::string& path = resource_cfg["meshes"][i]["path"].as<std::string>();
      EastWind::Scene scene;
      EastWind::Scene::AssImp_Read(path, scene);
      for (auto& mesh : scene.GetMeshes()) {
        mesh.SetModelMatrix(model_mat);
      }
      EastWind::SceneManager::instance().AddScene(scene);
    }
  }

  void OnUpdate(EastWind::Timestep ts) override
  {
    EastWind::CameraController::instance().OnUpdate(ts);
    // if (EastWind::Input::IsMouseButtonPressed(EW_MOUSE_BUTTON_MIDDLE)){
    //   auto [x, y] = EastWind::Input::GetMousePosition(); 
    //   auto z = EastWind::CameraController::instance().GetPosture().shoot;
    //   // std::cout << "x: " << x << ", y: " << y << std::endl;
    //   // std::cout << "z: " << z << std::endl;
    //   
    //   float dx = x - m_MouseX;
    //   float radian = EastWind::degree2radian(dx);
    //   EastWind::CameraController::instance().CamRotateZ(radian);
    // } else {
    //   auto [x, y] = EastWind::Input::GetMousePosition(); 
    //   m_MouseX = x;
    //   m_MouseY = y;
    // }
    //
    // {
    //   float camMovingSpeed = 1.f;
    //   if (EastWind::Input::IsKeyPressed(EW_KEY_W))
    //     EastWind::CameraController::instance().Translate(EastWind::Vec<float,3>({0,0,-ts*camMovingSpeed}));
    //   else if (EastWind::Input::IsKeyPressed(EW_KEY_S))
    //     EastWind::CameraController::instance().Translate(EastWind::Vec<float,3>({0,0,ts*camMovingSpeed}));
    //   else if (EastWind::Input::IsKeyPressed(EW_KEY_A))
    //     // EastWind::CameraController::instance().Translate(EastWind::Vec<float,3>({-ts*camMovingSpeed,0,0}));
    //     EastWind::CameraController::instance().CamRotateZ(ts*camMovingSpeed);
    //   else if (EastWind::Input::IsKeyPressed(EW_KEY_D))
    //     // EastWind::CameraController::instance().Translate(EastWind::Vec<float,3>({ts*camMovingSpeed,0,0}));
    //     EastWind::CameraController::instance().CamRotateZ(-ts*camMovingSpeed);
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

    // EastWind::CameraController::instance().SetPosition(EastWind::Vec<float,4>({0.f,0.f,0.f,1.f}));
    // EastWind::CameraController::instance().RotateY(EastWind::PI/1800.f);

    // --------- Scene Begin ---------
    EastWind::Renderer::BeginScene(EastWind::CameraController::instance().GetCamera());

    // EastWind::Renderer::Submit(m_MeshShader, m_MeshVA);

    // EastWind::Renderer::DepthTest(false);
    // EastWind::Renderer::CullFace(false);
    // m_skydome.SetModelMatrix(EastWind::rotateX(-PI/2.f));
    // m_skydome.Draw();
    EastWind::Renderer::DepthTest(true);
    EastWind::Renderer::CullFace(false);
    // if (EastWind::Input::GetCursorRay().Hit(m_rabbit)) {
    //   EW_ERROR("Hitting Rabbit Object");
    //   m_rabbit.SetActiveShader("BasicTextureShader");
    //   // m_rabbit.SetActiveShader("BasicShader");
    //   m_rabbit.Draw(EastWind::Renderer::PrimitiveType::Line);
    // } else {
    //   m_rabbit.SetActiveShader("BasicShader");
    //   m_rabbit.Draw();
    // }
    // m_sphere.SetActiveShader("BasicShader");
    // if (EastWind::Input::GetCursorRay().Hit(m_sphere)) {
    //   EW_ERROR("Hitting Sphere Object");
    //   m_sphere.SetActiveShader("BasicTextureShader");
    //   m_sphere.Draw(EastWind::Renderer::PrimitiveType::Line);
    // } else {
    //   m_sphere.SetActiveShader("BasicShader");
    //   m_sphere.Draw();
    // }
    // m_sphere.Draw();
    // m_cube.Draw();
    // m_plane.Draw();

    for (auto& scene : EastWind::SceneManager::instance().GetScenes()) {
      for (auto& mesh : scene.GetMeshes()) {
        if (EastWind::Input::GetCursorRay().Hit(m_rabbit)) {
          EW_ERROR("hit mesh Object");
          // mesh.SetActiveShader("BasicTextureShader");
          // m_rabbit.SetActiveShader("BasicShader");
          mesh.Draw(EastWind::Renderer::PrimitiveType::Line);
        } else {
          // mesh.SetActiveShader("BasicShader");
          mesh.Draw();
        }
        // mesh.Draw();
      }
    }

    EastWind::Renderer::EndScene();
    // --------- Scene End ---------
  }

  void OnEvent(EastWind::Event& e) override
  {
    EastWind::CameraController::instance().OnEvent(e);
  }

private:
  // Buffer Area
  EastWind::Ref<EastWind::BufferState> m_MeshVA;

  // Shader
  EastWind::Ref<EastWind::Shader> m_MeshShader;

  // Objects
  EastWind::Mesh m_rabbit;
  EastWind::Cube m_cube;
  EastWind::Sphere m_sphere;
  EastWind::Plane m_plane;

  // HDRi Light Background SkyDome
  EastWind::SkyDome m_skydome;

  // Current Mouse Position
  float m_MouseX, m_MouseY;

  // Model Matrix
  EastWind::Mat<float,4,4> m_ModelMatrix;

  // Background Color
  float *r, *g, *b;
};

