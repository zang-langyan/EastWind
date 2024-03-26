#ifndef UTILS_H
#define UTILS_H

#include "EW_Lapack.h"
#include "EW_Vector.h"
#include "EW_Matrix.h"

namespace EastWind {

#define PI 3.14159265358979323846264338327950288419716939937510

inline float degree2radian(const float& degree) {
  return degree/180.f*PI;
}

inline double degree2radian(const double& degree) {
  return degree/180.0*PI;
}

inline float radian2degree(const float& radian) {
  return radian/PI*180.f;
}

inline double radian2degree(const double& radian) {
  return radian/PI*180.0;
}


inline float len(const Vec<float,2>& v){
  return slapy2_(&v(0), &v(1));
}

inline double len(const Vec<double,2>& v){
  return dlapy2_(&v(0), &v(1));
}

inline float len(const Vec<float,3>& v){
  return slapy3_(&v(0), &v(1), &v(2));
}

inline double len(const Vec<double,3>& v){
  return dlapy3_(&v(0), &v(1), &v(2));
}



template<typename T, size_t n>
Vec<T,n> normalize(const Vec<T,n>& v){
  Vec<T,n> ans(v);
  if constexpr((n == 2 || n == 3) && (std::is_same_v<T,float> || std::is_same_v<T,double>)){
    float L = len(ans); 
    for (size_t i = 0; i < n; ++i){
      ans(i) /= L;
    }
  }else{
    long double L = 0.;
    for (size_t i = 0; i < n; ++i){
      L += ans(i)*ans(i);
    } 
    for (size_t i = 0; i < n; ++i){
      ans(i) /= L;
    }
  } 
  return ans;
}

}

#endif // !UTILS_H
