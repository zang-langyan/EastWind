#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include <EastWind.h> 

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>


class ImGuiLayer: public EastWind::Layer 
{
public:
  ImGuiLayer(float* r, float* g, float* b)
    :r(r),g(g),b(b)
  {

  }

  ~ImGuiLayer() override
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void OnAttach() override
  {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
                                                        

    ImGuiStyle& style = ImGui::GetStyle();
    {
      // std::cout << "Viewports enabled" << std::endl;
      style.WindowRounding = 8.0f;
      style.FrameRounding = 7.0f;
      
      // style.Colors[ImGuiCol_WindowBg].w = 1.0f;
      // light style
      // style.Colors[ImGuiCol_WindowBg] = ImVec4(0.8f, 1.0f, 1.0f, 0.35f);
      // dark style
      style.Colors[ImGuiCol_WindowBg] = ImVec4(0.2f, 0.25f, 0.20f, 0.80f);
    }

    // Font Set Up
    io.Fonts->AddFontFromFileTTF(IMGUI_FONT_PATH, 20.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());




    // Setup Platform/Renderer backends
    EastWind::App& app = EastWind::App::Get();

    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow()), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
  }

  void OnUpdate(EastWind::Timestep ts) override
  {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    float time = (float)glfwGetTime();
		io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
		m_time = time;

    ImGui::NewFrame();
    ImGui::ShowDemoWindow(); // Show demo window! :)

    {
      ImVec4 clear_color = ImVec4(*r, *g, *b, 1.00f);
      ImGui::Begin("Setup");

      // Edit a color stored as 4 floats
      ImGui::ColorEdit4("Background Color", (float*)&clear_color);

      *r = clear_color.x;
      *g = clear_color.y;
      *b = clear_color.z;

      auto cursor_pos = EastWind::Input::GetMousePosition();
      ImGui::Text("Cursor Position");
      ImGui::TextColored(ImVec4(0.1f, 0.9f, 0.2f, 1.0f), "X: %f", cursor_pos.first);
      ImGui::SameLine();
      ImGui::TextColored(ImVec4(0.1f, 0.8f, 0.6f, 1.0f), "Y: %f", cursor_pos.second);
      ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.6f, 1.0f), "Window Size: %d x %d", window_width, window_height);

      ImGui::End();
    } 

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
  }


  void OnEvent(EastWind::Event& e) override
  {
    if (e.GetEventType() == EastWind::EventType::WindowResize){
      EastWind::WindowResizeEvent& event = (EastWind::WindowResizeEvent&) e;
      window_width = event.GetWidth();
      window_height = event.GetHeight();
    }
  }

private:
  float m_time = 0.f;
  float *r, *g, *b;
  uint32_t window_width = 1280, window_height = 720;

// private:
//   void clamp(float& r, float& g, float& b){
//     if (r < 0.) r = 0.;
//     else if (r > 255.) r = 1.;
//     else if (r > 1. && r < 255.) r /= 255.;
//
//     if (g < 0.) g = 0.;
//     else if (g > 255.) g = 1.;
//     else if (g > 1. && g < 255.) g /= 255.;
//
//     if (b < 0.) b = 0.;
//     else if (b > 255.) b = 1.;
//     else if (b > 1. && b < 255.) b /= 255.;
//   }
};

#endif // !IMGUILAYER_H
