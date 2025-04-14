#include "quaternion.h"
#include <cmath>

namespace EastWind {
  
  inline Quaternion::Quaternion(const Vec<float,3>& axis, const float& angle)
  {
    Vec<float,3> v = axis;
    v.normalize();
    float sine = sinf(angle);
    s = cosf(angle);
    x = v(0) * sine; 
    y = v(1) * sine; 
    z = v(2) * sine;
  }

  inline void Quaternion::Set(float s, float x, float y, float z)
  {
    this->s = s;
    this->x = x;
    this->y = y;
    this->z = z;
  }

  inline void Quaternion::Set(const Vec<float,3>& axis, const float& angle)
  {
    Vec<float,3> v = axis;
    v.normalize();
    float sine = sinf(angle);
    s = cosf(angle);
    x = v(0) * sine; 
    y = v(1) * sine; 
    z = v(2) * sine;
  }

  inline float Quaternion::length() const
  {
    return sqrtf(s*s + x*x + y*y + z*z);
  }

  
  inline Quaternion& Quaternion::normalize()
  {
    const float EPSILON = 0.00001f;
    float d = s*s + x*x + y*y + z*z;
    if (d < EPSILON) return *this; // do nothing if it is zero

    float invLength = 1.0f / sqrtf(d);
    s *= invLength;  x *= invLength;  y *= invLength;  z *= invLength;
    return *this;
  }

  inline Quaternion& Quaternion::conjugate()
  {
    x = -x; y = -y; z = -z;
    return *this;
  }

  inline Quaternion& Quaternion::invert()
  {
    const float EPSILON = 0.00001f;
    float d = s*s + x*x + y*y + z*z;
    if (d < EPSILON) return *this; // do nothing if it is zero

    Quaternion q = *this;
    *this = q.conjugate() * (1.0f/d); // q* / |q||q|
    return *this;
  }


  inline Mat<float,4,4> Quaternion::GetMatrix() const
  {
    // assume quaternion is unit length 
    return Mat<float,4,4>(
        {
          {1 - 2.f*(y*y + z*z),      2.f*(x*y - s*z),      2.f*(x*z + s*y),      0.f}, 
          {    2.f*(x*y + s*z),  1 - 2.f*(x*x + z*z),      2.f*(y*z - s*x),      0.f}, 
          {    2.f*(x*z - s*y),      2.f*(y*z + s*x),  1 - 2.f*(x*x + y*y),      0.f}, 
          {                0.f,                  0.f,                  0.f,      1.f}
        }
    );

    // for non-unit quaternion
    // ss+xx-yy-zz, 2xy+2sz,     2xz-2sy,     0
    // 2xy-2sz,     ss-xx+yy-zz, 2yz-2sx,     0
    // 2xz+2sy,     2yz+2sx,     ss-xx-yy+zz, 0
    // 0,           0,           0,           1
  }

  inline Vec<float,3> Quaternion::GetVector() const
  {
    return {x,y,z};
  }

  // operators
  inline Quaternion Quaternion::operator-() const 
  { 
    return Quaternion(-s,-x,-y,-z); 
  }

  inline Quaternion Quaternion::operator+(const Quaternion& rhs) const 
  { 
    return Quaternion(s+rhs.s,x+rhs.x,y+rhs.y,z+rhs.z); 
  }

  inline Quaternion Quaternion::operator-(const Quaternion& rhs) const 
  {
    return Quaternion(s-rhs.s,x-rhs.x,y-rhs.y,z-rhs.z); 
  }

  inline Quaternion Quaternion::operator*(const float& a) const 
  { 
    return Quaternion(a*s, a*x, a*y, a*z); 
  }

  inline Quaternion Quaternion::operator*(const Quaternion& rhs) const 
  {
    // qq' = [s,v] * [s',v'] = [(ss' - v . v'), v x v' + sv' + s'v]  
    // NOTE: Not Commutative
    Vec<float,3> v1({x,y,z});
    Vec<float,3> v2({rhs.x,rhs.y,rhs.z});

    Vec<float,3> cross = v1.cross(v2);
    float dot = v1 * v2;

    Vec<float,3> v3 = cross + s * v2 + rhs.s * v1;
    return Quaternion(s*rhs.s - dot, v3(0), v3(1), v3(2));
  }

  inline Quaternion Quaternion::operator*(const Vec<float,3>& v) const
  {
    Quaternion q(0,x,y,z);
    return *this * q;
  }


  inline Quaternion& Quaternion::operator+=(const Quaternion& rhs)
  {
    s += rhs.s; x += rhs.x; y += rhs.y; z += rhs.z; 
    return *this;
  }
  
  inline Quaternion& Quaternion::operator-=(const Quaternion& rhs)
  {
    s -= rhs.s; x -= rhs.x; y -= rhs.y; z -= rhs.z; 
    return *this;
  }

  inline Quaternion& Quaternion::operator*=(float a)
  {
    s *= a; x *= a; y *= a; z *= a; 
    return *this;
  }

  inline Quaternion& Quaternion::operator*=(const Quaternion& rhs)
  {
    *this = *this * rhs;
    return *this;
  }

  inline bool Quaternion::operator==(const Quaternion& rhs) const
  {
    return (s == rhs.s) && (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
  }

  inline bool Quaternion::operator!=(const Quaternion& rhs) const
  {
    return (s != rhs.s) || (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
  }



  // Find Quaternion for rotating from v1 to v2;
  inline Quaternion Quaternion::GetQuaternion(const Vec<float,3>& v1, const Vec<float,3>& v2)
  {
    const float EPSILON = 0.001f;
    const float HALF_PI = acos(-1) * 0.5f;

    if (v1 == v2) {
      return Quaternion(v1, 0);
    } else if (v1 == -v2){ // if two vectors are opposite return a perpendicular vector with 180 angle
      Vec<float,3> v;
      if (v1(0) > -EW_EPSILON && v1(0) < EW_EPSILON)
        v = Vec<float,3>({1.f,0.f,0.f});
      else if (v1(1) > -EW_EPSILON && v1(1) < EW_EPSILON)
        v = Vec<float,3>({0.f,1.f,0.f});
      else if (v1(2) > -EW_EPSILON && v1(2) < EW_EPSILON)
        v = Vec<float,3>({0.f,0.f,1.f});
      return Quaternion(v,HALF_PI);
    }

    auto u1 = v1;
    auto u2 = v2;
    u1.normalize();
    u2.normalize();

    auto r = u1.cross(u2);            // rotation axis
    float angle = acosf(u1*u2);       // rotation angle
    return Quaternion(r, angle*0.5f);
  }

  // Euler -> Quaternion
  Quaternion Quaternion::GetQuaternionXYZ(const Vec<float,3>& angles)
  {
    Quaternion qx(Vec<float,3>({1,0,0}), angles(0));
    Quaternion qy(Vec<float,3>({0,1,0}), angles(1));
    Quaternion qz(Vec<float,3>({0,0,1}), angles(2));

    return qz*qy*qx;

  }

  Quaternion Quaternion::GetQuaternionXZY(const Vec<float,3>& angles)
  {
    Quaternion qx(Vec<float,3>({1,0,0}), angles(0));
    Quaternion qy(Vec<float,3>({0,1,0}), angles(1));
    Quaternion qz(Vec<float,3>({0,0,1}), angles(2));

    return qy*qz*qx;
  }

  Quaternion Quaternion::GetQuaternionYXZ(const Vec<float,3>& angles)
  {
    Quaternion qx(Vec<float,3>({1,0,0}), angles(0));
    Quaternion qy(Vec<float,3>({0,1,0}), angles(1));
    Quaternion qz(Vec<float,3>({0,0,1}), angles(2));

    return qz*qx*qy;
  }

  Quaternion Quaternion::GetQuaternionYZX(const Vec<float,3>& angles)
  {
    Quaternion qx(Vec<float,3>({1,0,0}), angles(0));
    Quaternion qy(Vec<float,3>({0,1,0}), angles(1));
    Quaternion qz(Vec<float,3>({0,0,1}), angles(2));

    return qx*qz*qy;
  }

  Quaternion Quaternion::GetQuaternionZXY(const Vec<float,3>& angles)
  {
    Quaternion qx(Vec<float,3>({1,0,0}), angles(0));
    Quaternion qy(Vec<float,3>({0,1,0}), angles(1));
    Quaternion qz(Vec<float,3>({0,0,1}), angles(2));

    return qy*qx*qz;
  }

  Quaternion Quaternion::GetQuaternionZYX(const Vec<float,3>& angles)
  {
    Quaternion qx(Vec<float,3>({1,0,0}), angles(0));
    Quaternion qy(Vec<float,3>({0,1,0}), angles(1));
    Quaternion qz(Vec<float,3>({0,0,1}), angles(2));

    return qx*qy*qz;
  }

  Vec<float,3> Quaternion::Rotate(Vec<float,3> point, Vec<float,3> axis, float radian)
  {
    Quaternion p(0, point(0), point(1), point(2));
    Quaternion q(axis, radian*0.5f);
    Quaternion c = q;
    c.conjugate();
    
    // rotate p by qpq*
    Quaternion p2 = q*p*c;
    return Vec<float,3>({p2.x, p2.y, p2.z});
  }

  Vec<float,3> Quaternion::RotateX(Vec<float,3> point, float radian)
  {
    return Rotate(point, Vec<float,3>({1,0,0}), radian); 
  }
  
  Vec<float,3> Quaternion::RotateY(Vec<float,3> point, float radian)
  {
    return Rotate(point, Vec<float,3>({0,1,0}), radian); 
  }

  Vec<float,3> Quaternion::RotateZ(Vec<float,3> point, float radian)
  {
    return Rotate(point, Vec<float,3>({0,0,1}), radian); 
  }

}
