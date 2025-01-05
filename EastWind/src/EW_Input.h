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
    Vec<float, 6> cam_boundary = cam.GetBoundary();
    // EW_WARN("Camera Boundary: " << cam_boundary);
    float l = cam_boundary(0), r = cam_boundary(1), 
          t = cam_boundary(2), b = cam_boundary(3),
          n = cam_boundary(4), f = cam_boundary(5);
    float x_half = (l - r) / 2.f, y_half = (t - b) / 2.f;
    Vec4 start{ndc_x, ndc_y, -n, 1};
    start *= -(2.f*f*n)/(f-n);
    Mat4 vp_inverse = cam.GetViewProjectionMatrix().Inverse();
    start = vp_inverse * start;
    return Ray(start, cam.GetDirection());
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
