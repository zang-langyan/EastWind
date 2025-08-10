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
    float ndc_y = 1.f - (mousePosY / window_height) * 2.f;

    const CameraController& camera_controller = CameraController::instance();
    const Camera& cam = camera_controller.GetCamera();
    Vec4 start = cam.GetPosition();
    
    // float n = cam.GetBoundary()(4);
    // Vec4 dest{n*ndc_x, n*ndc_y, -n, n}; 
    // Mat4 vp_inverse = cam.GetViewProjectionMatrix().Inverse();
    // dest = vp_inverse * dest;
    // dest /= dest(3); // Normalize the homogeneous coordinates
    // return Ray(start, dest-start);
    Vec4 near_plane_point{ndc_x, ndc_y, -1.0f, 1.0f};
    Vec4  far_plane_point{ndc_x, ndc_y,  1.0f, 1.0f};
    Mat4 vp_inverse = cam.GetViewProjectionMatrix().Inverse();
    near_plane_point = vp_inverse * near_plane_point;
    near_plane_point /= near_plane_point(3);
    far_plane_point = vp_inverse * far_plane_point;
    far_plane_point /= far_plane_point(3);
    // Vec3 direction = far_plane_point-near_plane_point;
    return Ray(start, far_plane_point-near_plane_point);
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
