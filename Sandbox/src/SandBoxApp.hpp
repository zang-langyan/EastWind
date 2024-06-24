#include <EastWind.h>
#include <EastWind_Math.h>
#include <EastWind_Graphics.h>

#include <Platform/SDK/OpenGL/OpenGLShader.h>

#include "ImGui/ImGuiLayer.h"
#include "MeshLayer.h"
class TestLayer: public EastWind::Layer
{
public:
  TestLayer(float* r, float* g, float* b)
    : Layer("Test"),
      m_Camera(1280.f/720.f),
      r(r),g(g),b(b)
  {
    /**************************************/
    // Triangle   
    /**************************************/
    // Buffer State Manager
    m_TriangleVA = EastWind::BufferState::Create();

    // Vertex Buffer Creation
    {
      EastWind::Ref<EastWind::VertexBuffer> vertexBuffer;
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
      EastWind::Ref<EastWind::IndexBuffer> indexBuffer;
      uint32_t indices[3] = {0,1,2};
      indexBuffer.reset(EastWind::IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
      m_TriangleVA->SetIndexBuffer(indexBuffer);
    }

    // TriangleShader
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
      m_TriangleShader = EastWind::Shader::Create("TriangleShader",vertexSrc, fragmentSrc);
    }
    /**************************************/


    /**************************************/
    // Square   
    /**************************************/
    // Buffer State Manager
    m_SquareVA = EastWind::BufferState::Create();

    // Vertex Buffer Creation
    {
      EastWind::Ref<EastWind::VertexBuffer> vertexBuffer;
      float vertices[5*4] = {
        -0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
         0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
         0.75f,  0.75f, 0.0f, 1.0f, 1.0f,
        -0.75f,  0.75f, 0.0f, 0.0f, 1.0f
      };
      vertexBuffer.reset(EastWind::VertexBuffer::Create(vertices, sizeof(vertices)));
      EastWind::BufferLayout layout = {
        { EastWind::ShaderDataType::Float3, "aPos"   },
        { EastWind::ShaderDataType::Float2, "aTexCoord"   },
      };
      vertexBuffer->SetLayout(layout);
      m_SquareVA->AddVertexBuffer(vertexBuffer);
    }

    // Index Buffer Creation
    {
      EastWind::Ref<EastWind::IndexBuffer> indexBuffer;
      uint32_t indices[6] = {0,1,2,2,3,0};
      indexBuffer.reset(EastWind::IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
      m_SquareVA->SetIndexBuffer(indexBuffer);
    }

    // SquareShader
    {
      std::string vertexSrc = R"(
        #version 330 core

        layout (location = 0) in vec3 aPos;

        uniform mat4 u_VPMatrix;
        uniform mat4 u_ModelMatrix;

        out vec3 v_Position;

        void main()
        {
            v_Position = aPos;
            gl_Position = u_ModelMatrix * u_VPMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
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
      m_SquareShader = EastWind::Shader::Create("SquareShader", vertexSrc, fragmentSrc);
    }
    
    // TextureShader
    {
      m_TextureShader = m_ShaderLib.Load(TEXTURE_SHADER_PATH);
    }


    // Texture
    {
      m_CheckerBoardTexture = EastWind::Texture2D::Create(RIVERROCK_TEXTURE_PATH); 
      m_HammerSickelTexture = EastWind::Texture2D::Create(HAMMERSICKEL_TEXTURE_PATH); 
      std::dynamic_pointer_cast<EastWind::OpenGLShader>(m_TextureShader)->Bind();
      std::dynamic_pointer_cast<EastWind::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0); // upload texture based on slot
    }
    /**************************************/

  }

  void OnUpdate(EastWind::Timestep ts) override
  {
    m_Camera.OnUpdate(ts);

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

    // EastWind::Renderer::Upload(m_SquareShader, "u_ModelMatrix", m_ModelMatrix);
    EastWind::Renderer::Upload(m_TextureShader, "u_ModelMatrix", m_ModelMatrix);
    m_CheckerBoardTexture->Bind();
    EastWind::Renderer::Submit(m_TextureShader, m_SquareVA);
    m_HammerSickelTexture->Bind();
    EastWind::Renderer::Submit(m_TextureShader, m_SquareVA);

    // EastWind::Renderer::Submit(m_TriangleShader, m_TriangleVA);

    EastWind::Renderer::EndScene();
    // --------- Scene End ---------


    if (EastWind::Input::IsKeyPressed(EW_KEY_TAB))
      EW_TRACE("Polling: Tab Key Is Pressed!");
  }

  void OnEvent(EastWind::Event& e) override
  {
    m_Camera.OnEvent(e);
    // if (e.GetEventType() == EastWind::EventType::KeyPressed){
    //   EastWind::KeyPressedEvent& event = (EastWind::KeyPressedEvent&)e;
    //   if (event.GetKeyCode() == EW_KEY_TAB){
    //     EW_CORE_WARN("Tab Key is Pressed");
    //   }  
    //   EW_INFO( (char)event.GetKeyCode() ); 
    // }
  }

private:
  // Buffer Area
  EastWind::Ref<EastWind::BufferState> m_TriangleVA;
  EastWind::Ref<EastWind::BufferState> m_SquareVA;

  // ShaderLibrary
  EastWind::ShaderLibrary m_ShaderLib;

  // Shader
  EastWind::Ref<EastWind::Shader> m_TriangleShader;
  EastWind::Ref<EastWind::Shader> m_SquareShader;
  EastWind::Ref<EastWind::Shader> m_TextureShader;

  // Texture
  EastWind::Ref<EastWind::Texture2D> m_CheckerBoardTexture;
  EastWind::Ref<EastWind::Texture2D> m_HammerSickelTexture;

  // Camera Controller
  EastWind::CameraController m_Camera;

  // Background Color
  float *r, *g, *b;

  // Current Mouse Position
  float m_MouseX, m_MouseY;

  // Model Matrix
  EastWind::Mat<float,4,4> m_ModelMatrix;
};


class Sandbox: public EastWind::App
{
public:
  Sandbox()
  {
    // PushLayer(new TestLayer(&r,&g,&b));
    PushLayer(new MeshLayer(&r,&g,&b));
    PushLayer(new ImGuiLayer(&r,&g,&b));
  }

  ~Sandbox()
  {
  }

private:
  float r = 0.f,g = 0.f,b = 0.f;
};

