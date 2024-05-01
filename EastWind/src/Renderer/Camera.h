#ifndef CAMERA_H
#define CAMERA_H

#include <EastWind_Math.h>
#include <EastWind_Graphics.h>

#include "EW_Timestep.h"
#include "Event/Event.h"
#include "Event/MouseEvent.h"
#include "Event/ApplicationEvent.h"

namespace EastWind {

struct CamPosture {
  Vec<float,4> right;
  Vec<float,4> up;
  Vec<float,4> shoot;
};

/* Camera */
class Camera 
{
public:
  Camera(const Vec<float,3>& position, const Vec<float,3>& up, const Vec<float,3>& direction, const Vec<float,6>& boundary);


  const Vec<float,4>& GetPosition() const { return m_position; };
  CamPosture GetPosture() const { return {m_right, m_up, m_direction}; };
  Vec3 GetDirection() const { return m_direction; }; 
  Vec3 GetUpDirection() const { return m_up; }; 
  Vec3 GetRightDirection() const { return m_right; }; 


  void SetPosition(const Vec<float,4>& position);
  void Translate(const Vec<float,3>& direction);

  
  // Rotate the Camera around the world axis (World Space Coordinates)
  void WSRotateX(const float& radian);
  void WSRotateY(const float& radian);
  void WSRotateZ(const float& radian);
  void SetWSRotationX(const float& radian);
  void SetWSRotationY(const float& radian);
  void SetWSRotationZ(const float& radian);
  

  // Rotate the Camera around the self axis (Eye Space Coordinates)
  void ESRotateX(const float& radian);
  void ESRotateY(const float& radian);
  void ESRotateZ(const float& radian);
  void SetESRotationX(const float& radian);
  void SetESRotationY(const float& radian);
  void SetESRotationZ(const float& radian);
  // void Quaternion(const Vec<float,3>& axis, const float& radian);
  

  const Mat<float,4,4>& GetViewMatrix() const { return m_ViewMatrix; }
  const Mat<float,4,4>& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

  void RecalculateProjMat(const Vec<float,6>& frustum);
  void RecalculateViewMat();
  void RecalculateViewMat(Vec<float,4>& x, Vec<float,4>& y, Vec<float,4>& z, Vec<float,4>& position);
  void RecalculateVPMatrix();
  
  CamPosture lookat(const Vec<float,4>& target);
  void SetTarget(const Vec4& t);
private:
  Vec<float,4> m_position;
  Vec<float,4> m_up;
  Vec<float,4> m_direction;
  Vec<float,4> m_right;
  Vec<float,6> m_boundary;

  Mat<float,4,4> m_ViewMatrix;
  Mat<float,4,4> m_ProjectionMatrix;
  Mat<float,4,4> m_ViewProjectionMatrix;

  Vec<float,4> m_target;
};


/* Camera Controller */
class CameraController
{
public:
  CameraController(const float& aspectRatio);

  void OnUpdate(Timestep ts);
  void OnEvent(Event& e);

  Camera& GetCamera() { return m_Camera; }
  const Camera& GetCamera() const { return m_Camera; }

  float GetZoomLevel() const { return m_ZoomLevel; }
  void SetZoomLevel(const float& level) { m_ZoomLevel = level; }
private:
  bool OnMouseScrolled(MouseScrolledEvent& e);
  bool OnWindowResized(WindowResizeEvent& e);
private:
  float m_AspectRatio;
  float m_ZoomLevel = 1.f;
  float m_CamMovingSpeed = 1.f;

  Vec<float,3> m_CamPosition;

  Camera m_Camera;
};

}


#endif // !CAMERA_H
