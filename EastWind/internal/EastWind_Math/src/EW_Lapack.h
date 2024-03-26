/* EW_Lapack.h
 * ============ BLAS Declaration ============ 
 *
 * This file gives the LAPACK subroutines and functions declarations 
 * defined in Fortran LAPACK library that are currently used in 
 * EastWind_Math library
 *
 * ============
 * Author: Langyan Zang
 * Last Modified: 2024-3-8
 */


#ifndef EW_LAPACK_H
#define EW_LAPACK_H


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/****************************************
 * LAPACK Driver Routines
****************************************/
// ================== LU ==================
// AX = B
// A - n x n, B - n x nrhs, X - n x nrhs
// A = P * L * U
void sgesv_(int* N, int* NRHS, float A[], int* LDA, int IPIV[], float B[], int* LDB, int* INFO);
void dgesv_(int* N, int* NRHS, double A[], int* LDA, int IPIV[], double B[], int* LDB, int* INFO);




/****************************************
 * LAPACK Computational Routines
****************************************/
// ================== LU ==================
//  SGETRS solves a system of linear equations
 //    A * X = B  or  A**T * X = B
 // with a general N-by-N matrix A using the LU factorization computed
 // by SGETRF.
void sgetrs_(char* TRANS, int* N, int* NRHS, float LU[], int* LDA, int IPIV[], float B[], int* LDB, int* INFO);
void dgetrs_(char* TRANS, int* N, int* NRHS, double LU[], int* LDA, int IPIV[], double B[], int* LDB, int* INFO);


/****************************************
 * BLAS-like Routines
****************************************/
// ================== Matrix Norm ==================
// General Matrix
//    SLANGE = ( max(abs(A(i,j))), NORM = 'M' or 'm'
//             (
//             ( norm1(A),         NORM = '1', 'O' or 'o'
//             (
//             ( normI(A),         NORM = 'I' or 'i'
//             (
//             ( normF(A),         NORM = 'F', 'f', 'E' or 'e'
//
// where  norm1  denotes the  one norm of a matrix (maximum column sum),
// normI  denotes the  infinity norm  of a matrix  (maximum row sum) and
// normF  denotes the  Frobenius norm of a matrix (square root of sum of
// squares).  Note that  max(abs(A(i,j)))  is not a consistent matrix norm.
float slange_(char* NORM, int* M, int* N, float A[], int* LDA, float WORK[]);
double dlange_(char* NORM, int* M, int* N, double A[], int* LDA, double WORK[]);



// ================== Scalar Operations ==================
// Robust sqrt(x^2+y^2)
float slapy2_(float* x, float* y);
double dlapy2_(double* x, double* y);

// Robust sqrt(x^2+y^2+z^2)
float slapy3_(float* x, float* y, float* z);
double dlapy3_(double* x, double* y, double* z);




#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !EW_LAPACK_H
