/* EW_Matrix.h
 * ============ EastWind Matrix Class ============ 
 *
 * Template definition of EastWind Matrix Class
 *
 * ============
 * Author: Langyan Zang
 * Last Modified: 2024-2-28
 */
#ifndef EW_MATRIX_H
#define EW_MATRIX_H

#include "EW_Blas.h"
#include "EW_Lapack.h"
#include "EW_Vector.h"
namespace EastWind {

template<typename T, size_t m>
struct LU {
  T* lu = new T[m*m];
  int* ipiv = new int[m];
  int info;
};

  
template<typename T, std::size_t m, std::size_t n>
class Mat 
{
template<typename T_V, std::size_t k>
friend class Vec;

template<typename T_M, std::size_t h, std::size_t k>
friend class Mat;

private:
  // T** mat = new T*[m];
  T* F_mat = new T[m*n];

  bool band = false;
  bool sym = false; 
  char tri = 'N';

private:
  LU<T,m> LU;

// ==========================================================================
//   * Constructor
// ==========================================================================
public:
  Mat(){
    for (size_t i = 0; i < m; ++i){
      // mat[i] = new T[n];
      for (size_t j = 0; j < n; ++j){
        // mat[i][j] = (i == j? 1 : 0);
        F_mat[m*j+i] = (i == j? 1 : 0);
      }
    }
  }

  Mat(T ele){
    for (size_t i = 0; i < m; ++i){
      // mat[i] = new T[n];
      for (size_t j = 0; j < n; ++j){
        // mat[i][j] = ele;
        F_mat[m*j+i] = i == j ? ele : 0;
      }
    }
  }

  Mat(const Vec<T,m>& v){
    static_assert(n == 1 && "n must be 1 to initialize with Vec!"); 
    for (size_t i = 0; i < m; ++i){
      F_mat[i] = v(i);
    }
  }

  Mat(T const (&arr)[m][n]){
    for (size_t i = 0; i < m; ++i){
      // mat[i] = new T[n];
      for (size_t j = 0; j < n; ++j){
        // mat[i][j] = arr[i][j];
        F_mat[m*j+i] = arr[i][j];
      }
    }
  }

  Mat(const Mat<T,m,n>& other){
    for (size_t i = 0; i < m; ++i){
      // mat[i] = new T[n];
      for (size_t j = 0; j < n; ++j){
        // mat[i][j] = other[i][j];
        F_mat[m*j+i] = other(i,j);
      }
    }
  }

  Mat<T,m,n>& operator=(const Mat<T,m,n>& other){
    for (size_t i = 0; i < m; ++i){
      // mat[i] = new T[n];
      for (size_t j = 0; j < n; ++j){
        // mat[i][j] = other[i][j];
        F_mat[m*j+i] = other(i,j);
      }
    }
    return *this; 
  }

  Mat(const Mat<T,m,n>&& other){
    for (size_t i = 0; i < m; ++i){
      // mat[i] = new T[n];
      for (size_t j = 0; j < n; ++j){
        // mat[i][j] = other[i][j];
        F_mat[m*j+i] = other(i,j);
      }
    }
  }

  Mat<T,m,n>& operator=(const Mat<T,m,n>&& other){
    for (size_t i = 0; i < m; ++i){
      // mat[i] = new T[n];
      for (size_t j = 0; j < n; ++j){
        // mat[i][j] = other[i][j];
        F_mat[m*j+i] = other(i,j);
      }
    }
    return *this; 
  }

  virtual ~Mat() {
    // for (size_t i = 0; i < m; ++i)
    //   delete[] mat[i]; 
    //
    // delete[] mat;
    delete[] F_mat;
  };
  
// ==========================================================================
//   * IO Stream 
// ==========================================================================
public:
  friend std::ostream& operator<<(std::ostream& os, Mat<T,m,n>& mat){
    os << "Mat<" << m << "," << n << ">" << std::endl;
    for (size_t i = 0; i < m; ++i){
      os << "[";
      for (size_t j = 0; j < n; ++j){
        os << mat(i,j) << (j == n-1 ? "" : ", ");
      }
      os << "]";
      os << std::endl;
    }
    return os;
  }

  friend std::ostream& operator<<(std::ostream& os, Mat<T,m,n>&& mat){
    os << "Mat<" << m << "," << n << ">" << std::endl;
    for (size_t i = 0; i < m; ++i){
      os << "[";
      for (size_t j = 0; j < n; ++j){
        os << mat(i,j) << (j == n-1 ? "]" : ", ");
      }
      os << std::endl;
    }
    return os;
  }

// ==========================================================================
//   * Indexing
// ==========================================================================
public:
  // auto operator[] (const size_t& i) {
  //   return *(mat+i); 
  // }

  T& operator() (const size_t& i, const size_t& j) const {
    return *(F_mat+i+m*j);
  }

// ==========================================================================
//   * Pointer
// ==========================================================================
public:
  T* get_ptr() const {
    return F_mat;
  }

// ==========================================================================
//   * Utility
// ==========================================================================
private:
  void check_band(){
    this->band = false; 
  }

  void check_sym(){
    if (m != n){
      this->sym = false; 
      return;
    }

    for (size_t i = 0; i < n; ++i){
      for (size_t j = i; j < n; ++j){
        if (*(F_mat+i+m*j) != *(F_mat+j+m*i)){
          this->sym = false;
          return;
        }
      }
    }

    this->sym = true;
  }

  void check_tri(){
    if (m != n){
      this->tri = 'N'; 
      return;
    }

    bool U = true, L = true;
    for (size_t i = 0; i < n; ++i){
      for (size_t j = 0; j < n; ++j){
        T entry = *(F_mat+i+m*j);
        if (i > j && entry != 0){
          L = false;
        }else if (i < j && entry != 0){
          U = false;
        } 

        if (!U && !L){
          this->tri = 'N'; 
          return;
        }
      }
    }

    if (U){
      this->tri = 'U';
    }else{
      this->tri = 'L';
    }
  }

public:
  bool& is_band(){
    return this->band;
  }

  bool& is_sym(){
    check_sym();
    return this->sym;
  }

  char& is_tri(){
    check_tri();
    return this->tri;
  }

// ==========================================================================
//   * Transpose
// ==========================================================================
public:
  Mat<T,n,m> Trans(){
    Mat<T,n,m> ans;
    for (size_t i = 0; i < m; ++i){
      for (size_t j = 0; j < n; ++j){
        ans(j,i) = F_mat[m*j+i];
      }
    } 
    return ans;
  }

// ==========================================================================
//   * Resize
// ==========================================================================
public:
  template<size_t h, size_t k>
  Mat<T,h,k> resize(){
    static_assert(h*k == m*n && "Resize size does not match!");
    Mat<T,h,k> ans;
    for (size_t i = 0; i < h; ++i){
      for (size_t j = 0; j < k; ++j){
        ans(i,j) = F_mat[h*j+i];
      }
    } 
    return ans;
  } 

// ==========================================================================
//   * Inverse
// ==========================================================================
public:
  Mat<T,m,n> Inverse(){
    static_assert(n == m && "Matrix must be square to compute inverse");
    Mat<T,m,n> I;
    int N = n;
    for (size_t i = 0; i < m*m; ++i){
      LU.lu[i] = F_mat[i];
    }
    if constexpr(std::is_same_v<T,float>){
      sgesv_(&N, &N, LU.lu, &N, LU.ipiv, I.F_mat, &N, &LU.info); 
    } else if constexpr(std::is_same_v<T,double>){
      dgesv_(&N, &N, LU.lu, &N, LU.ipiv, I.F_mat, &N, &LU.info); 
    }
  
    if (LU.info > 0){
      std::cout << "U(" << LU.info << "," << LU.info << ") is exactly zero.\n The factorization"
                " has been completed, but the factor U is exactly"
                " singular, so the solution could not be computed." 
                << std::endl;
    } else if (LU.info < 0) {
      std::cout << "the" << -LU.info << "-th argument had an illegal value" << std::endl;
    }

    return I;
  }

// ==========================================================================
//   * Norm
// ==========================================================================
public:
  auto norm(char level){
    if constexpr(std::is_same_v<T,float>){
      int M = m, N = n;
      float WORK[(level == 'I'? m : 1)];
      return slange_(&level, &M, &N, F_mat, &M, WORK);
    }else if constexpr(std::is_same_v<T,double>){
      int M = m, N = n;
      double WORK[(level == 'I'? m : 1)];
      return dlange_(&level, &M, &N, F_mat, &M, WORK);
    }else{
      if (level == 'M'){
        T ans = std::abs(F_mat[0]);
        for (size_t i = 1; i < m*n; ++i){
          if (std::abs(F_mat[i]) > ans){
            ans = std::abs(F_mat[i]);
          } 
        }
        return ans;
      }else if (level == '1'){
        long long ans;
        for (size_t j = 0; j < n; ++j){
          long long col_sum = 0LL;
          for (size_t i = 0; i < m; ++i){
            col_sum += F_mat[m*j+i];
          }
          if (j == 0 || col_sum > ans) ans = col_sum;
        }
        return ans;
      }else if (level == 'I'){
        long long ans;
        for (size_t i = 0; i < n; ++i){
          long long row_sum = 0LL;
          for (size_t j = 0; j < m; ++j){
            row_sum += F_mat[m*j+i];
          }
          if (i == 0 || row_sum > ans) ans = row_sum;
        }
        return ans;
      }else if (level == 'F'){
        long double ans = 0.;
        for (size_t i = 0; i < m*n; ++i){
          ans += F_mat[i]*F_mat[i];
        }
        return static_cast<double>(std::sqrt(ans));
      }
    }
  }

// ==========================================================================
//   * Operators
// ==========================================================================
public:
  friend Mat<T,m,n> operator+(const Mat<T,m,n>& m1, const Mat<T,m,n> m2){
    Mat<T,m,n> ans;
    for (size_t i = 0; i < m; ++i){
      for (size_t j = 0; j < n; ++j){
        ans(i,j) = m1(i,j) + m2(i,j);
      }
    }
    return ans;
  } 

  Mat<T,m,n>& operator+=(const Mat<T,m,n> other){
    for (size_t i = 0; i < m; ++i){
      for (size_t j = 0; j < n; ++j){
         *(F_mat+i+m*j) += other(i,j);
      }
    }
    check_band();
    check_sym();
    check_tri();
    return *this;
  }

  friend Mat<T,m,n> operator-(const Mat<T,m,n>& m1, const Mat<T,m,n> m2){
    Mat<T,m,n> ans;
    for (size_t i = 0; i < m; ++i){
      for (size_t j = 0; j < n; ++j){
        ans(i,j) = m1(i,j) - m2(i,j);
      }
    }
    return ans;
  } 

  Mat<T,m,n>& operator-=(const Mat<T,m,n> other){
    for (size_t i = 0; i < m; ++i){
      for (size_t j = 0; j < n; ++j){
         *(F_mat+i+m*j) -= other(i,j);
      }
    }
    check_band();
    check_sym();
    check_tri();
    return *this;
  }

  friend Vec<T,m> operator*(const Mat<T,m,n>& mat, const Vec<T,n>& vec){
    Vec<T,m> ans;
    
    if constexpr(std::is_same_v<T,float>){
      char TRANS = 'N';
      int M = m, N = n;
      int LDA = m;
      float ALPHA = 1;
      int INCX = 1, INCY = 1;
      float BETA = 1;
      sgemv_(&TRANS, &M, &N, &ALPHA, mat.F_mat, &LDA, vec.get_ptr(), &INCX, &BETA, ans.get_ptr(), &INCY);
    }else if constexpr(std::is_same_v<T,double>){
      char TRANS = 'N';
      int M = m, N = n;
      int LDA = m;
      double ALPHA = 1;
      int INCX = 1, INCY = 1;
      double BETA = 1;
      dgemv_(&TRANS, &M, &N, &ALPHA, mat.F_mat, &LDA, vec.get_ptr(), &INCX, &BETA, ans.get_ptr(), &INCY);
    }else{
      for (size_t i = 0; i < m; ++i){
        T entry = 0;
        for (size_t j = 0; j < n; ++j){
          entry += mat(i,j)*vec(j);
        }
        ans(i) = entry; 
      }
    }
    return ans;
  }

  template<size_t k>
  friend Mat<T,m,k> operator*(const Mat<T,m,n>& mat1, const Mat<T,n,k>& mat2){
    Mat<T,m,k> ans(0);

    char TRANSA = 'N', TRANSB = 'N';
    int M = m, N = k, K = n;
    int LDA = m, LDB = n, LDC = m;

    if constexpr(std::is_same_v<T,float>){
      float ALPHA = 1, BETA = 1;
      sgemm_(&TRANSA, &TRANSB, &M, &N, &K, &ALPHA, mat1.F_mat, &LDA, mat2.F_mat, &LDB, &BETA, ans.F_mat, &LDC);
    } else if constexpr(std::is_same_v<T,double>){
      double ALPHA = 1, BETA = 1;
      dgemm_(&TRANSA, &TRANSB, &M, &N, &K, &ALPHA, mat1.F_mat, &LDA, mat2.F_mat, &LDB, &BETA, ans.F_mat, &LDC);
    } else {
      for (size_t i = 0; i < m; ++i){
        for (size_t j = 0; j < n; ++j){
          T entry = 0;
          for (size_t h = 0; h < k; ++h){
            entry += mat1(i,h)*mat2(h,j);
          }
          ans(i,j) = entry;
        }
      }
    }

    return ans;
  }



};

}


#endif // !EW_MATRIX_H
