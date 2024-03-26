#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "EW_Vector.h"
#include "EW_Matrix.h"
#include <cmath>


namespace EastWind {

  inline Mat<float,4,4> translate(const Vec<float,3>& d){
    return Mat<float,4,4>({
        {1.f,0.f,0.f,d(0)},
        {0.f,1.f,0.f,d(1)},
        {0.f,0.f,1.f,d(2)},
        {0.f,0.f,0.f,1.f}
    });
  }

  inline Mat<float,4,4> scale(const Vec<float,3>& s){
    return Mat<float,4,4>({
        {s(0),0.f,0.f,0.f},
        {0.f,s(1),0.f,0.f},
        {0.f,0.f,s(2),0.f},
        {0.f,0.f,0.f,1.f}
    });
  }

  inline Mat<float,4,4> rotateX(const float& radian){
    return Mat<float,4,4>({
        {1.f,0.f,0.f,0.f},
        {0.f,std::cos(radian),std::sin(radian),0.f},
        {0.f,-std::sin(radian),std::cos(radian),0.f},
        {0.f,0.f,0.f,1.f}
    });
  }

  inline Mat<float,4,4> rotateY(const float& radian){
    return Mat<float,4,4>({
        {std::cos(radian),0.f,std::sin(radian),0.f},
        {0.f,1.f,0.f,0.f},
        {-std::sin(radian),0.f,std::cos(radian),0.f},
        {0.f,0.f,0.f,1.f}
    });
  }

  inline Mat<float,4,4> rotateZ(const float& radian){
    return Mat<float,4,4>({
        {std::cos(radian),std::sin(radian),0.f,0.f},
        {-std::sin(radian),std::cos(radian),0.f,0.f},
        {0.f,0.f,1.f,0.f},
        {0.f,0.f,0.f,1.f}
    });
  }


  inline Mat<float,4,4> cameraTranslate(const Vec<float,4>& position){
    return Mat<float,4,4>({
      {1.f,0.f,0.f,-position(0)},
      {0.f,1.f,0.f,-position(1)},
      {0.f,0.f,1.f,-position(2)},
      {0.f,0.f,0.f,1.f}
    });
  }

  inline Mat<float,4,4> cameraRotate(const Vec<float,4>& right, const Vec<float,4>& up, const Vec<float,4>& direction){
    return Mat<float,4,4>({
      {right(0) ,  right(1),  right(2), 0.f},
      {up(0)    ,  up(1)   ,  up(2)   , 0.f},
      {direction(0), direction(1), direction(2), 0.f},
      {0.f      ,  0.f     ,  0.f     , 1.f}
    }); 
  }

  struct CamFrustum{
    float l,r,t,b,n,f;
    CamFrustum(const Vec<float,6>& props)
      : l(props(0)), r(props(1)), t(props(2)), b(props(3)), n(props(4)), f(props(5))
    {}
  };
  
  inline Mat<float,4,4> cameraPerspective(const CamFrustum& prop){
    return Mat<float,4,4>({
      {2.f*prop.n/(prop.r-prop.l), 0.f, (prop.r+prop.l)/(prop.r-prop.l), 0.f},
      {0.f, 2.f*prop.n/(prop.t-prop.b), (prop.t+prop.b)/(prop.t-prop.b), 0.f},
      {0.f, 0.f, -(prop.f+prop.n)/(prop.f-prop.n), -2.f*prop.f*prop.n/(prop.f-prop.n)},
      {0.f      ,  0.f     ,  -1.f     , 0.f}
    });
  }

  inline Mat<float,4,4> cameraOrtho(const CamFrustum& prop){
    return Mat<float,4,4>({
      {2.f/(prop.r-prop.l), 0.f, 0.f, -(prop.r+prop.l)/(prop.r-prop.l)},
      {0.f, 2.f/(prop.t-prop.b), 0.f, -(prop.t+prop.b)/(prop.t-prop.b)},
      {0.f, 0.f, -2.f/(prop.f-prop.n), -(prop.f+prop.n)/(prop.f-prop.n)},
      {0.f      ,  0.f     ,  0.f     , 1.f}
    });
  }
}

#endif // !TRANSFORM_H
