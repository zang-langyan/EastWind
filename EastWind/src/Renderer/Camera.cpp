#include "EW_pch.h"
#include "EW_Core.h"
#include "Camera.h"
#include "EW_Input.h"
#include "EW_Codes.h"
#include "Event.h"
#include "MouseEvent.h"
#include "ApplicationEvent.h"

#include "EW_Log.h"

namespace EastWind {
  
  /* Camera */
  // NOTE!!!
  // the direction represents the actual camera space z axis vector dir
  // rather than where the camera is shooting at
  Camera::Camera(const Vec<float,3>& position, const Vec<float,3>& up, const Vec<float,3>& direction, const Vec<float,6>& boundary)
    : m_position(position,1.f),
      m_up(normalize(up),1.f), 
      m_right(normalize(up.cross(direction)),1.f),
      m_direction(normalize(direction),1.f), 
      m_boundary(boundary)
      // m_ViewMatrix(cameraRotate(m_right,m_up,m_direction)*cameraTranslate(m_position)), 
      // m_ProjectionMatrix(cameraPerspective(boundary)), 
      // m_ViewProjectionMatrix(m_ProjectionMatrix*m_ViewMatrix)
  {
    Vec<float,3> right({m_right(0),m_right(1),m_right(2)});
    m_up = Vec<float,4>(normalize(direction.cross(right)), 1.f);
    // std::cout << "m_x" << m_right << std::endl;
    // std::cout << "m_y" << m_up << std::endl;
    // std::cout << "m_z" << m_direction << std::endl;

    m_ViewMatrix = cameraRotate(m_right,m_up,m_direction)*cameraTranslate(m_position);
    m_ProjectionMatrix = cameraPerspective(boundary);
    m_ViewProjectionMatrix = m_ProjectionMatrix*m_ViewMatrix;
  }

  void Camera::RecalculateProjMat(const Vec<float,6>& frustum){
    m_boundary = frustum;
    m_ProjectionMatrix = cameraPerspective(m_boundary);
    RecalculateVPMatrix();
  }

  void Camera::RecalculateViewMat(){
    m_ViewMatrix = cameraRotate(m_right,m_up,m_direction)*cameraTranslate(m_position);
    RecalculateVPMatrix();
  }

  void Camera::RecalculateViewMat(Vec<float,4>& x, Vec<float,4>& y, Vec<float,4>& z, Vec<float,4>& position){
    m_ViewMatrix = cameraRotate(x,y,z)*cameraTranslate(position);
    RecalculateVPMatrix();
  }

  void Camera::RecalculateVPMatrix(){
    m_ViewProjectionMatrix = m_ProjectionMatrix*m_ViewMatrix;
  }

  void Camera::SetTarget(const Vec4& t){
    m_target = t;
  }

  CamPosture Camera::lookat(const Vec<float,4>& target){
    auto direction = m_position - target;
    Vec<float,3> z({direction(0),direction(1),direction(2)});
    // Vec<float,3> y({m_up(0),m_up(1),m_up(2)});
    Vec<float,3> y({0.f,0.f,1.f});
    Vec<float,3> x = normalize(y.cross(z));
    auto up = Vec<float,4>(normalize(z.cross(x)), 1.f);
    auto right = Vec<float,4>(x, 1.f);
    direction = Vec<float,4>(normalize(z), 1.f);
    return {right, up, direction};
  }

  void Camera::SetPosition(const Vec<float,4>& position){
    m_position = position;
    
    auto [x,y,z] = lookat(m_target);
    m_right = x;
    m_up = y;
    m_direction = z;
    
    RecalculateViewMat();
  }

  void Camera::Translate(const Vec<float,3>& direction){
    m_position = translate(direction) * m_position;

    // auto [x,y,z] = lookat(m_target);
    // m_right = x;
    // m_up = y;
    // m_direction = z;

    RecalculateViewMat();
  }


  void Camera::WSRotateX(const float& radian){
    // auto R = rotateX(radian);
    // auto position = R*m_position;
    auto position = Vec<float,4>(Quaternion::RotateX(m_position, radian),1.f);

    auto [x,y,z] = lookat(m_target);

    RecalculateViewMat(x,y,z,position);
  }

  void Camera::WSRotateY(const float& radian){
    // auto R = rotateY(radian);
    // auto position = R*m_position;
    auto position = Vec<float,4>(Quaternion::RotateY(m_position, radian),1.f);

    auto [x,y,z] = lookat(m_target);

    RecalculateViewMat(x,y,z,position);
  }
  
  void Camera::WSRotateZ(const float& radian){
    // auto R = rotateZ(radian);
    // auto position = R*m_position;
    auto position = Vec<float,4>(Quaternion::RotateZ(m_position, radian),1.f);

    auto [x,y,z] = lookat(m_target);

    RecalculateViewMat(x,y,z,position);
  }

  void Camera::SetWSRotationX(const float& radian){
    // auto R = rotateX(radian);
    // m_position = R*m_position;
    m_position = Vec<float,4>(Quaternion::RotateX(m_position, radian), 1.f);

    auto [x,y,z] = lookat(m_target);
    m_right = x;
    m_up = y;
    m_direction = z;

    RecalculateViewMat();
  }

  void Camera::SetWSRotationY(const float& radian){
    // auto R = rotateY(radian);
    // m_position = R*m_position;
    m_position = Vec<float,4>(Quaternion::RotateY(m_position, radian), 1.f);

    auto [x,y,z] = lookat(m_target);
    m_right = x;
    m_up = y;
    m_direction = z;

    RecalculateViewMat();
  }
  
  void Camera::SetWSRotationZ(const float& radian){
    // auto R = rotateZ(radian);
    // m_position = R*m_position;
    m_position = Vec<float,4>(Quaternion::RotateZ(m_position, radian), 1.f);

    auto [x,y,z] = lookat(m_target);
    m_right = x;
    m_up = y;
    m_direction = z;

    RecalculateViewMat();
  }

  void Camera::ESRotateX(const float& radian){
    auto R = rotateX(radian);
    auto y = R * m_up;
    auto z = R * m_direction;
    RecalculateViewMat(m_right, y, z, m_position);
  }

  void Camera::ESRotateY(const float& radian){
    auto R = rotateY(radian);
    auto x = R * m_right;
    auto z = R * m_direction;
    RecalculateViewMat(x, m_up, z, m_position);
  }

  void Camera::ESRotateZ(const float& radian){
    auto R = rotateZ(radian);
    auto y = R * m_up;
    auto x = R * m_right;
    RecalculateViewMat(x, y, m_direction, m_position);
  }

  void Camera::SetESRotationX(const float& radian){
    auto R = rotateX(radian);
    m_up = R * m_up;
    m_direction = R * m_direction;
    RecalculateViewMat();
  }

  void Camera::SetESRotationY(const float& radian){
    auto R = rotateY(radian);
    m_right = R * m_right;
    m_direction = R * m_direction;
    RecalculateViewMat();
  }

  void Camera::SetESRotationZ(const float& radian){
    auto R = rotateZ(radian);
    m_up = R * m_up;
    m_right = R * m_right;
    RecalculateViewMat();
  }




  /* Camera Controller */
  CameraController::CameraController(const float& aspectRatio)
    : m_AspectRatio(aspectRatio),
      m_Camera(
          Vec<float,3>({2.f,2.f,2.f}), // position
          Vec<float,3>({0.f,0.f,1.f}), // up
          Vec<float,3>({2.f,2.f,1.f}), // direction (z axis)
          Vec<float,6>({-m_AspectRatio*m_ZoomLevel,m_AspectRatio*m_ZoomLevel,-m_ZoomLevel,m_ZoomLevel,1.f,200.f}) // boundary
      )
  {
    m_Camera.SetTarget(Vec4({0.f,0.f,1.f,1.f}));
  }


  void CameraController::OnUpdate(Timestep ts)
  {
    if (Input::IsKeyPressed(EW_KEY_A)){
      m_Camera.SetWSRotationZ(-m_CamMovingSpeed*ts);
    } else if (Input::IsKeyPressed(EW_KEY_D)){
      m_Camera.SetWSRotationZ(m_CamMovingSpeed*ts);
    } else if (Input::IsKeyPressed(EW_KEY_W)){
      auto z = m_Camera.GetPosture().shoot;
      Vec<float,3> dir({z(0), z(1), z(2)});
      m_Camera.Translate(-m_CamMovingSpeed*ts*dir);
    } else if (Input::IsKeyPressed(EW_KEY_S)){
      auto z = m_Camera.GetPosture().shoot;
      Vec<float,3> dir({z(0), z(1), z(2)});
      m_Camera.Translate(m_CamMovingSpeed*ts*dir);
    }
  }
  
  void CameraController::OnEvent(Event& e)
  {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(
      BIND_EVENT_FN(CameraController::OnMouseScrolled)
    );
    dispatcher.Dispatch<WindowResizeEvent>(
      BIND_EVENT_FN(CameraController::OnWindowResized)
    );

  }

  bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
  {
    m_ZoomLevel -= e.GetYOffset()*0.25f;
    m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

    m_Camera.RecalculateProjMat(Vec<float,6>({-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, 1.f, 200.f}));
    return false;
  }

  bool CameraController::OnWindowResized(WindowResizeEvent& e)
  {
    m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    m_Camera.RecalculateProjMat(Vec<float,6>({-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, 1.f, 200.f}));
		return false;
  }
}
