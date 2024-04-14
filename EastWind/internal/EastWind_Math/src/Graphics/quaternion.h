#ifndef QUATERNION_H
#define QUATERNION_H

#include "EW_Vector.h"
#include "EW_Matrix.h"


namespace EastWind {

/*
 *  Class Quaternion
 *  q = s + ix + jy + kz
 *    = [s, v]
 *    = S(q) + V(q)
 *
 *  where,
 *    S(q) = B/A cos(\theta)
 *    V(q) = \vec r · B/A sin(\theta)
 *
 *  When the quaternion is used for 3D rotation, initialize the quaternion with
 *  the rotation axis (unit vector) and the half rotation angle (radian) because
 *  of double multiplication, qpq*.
 *    q = cos(a) + sin(a)(ix + jy + kz)
 *
 *  If there is no rotation, initialize the quaternion (1,0,0,0) because of cos(0) = 1, sin(0) = 0
 */
class Quaternion
{
public:
  float s, x, y, z;

public:
  Quaternion(): s(0.f), x(0.f), y(0.f), z(0.f) {}
  Quaternion(float s_, float x_, float y_, float z_): s(s_), x(x_), y(y_), z(z_) {}; 
  Quaternion(const Vec<float,3>& axis, const float& angle); // angle needs to be radian

  // util funcs
  void Set(float s, float x, float y, float z); 
  void Set(const Vec<float,3>& axis, const float& angle); 

  float length() const;
  Quaternion& normalize();
  Quaternion& conjugate();
  Quaternion& invert();

  Mat<float,4,4> GetMatrix() const;
  Vec<float,3>   GetVector() const;

  // operators
  Quaternion operator-() const;
  Quaternion operator+(const Quaternion& rhs) const;
  Quaternion operator-(const Quaternion& rhs) const;
  Quaternion operator*(const float& a) const;                // scaler multiplication
  Quaternion operator*(const Quaternion& rhs) const;  // multiplication
  Quaternion operator*(const Vec<float,3>& v) const;  // conjugate multiplication
  Quaternion& operator+=(const Quaternion& rhs);
  Quaternion& operator-=(const Quaternion& rhs);
  Quaternion& operator*=(float a);
  Quaternion& operator*=(const Quaternion& rhs); 
  bool operator==(const Quaternion& rhs) const;
  bool operator!=(const Quaternion& rhs) const;
  friend Quaternion operator*(const float& a, const Quaternion& q);  // multiplication

  friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);

public:
  // Find Quaternion for rotating from v1 to v2;
  static Quaternion GetQuaternion(const Vec<float,3>& v1, const Vec<float,3>& v2);

  // Euler -> Quaternion
  static Quaternion GetQuaternionXYZ(const Vec<float,3>& angles);
  static Quaternion GetQuaternionXZY(const Vec<float,3>& angles);
  static Quaternion GetQuaternionYXZ(const Vec<float,3>& angles);
  static Quaternion GetQuaternionYZX(const Vec<float,3>& angles);
  static Quaternion GetQuaternionZXY(const Vec<float,3>& angles);
  static Quaternion GetQuaternionZYX(const Vec<float,3>& angles);

public:
  static Vec<float,3> Rotate(Vec<float,3> point, Vec<float,3> axis, float radian);
  static Vec<float,3> RotateX(Vec<float,3> point, float radian);
  static Vec<float,3> RotateY(Vec<float,3> point, float radian);
  static Vec<float,3> RotateZ(Vec<float,3> point, float radian);
};

// friend functions
inline Quaternion operator*(const float& a, const Quaternion& q)
{
  return Quaternion(a*q.s, a*q.x, a*q.y, a*q.z);
}

inline std::ostream& operator<<(std::ostream& os, const Quaternion& q)
{
  os << "Quaternion: (" << q.s << ", " << q.x << ", " << q.y << ", " <<  q.z << ")"; 
  return os;
}

}

#endif // !QUATERNION_H
