#include "EW_pch.h"
#include "Camera.h"


namespace EastWind {
  
  Camera::Camera(const Vec<float,3>& position, const Vec<float,3>& up, const Vec<float,3>& direction, const Vec<float,6>& boundary)
    : m_position(position,1.f),
      m_up(up,1.f), 
      m_right(normalize(up.cross(direction)),1.f),
      m_direction(direction,1.f), 
      m_boundary(boundary),
      m_ViewMatrix(cameraRotate(m_right,m_up,m_direction)*cameraTranslate(m_position)), 
      m_ProjectionMatrix(cameraPerspective(boundary)), 
      m_ViewProjectionMatrix(m_ProjectionMatrix*m_ViewMatrix)
  {
    Vec<float,3> right({m_right(0),m_right(1),m_right(2)});
    m_up = Vec<float,4>(normalize(direction.cross(right)), 1.f);
    // std::cout << "m_x" << m_right << std::endl;
    // std::cout << "m_y" << m_up << std::endl;
    // std::cout << "m_z" << m_direction << std::endl;
  }


  void Camera::recalculateViewMat(){
    m_ViewMatrix = cameraRotate(m_right,m_up,m_direction)*cameraTranslate(m_position);
    m_ViewProjectionMatrix = m_ProjectionMatrix*m_ViewMatrix;
  }

  void Camera::lookat(const Vec<float,4>& target){
    m_direction = m_position - target;
    Vec<float,3> z({m_direction(0),m_direction(1),m_direction(2)});
    Vec<float,3> y({m_up(0),m_up(1),m_up(2)});
    Vec<float,3> x = normalize(y.cross(z));
    m_up = Vec<float,4>(normalize(z.cross(x)), 1.f);
    m_right = Vec<float,4>(x, 1.f);
  }

  void Camera::SetPosition(const Vec<float,4>& position){
    m_position = position;
    
    lookat(Vec<float,4>({0,0,0,0}));
    
    recalculateViewMat();
  }

  void Camera::Translate(const Vec<float,3>& direction){
    m_position = translate(direction) * m_position;

    lookat(Vec<float,4>({0,0,0,0}));
    
    recalculateViewMat();
  }


  void Camera::CamRotateX(const float& radian){
    auto R = rotateX(radian);
    m_position = R*m_position;

    lookat(Vec<float,4>({0,0,0,0}));

    recalculateViewMat();
  }

  void Camera::CamRotateY(const float& radian){
    auto R = rotateY(radian);
    m_position = R*m_position;

    lookat(Vec<float,4>({0,0,0,0}));

    recalculateViewMat();
  }
  
  void Camera::CamRotateZ(const float& radian){
    auto R = rotateZ(radian);
    m_position = R*m_position;

    lookat(Vec<float,4>({0,0,0,0}));

    recalculateViewMat();
  }

  void Camera::RotateX(const float& radian){
    auto R = rotateX(radian);
    m_up = R * m_up;
    m_direction = R * m_direction;
    recalculateViewMat();
  }

  void Camera::RotateY(const float& radian){
    auto R = rotateY(radian);
    m_right = R * m_right;
    m_direction = R * m_direction;
    recalculateViewMat();
  }

  void Camera::RotateZ(const float& radian){
    auto R = rotateZ(radian);
    m_up = R * m_up;
    m_right = R * m_right;
    recalculateViewMat();
  }
}
