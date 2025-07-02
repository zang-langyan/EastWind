#ifndef EW_INPUT_H
#define EW_INPUT_H

#include "EW_App.h"
#include "Geometry/Ray.h"

namespace EastWind {

class Input
{
public:
  inline static bool IsKeyPressed(int keycode) { return s_InputInstance->IsKeyPressedImpl(keycode); }
  inline static bool IsMouseButtonPressed(int button) { return s_InputInstance->IsMouseButtonPressedImpl(button); }

  inline static std::pair<float, float> GetMousePosition() { return s_InputInstance->GetMousePositionImpl(); }
  inline static float GetMouseX() { return s_InputInstance->GetMouseXImpl(); }
  inline static float GetMouseY() { return s_InputInstance->GetMouseYImpl(); }

public:
  static const Ray GetCursorRay() {
    const auto& [mousePosX, mousePosY] = Input::GetMousePosition();
    EastWind::App& app = EastWind::App::Get();
    Window& window = app.GetWindow();
    uint32_t window_width = window.GetWidth();
    uint32_t window_height = window.GetHeight();
    float ndc_x = (mousePosX / window_width ) * 2.f - 1.f;
    float ndc_y = (mousePosY / window_height) * 2.f - 1.f;

    const CameraController& camera_controller = CameraController::instance();
    const Camera& cam = camera_controller.GetCamera();
    float n = cam.GetBoundary()(4);

    Vec4 start = cam.GetPosition();
    Vec4 dest{n*ndc_x, n*ndc_y, -n, n}; 
    Mat4 vp_inverse = cam.GetViewProjectionMatrix().Inverse();
    dest = vp_inverse * dest;
    return Ray(start, dest-start);
  }

protected:
  virtual bool IsKeyPressedImpl(int keycode) = 0;
  virtual bool IsMouseButtonPressedImpl(int button) = 0;

  virtual std::pair<float, float> GetMousePositionImpl() = 0;
  virtual float GetMouseXImpl() = 0;
  virtual float GetMouseYImpl() = 0;
private:
  static Input* s_InputInstance;
};

}

#endif // !EW_INPUT_H
