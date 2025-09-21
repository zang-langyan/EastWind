/* EW_Vector.h
 * ============ EastWind Matrix Class ============ 
 *
 * Template definition of EastWind Vector Class
 *
 * ============
 * Author: Langyan Zang
 * Last Modified: 2024-2-28
 */
#ifndef EW_VECTOR_H
#define EW_VECTOR_H

#include <cassert>
#include <iostream>
#include <cmath>
#include <climits>
#include <vector>

#ifdef _MSC_BUILD
#define EWMATH_MKL
#endif

#ifdef EWMATH_MKL
#include "mkl.h"
#else
#include "EW_Blas.h"
#endif

#define EW_EPSILON 0.001f

namespace EastWind {

template<typename T, std::size_t m, std::size_t n>
class Mat;

template<typename T, std::size_t n>
class Vec 
{
template<typename T_M, std::size_t m, std::size_t k>
friend class Mat;

private:
  T* v = new T[n];
public:
// ==========================================================================
//   * Constructor
// ==========================================================================
  Vec(){
    for (size_t i = 0; i < n; ++i){
      v[i] = 0;
    }
  }

  Vec(T ele){
    for (size_t i = 0; i < n; ++i){
      v[i] = ele;
    }
  }

  Vec(T const (&arr)[n]){
    for (size_t i = 0; i < n; ++i){
      v[i] = arr[i];
    }
  }

  Vec(std::initializer_list<T> lst){
    for (size_t i = 0; i < n; ++i){
      v[i] = i < lst.size() ? *(lst.begin()+i) : 0;
    }
  }

  Vec(std::vector<T> lst){
    for (size_t i = 0; i < n; ++i){
      v[i] = i < lst.size() ? *(lst.begin()+i) : 0;
    }
  }

  template<size_t k>
  Vec(const Vec<T,k>& other, T fill){
    static_assert(k < n && "use the normal constructor");
    for (size_t i = 0; i < n; ++i){
      v[i] = i < k ? other(i) : fill;
    }
  }
  
  Vec(const Vec<T,n>& other){
    for (size_t i = 0; i < n; ++i){
      v[i] = other(i);
    }
  }

  template<size_t k>
  Vec(const Vec<T,k>& other){
    for (size_t i = 0; i < n; ++i){
      v[i] = other(i);
    }
  }

  Vec<T,n>& operator=(const Vec<T,n>& other){
    for (size_t i = 0; i < n; ++i){
      v[i] = other(i);
    }
    return *this; 
  }
  
  Vec(Vec<T,n>&& other){
    for (size_t i = 0; i < n; ++i){
      v[i] = other(i);
    }
  }

  Vec<T,n>& operator=(Vec<T,n>&& other){
    for (size_t i = 0; i < n; ++i){
      v[i] = other(i);
    }
    return *this; 
  }

  virtual ~Vec() { delete[] v; };
  
// ==========================================================================
//   * IO Stream 
// ==========================================================================
  friend std::ostream& operator<<(std::ostream& os, const Vec<T,n>& v){
    os << "Vec<" << n << ">" << std::endl;
    os << "[";
    for (size_t i = 0; i < n; ++i){
      os << v(i) << (i == n-1 ? "" : ", ");
    }
    os << "]";
    os << std::endl;
    return os;
  }

  friend std::ostream& operator<<(std::ostream& os, const Vec<T,n>&& v){
    os << "Vec<" << n << ">" << std::endl;
    os << "[";
    for (size_t i = 0; i < n; ++i){
      os << v(i) << (i == n-1 ? "]" : ", ");
    }
    os << std::endl;
    return os;
  }


// ==========================================================================
//   * Indexing
// ==========================================================================
  // T& operator[](const size_t& idx) const {
  //   assert(idx < n && "Index must less than n!");
  //   return *(v+idx); 
  // }
  
  T& operator()(const size_t& idx) const {
    assert(idx < n && "Index must less than n!");
    return *(v+idx); 
  }

// ==========================================================================
//   * Pointer
// ==========================================================================
public:
  T* get_ptr() const {
    return v;
  }  

// ==========================================================================
//   * Operators
// ==========================================================================
  template<typename F>
  auto operator+(const Vec<F,n>& other) const {
    Vec<decltype(v[0]+other(0)), n> ans;
    for (size_t i = 0; i < n; ++i){
      ans(i) = v[i]+other(i);
    }
    return ans;
  }

  template<typename F>
  auto operator+(const Vec<F,n>&& other) const {
    Vec<decltype(v[0]+other(0)), n> ans;
    for (size_t i = 0; i < n; ++i){
      ans(i) = v[i]+other(i);
    }
    return ans;
  }

  Vec<T,n> operator+(const T x) const {
    Vec<T, n> ans;
    for (size_t i = 0; i < n; ++i){
      ans(i) = v[i]+x;
    }
    return ans;
  }

  friend auto operator+(const T x, const Vec<T,n>& vec){
    Vec<decltype(x+v[0]),n> ans;
    for (size_t i = 0; i < n; ++i){
      ans(i) = x+vec(i);
    } 
    return ans;
  }

  Vec<T,n>& operator+=(const Vec<T,n>& other){
    for (size_t i = 0; i < n; ++i){
      v[i] += other(i);
    }
    return *this;
  }

  Vec<T,n>& operator+=(const T& x){
    for (size_t i = 0; i < n; ++i){
      v[i] += x;
    }
    return *this;
  }

  Vec<T,n> operator-() const {
    Vec<T,n> ans;
    for (size_t i = 0; i < n; ++i){
      ans(i) = -v[i];
    }
    return ans;
  }

  Vec<T,n> operator-(const Vec<T, n>& other) const {
    Vec<T,n> ans;
    for (size_t i = 0; i < n; ++i){
      ans(i) = v[i]-other(i);
    }
    return ans;
  }

  Vec<T,n> operator-(const T x) const {
    Vec<T,n> ans;
    for (size_t i = 0; i < n; ++i){
      ans(i) = v[i]-x;
    }
    return ans;
  }

  friend Vec<T,n> operator-(const T x, const Vec<T,n>& vec){
    Vec<T,n> ans;
    for (size_t i = 0; i < n; ++i){
      ans(i) = x-vec(i);
    } 
    return ans;
  }

  Vec<T,n>& operator-=(const Vec<T,n>& other){
    for (size_t i = 0; i < n; ++i){
      v[i] -= other(i);
    }
    return *this;
  }

  Vec<T,n>& operator-=(const T& x){
    for (size_t i = 0; i < n; ++i){
      v[i] -= x;
    }
    return *this;
  }



  T dot(const Vec<T,n>& Vy) const {
#ifdef EWMATH_MKL
    if constexpr(std::is_same_v<T,float>){
      // std::cout << "SINGLE PRECISION .dot" << std::endl;
      MKL_INT N = n;
      MKL_INT INCX = 1, INCY = 1;
      return cblas_sdot(N, v, INCX, Vy.v, INCY);
    } else if constexpr(std::is_same_v<T,double>){
      // std::cout << "DOUBLE PRECISION .dot" << std::endl;
      MKL_INT N = n;
      MKL_INT INCX = 1, INCY = 1;
      return cblas_ddot(N, v, INCX, Vy.v, INCY);
#else
    if constexpr(std::is_same_v<T,float>){
      // std::cout << "SINGLE PRECISION .dot" << std::endl;
      int N = n;
      int INCX = 1, INCY = 1;
      return sdot_(&N, v, &INCX, Vy.v, &INCY);
    } else if constexpr(std::is_same_v<T,double>){
      // std::cout << "DOUBLE PRECISION .dot" << std::endl;
      int N = n;
      int INCX = 1, INCY = 1;
      return ddot_(&N, v, &INCX, Vy.v, &INCY);
#endif
    } else {
      // std::cout << "OTHER PRECISION .dot" << std::endl;
      T ans = 0;
      for (size_t i = 0; i < n; ++i){
        ans += v[i]*Vy(i);
      }
      return ans;
    }
  }
  
  friend T operator*(const Vec<T,n>& Vx, const Vec<T,n>& Vy){
    return Vx.dot(Vy);
  }

  friend Vec<T,n> operator*(const T& x, const Vec<T,n>& Vy){
    Vec<T,n> ans;
    for (size_t i = 0; i < n; ++i){
      ans(i) = Vy(i)*x;
    }
    return ans;
  }

  Vec<T,n> operator*(const T& x) const {
    Vec<T,n> ans;
    for (size_t i = 0; i < n; ++i){
      ans(i) = v[i]*x;
    }
    return ans;
  }

  Vec<T,n>& operator*=(const T& scaler) {
    for (size_t i = 0; i < n; ++i){
      v[i] *= scaler;
    }
    return *this;
  }

  Vec<T,n> operator/(const T& x) const {
    Vec<T,n> ans;
    for (size_t i = 0; i < n; ++i){
      ans(i) = v[i]/x;
    }
    return ans;
  }

  Vec<T,n>& operator/=(const T& scaler){
    for (size_t i = 0; i < n; ++i){
      v[i] /= scaler;
    }
    return *this;
  }

  // Comparison
  bool operator==(const Vec<T,n>& other) const {
    for (size_t i = 0; i < n; ++i){
      if (v[i]-other(i) > EW_EPSILON){
        return false;
      }
    }
    return true;
  }

  bool operator==(const Vec<T,n>&& other) const {
    for (size_t i = 0; i < n; ++i){
      if (v[i]-other(i) > EW_EPSILON){
        return false;
      }
    }
    return true;
  }

  bool operator!=(const Vec<T,n>& other) const {
    for (size_t i = 0; i < n; ++i){
      if (v[i]-other(i) > EW_EPSILON){
        return true;
      }
    }
    return false;
  }

  bool operator!=(const Vec<T,n>&& other) const {
    for (size_t i = 0; i < n; ++i){
      if (v[i]-other(i) > EW_EPSILON){
        return true;
      }
    }
    return false;
  }
private:
  T _norm1() const {
    T ans = 0;
    for (size_t i = 0; i < n; ++i){
      ans += abs(v[i]);
    }
    return ans;
  }

  auto _norm2() const {
#ifdef EWMATH_MKL
    if constexpr(std::is_same_v<T,float>){
      MKL_INT N = n;
      MKL_INT INCX = 1;
      return cblas_snrm2(N, v, INCX);
    }else if constexpr(std::is_same_v<T, double>){
      MKL_INT INCX = 1;
      MKL_INT N = n;
      return cblas_dnrm2(N, v, INCX);
#else
    if constexpr(std::is_same_v<T,float>){
      int N = n;
      int INCX = 1;
      return snrm2_(&N, v, &INCX);
    }else if constexpr(std::is_same_v<T, double>){
      int N = n;
      int INCX = 1;
      return dnrm2_(&N, v, &INCX);
#endif
    }else{
      long double ans = 0;
      for (size_t i = 0; i < n; ++i){
        ans += v[i]*v[i];
      }
      return static_cast<double>(std::sqrt(ans));
    }
  }

  T _normInf() const {
    T ans = INT_MIN;
    for (size_t i = 0; i < n; ++i){
      if (abs(v[i]) > ans) ans = abs(v[i]);
    }
    return ans;
  }
public:
  T norm(char level) const {
    if (level == '1'){
      return _norm1();
    }else if (level == '2'){
      return _norm2(); 
    }else if (level == 'I'){
      return _normInf();
    }
    return _norm2();
  }

  T norm() const {
    return _norm2();
  }

  Vec<T,n> cross(const Vec<T,n>& Vy) const {
    Vec<T,n> ans;

    for (size_t i = 0; i < n; ++i){
      T x1 = v[(i+1)%n], x2 = v[(i+2)%n];
      T y1 = Vy((i+1)%n), y2 = Vy((i+2)%n);
      ans(i) = x1*y2 - x2*y1;
    }

    return ans;
  }

  // void swap(Vec<T,n>& Vy){
  //   int N = n;
  //   int INCX = 1, INCY = 1;
  //   if constexpr(std::is_same_v(T,float)){
  //     sswap(&N, v, &INCX, Vy.v, &INCY);
  //   }else if constexpr(std::is_same_v(T,double)){
  //     dswap()
  //   }
  // }
 
  void normalize() {
    T L = this->norm(); 
    for (size_t i = 0; i < n; ++i){
      v[i] /= L;
    }
  }
};



}
#endif // !EW_VECTOR_H
