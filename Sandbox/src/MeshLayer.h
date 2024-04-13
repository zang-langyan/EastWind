#include <EastWind.h>
#include <EastWind_Math.h>
#include <EastWind_Graphics.h>
#include <cmath>


class MeshLayer: public EastWind::Layer
{
public:
  MeshLayer()
    : Layer("Mesh"),
      m_Camera(1280.f/720.f)    
  {
    /**************************************/
    // Mesh Creation   
    /**************************************/
    // Buffer State Manager
    m_MeshVA = EastWind::BufferState::Create();

    {
      // Vertex Buffer Creation
      EastWind::Ref<EastWind::VertexBuffer> vertexBuffer;
      
      const int Nx = 50, Ny = 50;
      float vertices[3*Nx*Ny];
      float x0 = -2.f, y0 = -2.f;
      float xn = 2.f, yn = 2.f;
      for (int i = 0; i < Nx; ++i){
        float x = x0 + (xn-x0)*i/Nx;
        for (int j = 0; j < Ny; ++j){
          float y = y0 + (yn-y0)*j/Ny;
          int idx = 3*(j+i*Nx);
          vertices[idx] = x; 
          vertices[idx+1] = y; 
          // vertices[idx+2] = 2.f * x * std::exp(-x*x - y*y); 
          // vertices[idx+2] = 2.f * x * std::sin(x) * std::cos(y); 
          // vertices[idx+2] = 2.f * std::sin(x*y); 
          vertices[idx+2] = std::sqrt(8.f-x*x-y*y); 
        }
      }

      vertexBuffer.reset(EastWind::VertexBuffer::Create(vertices, sizeof(vertices)));
      EastWind::BufferLayout layout = {
        { EastWind::ShaderDataType::Float3, "aPos" },
      };
      vertexBuffer->SetLayout(layout);
      m_MeshVA->AddVertexBuffer(vertexBuffer);
    

      // Index Buffer Creation
      EastWind::Ref<EastWind::IndexBuffer> indexBuffer;
      uint32_t indices[2*3*(Nx-1)*(Ny-1)];
      for (int i = 0; i < Nx-1; ++i){
        for (int j = 0; j < Ny-1; ++j){
          int i0 = j+i*Nx;
          int i1 = j+1+i*Nx;
          int i2 = j+(i+1)*Nx;
          int i3 = j+1+(i+1)*Nx;

          int idx = 2*3*(j+i*Nx);
          indices[idx] = i0; 
          indices[idx+1] = i2; 
          indices[idx+2] = i3; 

          indices[idx+3] = i3; 
          indices[idx+4] = i1; 
          indices[idx+5] = i0; 
        }
      }
      indexBuffer.reset(EastWind::IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
      m_MeshVA->SetIndexBuffer(indexBuffer);
    }

    {
      std::string vertexSrc = R"(
        #version 330 core

        layout (location = 0) in vec3 aPos;

        uniform mat4 u_VPMatrix;

        out vec3 v_Position;
        out vec4 v_Color;

        void main()
        {
            //v_Position = aPos;
            v_Color = vec4(0.f,1.f,0.f,1.f);
            gl_Position = u_VPMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
      )";

      std::string fragmentSrc = R"(
        #version 330 core

        out vec4 FragColor;

        in vec3 v_Position;
        in vec4 v_Color;

        void main()
        {
            // FragColor = vec4(v_Position*0.5f+0.5f, 1.0f);
            FragColor = v_Color;
        }
      )"; 
      m_MeshShader = EastWind::Shader::Create("MeshShader", vertexSrc, fragmentSrc);
    }
    /**************************************/
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

    EastWind::Renderer::ClearColor({0.f, 0.f, 0.f, 1.f});
    EastWind::Renderer::ClearBufferAndDepth();

    // m_Camera.SetPosition(EastWind::Vec<float,4>({0.f,0.f,0.f,1.f}));
    // m_Camera.RotateY(EastWind::PI/1800.f);

    // --------- Scene Begin ---------
    EastWind::Renderer::BeginScene(m_Camera.GetCamera());

    EastWind::Renderer::Submit(m_MeshShader, m_MeshVA);

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

  // Current Mouse Position
  float m_MouseX, m_MouseY;

  // Model Matrix
  EastWind::Mat<float,4,4> m_ModelMatrix;
};

