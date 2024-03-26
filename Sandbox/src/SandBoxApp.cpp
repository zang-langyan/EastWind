#include <EastWind.h>
#include <EastWind_Math.h>
#include <EastWind_Graphics.h>

#include "ImGui/ImGuiLayer.h"

class TestLayer: public EastWind::Layer
{
public:
  TestLayer(float* r, float* g, float* b)
    : Layer("Test"),
      m_Camera(
        EastWind::Vec<float,3>({0.f,0.f,1.f}), // position
        EastWind::Vec<float,3>({0.f,1.f,0.f}), // up
        EastWind::Vec<float,3>({0.f,0.f,1.f}), // direction
        EastWind::Vec<float,6>({-1.f,1.5f,1.f,-1.f,1.f,200.f}) // boundary l,r,t,b,n,f
      ),
      r(r),g(g),b(b)
  {
    /**************************************/
    // Triangle   
    /**************************************/
    // Buffer State Manager
    m_TriangleVA.reset(EastWind::BufferState::Create());

    // Vertex Buffer Creation
    {
      std::shared_ptr<EastWind::VertexBuffer> vertexBuffer;
      float vertices[3*7] = {
        -0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.2f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.2f, 0.8f, 0.1f, 1.0f,
        0.0f,  0.5f, 0.0f, 0.1f, 0.2f, 0.8f, 1.0f
      };
      vertexBuffer.reset(EastWind::VertexBuffer::Create(vertices, sizeof(vertices)));
      EastWind::BufferLayout layout = {
        { EastWind::ShaderDataType::Float3, "aPos"   },
        { EastWind::ShaderDataType::Float4, "aColor" }
      };
      vertexBuffer->SetLayout(layout);
      m_TriangleVA->AddVertexBuffer(vertexBuffer);
    }

    // Index Buffer Creation
    {
      std::shared_ptr<EastWind::IndexBuffer> indexBuffer;
      uint32_t indices[3] = {0,1,2};
      indexBuffer.reset(EastWind::IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
      m_TriangleVA->SetIndexBuffer(indexBuffer);
    }

    {
      std::string vertexSrc = R"(
        #version 330 core

        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec4 aColor;

        uniform mat4 u_VPMatrix;

        out vec3 v_Position;
        out vec4 v_Color;

        void main()
        {
            v_Position = aPos;
            v_Color = aColor;
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
            FragColor = vec4(v_Position*0.5f+0.5f, 1.0f);
            FragColor = v_Color;
        }
      )"; 
      m_TriangleShader.reset(EastWind::Shader::Create(vertexSrc, fragmentSrc));
    }
    /**************************************/


    /**************************************/
    // Square   
    /**************************************/
    // Buffer State Manager
    m_SquareVA.reset(EastWind::BufferState::Create());

    // Vertex Buffer Creation
    {
      std::shared_ptr<EastWind::VertexBuffer> vertexBuffer;
      float vertices[3*4] = {
        -0.75f, -0.75f, 0.0f,
        0.75f, -0.75f, 0.0f,
        0.75f,  0.75f, 0.0f,
        -0.75f,  0.75f, 0.0f,
      };
      vertexBuffer.reset(EastWind::VertexBuffer::Create(vertices, sizeof(vertices)));
      EastWind::BufferLayout layout = {
        { EastWind::ShaderDataType::Float3, "aPos"   },
      };
      vertexBuffer->SetLayout(layout);
      m_SquareVA->AddVertexBuffer(vertexBuffer);
    }

    // Index Buffer Creation
    {
      std::shared_ptr<EastWind::IndexBuffer> indexBuffer;
      uint32_t indices[6] = {0,1,2,2,3,0};
      indexBuffer.reset(EastWind::IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
      m_SquareVA->SetIndexBuffer(indexBuffer);
    }

    {
      std::string vertexSrc = R"(
        #version 330 core

        layout (location = 0) in vec3 aPos;

        uniform mat4 u_VPMatrix;

        out vec3 v_Position;

        void main()
        {
            v_Position = aPos;
            gl_Position = u_VPMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
      )";

      std::string fragmentSrc = R"(
        #version 330 core

        out vec4 FragColor;

        in vec3 v_Position;

        void main()
        {
            FragColor = vec4(v_Position*0.5f+0.5f, 1.0f);
        }
      )"; 
      m_SquareShader.reset(EastWind::Shader::Create(vertexSrc, fragmentSrc));
    }
    /**************************************/

  }

  void OnUpdate(EastWind::Timestep ts) override
  {
    if (EastWind::Input::IsMouseButtonPressed(EW_MOUSE_BUTTON_MIDDLE)){
      auto [x, y] = EastWind::Input::GetMousePosition(); 
      auto z = m_Camera.GetPosture().shoot;
      // std::cout << "x: " << x << ", y: " << y << std::endl;
      // std::cout << "z: " << z << std::endl;
      
      float dx = x - m_MouseX;
      float radian = EastWind::degree2radian(dx);
      m_Camera.CamRotateY(radian);
    } else {
      auto [x, y] = EastWind::Input::GetMousePosition(); 
      m_MouseX = x;
      m_MouseY = y;
    }

    {
      float camMovingSpeed = 1.f;
      if (EastWind::Input::IsKeyPressed(EW_KEY_W))
        m_Camera.Translate(EastWind::Vec<float,3>({0,0,-ts*camMovingSpeed}));
      else if (EastWind::Input::IsKeyPressed(EW_KEY_S))
        m_Camera.Translate(EastWind::Vec<float,3>({0,0,ts*camMovingSpeed}));
      else if (EastWind::Input::IsKeyPressed(EW_KEY_A))
        m_Camera.Translate(EastWind::Vec<float,3>({-ts*camMovingSpeed,0,0}));
      else if (EastWind::Input::IsKeyPressed(EW_KEY_D))
        m_Camera.Translate(EastWind::Vec<float,3>({ts*camMovingSpeed,0,0}));
    }

    EastWind::Renderer::ClearColor({*r, *g, *b, 1.f});
    EastWind::Renderer::ClearBufferAndDepth();

    // m_Camera.SetPosition(EastWind::Vec<float,4>({0.f,0.f,0.f,1.f}));
    // m_Camera.RotateY(EastWind::PI/1800.f);

    // --------- Scene Begin ---------
    EastWind::Renderer::BeginScene(m_Camera);

    EastWind::Renderer::Submit(m_SquareShader, m_SquareVA);
    EastWind::Renderer::Submit(m_TriangleShader, m_TriangleVA);

    EastWind::Renderer::EndScene();
    // --------- Scene End ---------


    if (EastWind::Input::IsKeyPressed(EW_KEY_TAB))
      EW_TRACE("Polling: Tab Key Is Pressed!");
  }

  void OnEvent(EastWind::Event& e) override
  {
    if (e.GetEventType() == EastWind::EventType::KeyPressed){
      EastWind::KeyPressedEvent& event = (EastWind::KeyPressedEvent&)e;
      if (event.GetKeyCode() == EW_KEY_TAB){
        EW_CORE_WARN("Tab Key is Pressed");
      }  
      EW_INFO( (char)event.GetKeyCode() ); 
    }
  }

private:
  // Buffer Area
  std::shared_ptr<EastWind::BufferState> m_TriangleVA;
  std::shared_ptr<EastWind::BufferState> m_SquareVA;

  // Shader
  std::shared_ptr<EastWind::Shader> m_TriangleShader;
  std::shared_ptr<EastWind::Shader> m_SquareShader;

  // Camera
  EastWind::Camera m_Camera;

  // Background Color
  float *r, *g, *b;

  // Current Mouse Position
  float m_MouseX, m_MouseY;
};


class Sandbox: public EastWind::App
{
public:
  Sandbox()
  {
    PushLayer(new TestLayer(&r,&g,&b));
    PushLayer(new ImGuiLayer(&r,&g,&b));
  }

  ~Sandbox()
  {
  }

  

};

EastWind::App* EastWind::CreateApp()
{
  return new Sandbox();
}
