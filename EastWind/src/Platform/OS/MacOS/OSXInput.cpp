#include "EW_pch.h"

#include "OSXInput.h"

#include "EW_App.h"

#include <GLFW/glfw3.h>

namespace EastWind {

  Input* Input::s_InputInstance = new OSXInput();

  bool OSXInput::IsKeyPressedImpl(int keycode)  
  {
    GLFWwindow* window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
  }

  bool OSXInput::IsMouseButtonPressedImpl(int button)  
  {
    GLFWwindow* window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
  }

  std::pair<float, float> OSXInput::GetMousePositionImpl()  
  {
    GLFWwindow* window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return { (float)xpos, (float)ypos };
  }

  float OSXInput::GetMouseXImpl()
  {
    auto [x, y] = GetMousePositionImpl();
    return x;
  }

  float OSXInput::GetMouseYImpl()
  {
    auto [x, y] = GetMousePositionImpl();
    return y;
  }
}
