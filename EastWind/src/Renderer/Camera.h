#ifndef CAMERA_H
#define CAMERA_H

#include <EastWind_Math.h>
#include <EastWind_Graphics.h>

namespace EastWind {

struct CamPosture {
  Vec<float,4> right;
  Vec<float,4> up;
  Vec<float,4> shoot;
};

class Camera 
{
public:
  Camera(const Vec<float,3>& position, const Vec<float,3>& up, const Vec<float,3>& direction, const Vec<float,6>& boundary);


  const Vec<float,4>& GetPosition() const { return m_position; };
  void SetPosition(const Vec<float,4>& position);
  void Translate(const Vec<float,3>& direction);

  CamPosture GetPosture() const { return {m_right, m_up, m_direction}; };

 
  // Rotate the Camera around the world axis
  void CamRotateX(const float& radian);
  void CamRotateY(const float& radian);
  void CamRotateZ(const float& radian);
  

  // Rotate the Camera around the self axis (Eye Space Coordinates)
  // void SetRotation(const float& radian);
  void RotateX(const float& radian);
  void RotateY(const float& radian);
  void RotateZ(const float& radian);
  // void Quaternion(const Vec<float,3>& axis, const float& radian);
  

  const Mat<float,4,4>& GetViewMatrix() const { return m_ViewMatrix; }
  const Mat<float,4,4>& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

private:
  void recalculateViewMat();
  void lookat(const Vec<float,4>& target);
private:
  Vec<float,4> m_position;
  Vec<float,4> m_up;
  Vec<float,4> m_direction;
  Vec<float,4> m_right;
  Vec<float,6> m_boundary;

  Mat<float,4,4> m_ViewMatrix;
  Mat<float,4,4> m_ProjectionMatrix;
  Mat<float,4,4> m_ViewProjectionMatrix;
};

}


#endif // !CAMERA_H
