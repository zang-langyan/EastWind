/* EW_Blas.h
 * ============ BLAS Declaration ============ 
 *
 * This file gives the BLAS subroutines and functions declarations 
 * defined in Fortran BLAS library that are currently used in 
 * EastWind_Math library
 *
 * ============
 * Author: Langyan Zang
 * Last Modified: 2024-3-2
 */


#ifndef EW_BLAS_H
#define EW_BLAS_H


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*
 * BLAS Level 1
 */
// Max Index of Array Absolute Value
int isamax_(int* N, float SX[], int* INCX);
int idamax_(int* N, double DX[], int* INCX);

// Sum of Absolute value in an Array
float sasum_(int* N, float SX[], int* INCX);
double dasum_(int* N, double DX[], int* INCX);

// Constant * Vec SX + Vec SY 
void saxpy_(int* N, float* SA, float SX[], int* INCX, float SY[], int* INCY);
void daxpy_(int* N, double* DA, double DX[], int* INCX, double DY[], int* INCY);

// Copy a Vector SX to Vector SY 
void scopy_(int* N, float SX[], int* INCX, float SY[], int* INCY);
void dcopy_(int* N, double DX[], int* INCX, double DY[], int* INCY);

// Dot Product (Inner Product)
float sdot_(int* N, float SX[], int* INCX, float SY[], int* INCY);
double ddot_(int* N, double DX[], int* INCX, double DY[], int* INCY);

// Accumulated dot product to start point SB
float sdsdot_(int* N, float* SB, float SX[], int* INCX, float SY[], int* INCY);

// Euclidean Norm 
float snrm2_(int* N, float X[], int* INCX);
double dnrm2_(int* N, double X[], int* INCX);

// Apply a Plane Rotation by C(cos(theta)), S(Sin(theta)) to each pair of SX and SY 
// This subroutine is applying a *clockwise* rotation by theta degree
// Notice the difference between it and the rotation matrix
void srot_(int* N, float SX[], int* INCX, float SY[], int* INCY, float* C, float* S);
void drot_(int* N, double DX[], int* INCX, double DY[], int* INCY, double* C, double* S);


/*
! srotg and drotg
  ! srotg(a,b,c,s) computes the Givens rotation parameters c and s such that
  ! [ c s ] [ a ] = [ r ]
  ! [-s c ] [ b ] = [ 0 ]
  ! where r is the hypotenuse of a and b.
  ! drotg(a,b,c,s) is the double precision version of srotg.
  ! a => r
  ! b => z ; z is for the recovery of c & s
  ! c => cos(phi) ; phi is the angle of rotation
  ! s => sin(phi) ; phi is the angle of rotation

!> The computation uses the formulas
!>    sigma = sgn(a)    if |a| >  |b|
!>          = sgn(b)    if |b| >= |a|
!>    r = sigma*sqrt( a**2 + b**2 )
!>    c = 1; s = 0      if r = 0
!>    c = a/r; s = b/r  if r != 0
!> The subroutine also computes
!>    z = s    if |a| > |b|,
!>      = 1/c  if |b| >= |a| and c != 0
!>      = 1    if c = 0
!> This allows c and s to be reconstructed from z as follows:
!>    If z = 1, set c = 0, s = 1.
!>    If |z| < 1, set c = sqrt(1 - z**2) and s = z.
!>    If |z| > 1, set c = 1/z and s = sqrt( 1 - c**2).
*/
// a <intout>, b <intout>, c <out>, s <out>
// [a, b] in: intial vector => [a, 0] out: rotated;
// b => z: for c, s recovery
// c get cos(theta), also contained in b<out>
// s get sin(theta), also contained in b<out>
void srotg_(float* a, float* b, float* c, float* s);
void drotg_(double* a, double* b, double* c, double* s);


/*
*>    CONSTRUCT THE MODIFIED GIVENS TRANSFORMATION MATRIX H WHICH ZEROS
*>    THE SECOND COMPONENT OF THE 2-VECTOR  (SQRT(SD1)*SX1,SQRT(SD2)*>    SY2)**T.
*>    WITH SPARAM(1)=SFLAG, H HAS ONE OF THE FOLLOWING FORMS..
*>
*>    SFLAG=-1.E0     SFLAG=0.E0        SFLAG=1.E0     SFLAG=-2.E0
*>
*>      (SH11  SH12)    (1.E0  SH12)    (SH11  1.E0)    (1.E0  0.E0)
*>    H=(          )    (          )    (          )    (          )
*>      (SH21  SH22),   (SH21  1.E0),   (-1.E0 SH22),   (0.E0  1.E0).
*>    LOCATIONS 2-4 OF SPARAM CONTAIN SH11,SH21,SH12, AND SH22
*>    RESPECTIVELY. (VALUES OF 1.E0, -1.E0, OR 0.E0 IMPLIED BY THE
*>    VALUE OF SPARAM(1) ARE NOT STORED IN SPARAM.)
*>
*>    THE VALUES OF GAMSQ AND RGAMSQ SET IN THE DATA STATEMENT MAY BE
*>    INEXACT.  THIS IS OK AS THEY ARE ONLY USED FOR TESTING THE SIZE
*>    OF SD1 AND SD2.  ALL ACTUAL SCALING OF DATA IS DONE USING GAM.
*>
*/
void srotmg_(float* SD1, float* SD2, float* SX1, float* SY1, float (&SPARAM)[5]);
void drotmg_(double* DD1, double* DD2, double* DX1, double* DY1, double (&DPARAM)[5]);

/*
*>    APPLY THE MODIFIED GIVENS TRANSFORMATION, H, TO THE 2 BY N MATRIX
*>
*>    (SX**T) , WHERE **T INDICATES TRANSPOSE. THE ELEMENTS OF SX ARE IN
*>    (SX**T)
*>
*>    SX(LX+I*INCX), I = 0 TO N-1, WHERE LX = 1 IF INCX .GE. 0, ELSE
*>    LX = (-INCX)*N, AND SIMILARLY FOR SY USING USING LY AND INCY.
*>    WITH SPARAM(1)=SFLAG, H HAS ONE OF THE FOLLOWING FORMS..
*>
*>    SFLAG=-1.E0     SFLAG=0.E0        SFLAG=1.E0     SFLAG=-2.E0
*>
*>      (SH11  SH12)    (1.E0  SH12)    (SH11  1.E0)    (1.E0  0.E0)
*>    H=(          )    (          )    (          )    (          )
*>      (SH21  SH22),   (SH21  1.E0),   (-1.E0 SH22),   (0.E0  1.E0).
*>    SEE  SROTMG FOR A DESCRIPTION OF DATA STORAGE IN SPARAM.
*>
*/
void srotm_(int* N, float SX[], int* INCX, float SY[], int* INCY, float (&SPARAM)[5]);
void drotm_(int* N, double DX[], int* INCX, double DY[], int* INCY, double (&DPARAM)[5]);

// Scale a Vector by a Scalar
void sscal_(int* N, float* SA, float SX[], int* INCX);
void dscal_(int* N, double* DA, double DX[], int* INCX);


// Swap two vectors
void sswap_(int* N, float SX[], int* INCX, float SY[], int* INCY);
void dswap_(int* N, double DX[], int* INCX, double DY[], int* INCY);



/*
 * BLAS Level 2
 */
// y := alpha*A*x + beta*y,   or   y := alpha*A**T*x + beta*y
void sgemv_(char* TRANS, int* M, int* N, float* ALPHA, float A[], int* LDA, float X[], int* INCX, float* BETA, float Y[], int* INCY);
void dgemv_(char* TRANS, int* M, int* N, double* ALPHA, double A[], int* LDA, double X[], int* INCX, double* BETA, double Y[], int* INCY);

// y := alpha*A*x + beta*y,   or   y := alpha*A**T*x + beta*y
// A is m x n band matrix with kl sub-diagonals and ku super-diagonals
void sgbmv_(char* TRANS, int* M, int* N, int* KL, int* KU, float* ALPHA, float A[], int* LDA, float X[], int* INCX, float* BETA, float Y[], int* INCY);
void dgbmv_(char* TRANS, int* M, int* N, int* KL, int* KU, double* ALPHA, double A[], int* LDA, double X[], int* INCX, double* BETA, double Y[], int* INCY);

// y := alpha*A*x + beta*y
// A is an n x n symmetric matrix
void ssymv_(char* UPLO, int* N, float* ALPHA, float A[], int* LDA, float X[], int* INCX, float* BETA, float Y[], int* INCY);
void dsymv_(char* UPLO, int* N, double* ALPHA, double A[], int* LDA, double X[], int* INCX, double* BETA, double Y[], int* INCY);

// y := alpha*A*x + beta*y
// A is an n x n symmetric band matrix
void ssbmv_(char* UPLO, int* N, int* K, float* ALPHA, float A[], int* LDA, float X[], int* INCX, float* BETA, float Y[], int* INCY);
void dsbmv_(char* UPLO, int* N, int* K, double* ALPHA, double A[], int* LDA, double X[], int* INCX, double* BETA, double Y[], int* INCY);

// y := alpha*A*x + beta*y
// A is an n x n symmetric matrix, supplied in packed form
void sspmv_(char* UPLO, int* N, float* ALPHA, float AP[], float X[], int* INCX, float* BETA, float Y[], int* INCY);
void dspmv_(char* UPLO, int* N, double* ALPHA, double AP[], double X[], int* INCX, double* BETA, double Y[], int* INCY);

// x := A*x,   or   x := A**T*x
// where x is an n element vector and  A is an n by n unit, or non-unit upper or lower triangular band matrix
// with ( k + 1 ) diagonals
void strmv_(char* UPLO, char* TRANS, char* DIAG, int* N, int* K, float A[], int* LDA, float X[], int* INCX);
void dtrmv_(char* UPLO, char* TRANS, char* DIAG, int* N, int* K, double A[], int* LDA, double X[], int* INCX);

// x := A*x,   or   x := A**T*x
// where x is an n element vector and  A is an n by n unit, or non-unit upper or lower triangular matrix
// DIAG specifies whether or not A is unit triangular
void stbmv_(char* UPLO, char* TRANS, char* DIAG, int* N, float A[], int* LDA, float X[], int* INCX);
void dtbmv_(char* UPLO, char* TRANS, char* DIAG, int* N, double A[], int* LDA, double X[], int* INCX);


// x := A*x,   or   x := A**T*x
// where x is an n element vector and  A is an n by n unit, or non-unit upper or lower triangular matrix supplied in packed form
void stpmv_(char* UPLO, char* TRANS, char* DIAG, int* N, float AP[], float X[], int* INCX);
void dtpmv_(char* UPLO, char* TRANS, char* DIAG, int* N, double AP[], double X[], int* INCX);


//  Solves one of the systems of equations:
//  A*x = b,   or   A**T*x = b
//  where b and x are n element vectors and A is an n by n unit, or non-unit, upper or lower triangular matrix.
void strsv_(char* UPLO, char* TRANS, char* DIAG, int* N, float A[], int* LDA, float X[], int* INCX);
void dtrsv_(char* UPLO, char* TRANS, char* DIAG, int* N, double A[], int* LDA, double X[], int* INCX);

//  Solves one of the systems of equations:
//  A*x = b,   or   A**T*x = b
//  where b and x are n element vectors and A is an n by n unit, or non-unit, upper or lower triangular band matrix.
void stbsv_(char* UPLO, char* TRANS, char* DIAG, int* N, int* K, float A[], int* LDA, float X[], int* INCX);
void dtbsv_(char* UPLO, char* TRANS, char* DIAG, int* N, int* K, double A[], int* LDA, double X[], int* INCX);

//  Solves one of the systems of equations:
//  A*x = b,   or   A**T*x = b
//  where b and x are n element vectors and A is an n by n unit, or non-unit, upper or lower triangular matrix suppiled in packed form.
void stpsv_(char* UPLO, char* TRANS, char* DIAG, int* N, float AP[], float X[], int* INCX);
void dtpsv_(char* UPLO, char* TRANS, char* DIAG, int* N, double AP[], double X[], int* INCX);

// SGER Performs the rank 1 operation
// A := alpha*x*y**T + A
// where alpha is a scalar, x is an m element vector, y is an n element vector and A is an m by n matrix.
void sger_(int* M, int* N, float* ALPHA, float X[], int* INCX, float Y[], int* INCY, float A[], int* LDA);
void dger_(int* M, int* N, double* ALPHA, double X[], int* INCX, double Y[], int* INCY, double A[], int* LDA);

// SSYR Performs the symmetric rank 1 operation
// A := alpha*x*x**T + A
// where alpha is a scalar, x is an n element vector and A is an n by n symmetric matrix.
void ssyr_(char* UPLO, int* N, float* ALPHA, float X[], int* INCX, float A[], int* LDA);
void dsyr_(char* UPLO, int* N, double* ALPHA, double X[], int* INCX, double A[], int* LDA);

// SSYR Performs the symmetric rank 1 operation
// A := alpha*x*x**T + A
// where alpha is a scalar, x is an n element vector and A is an n by n symmetric matrix, suppiled in packed form.
void sspr_(char* UPLO, int* N, float* ALPHA, float X[], int* INCX, float AP[]);
void dspr_(char* UPLO, int* N, double* ALPHA, double X[], int* INCX, double AP[]);

// SSYR2  performs the symmetric rank 2 operation
//    A := alpha*x*y**T + alpha*y*x**T + A,
// where alpha is a scalar, x and y are n element vectors and A is an n by n symmetric matrix.
void ssyr2_(char* UPLO, int* N, float* ALPHA, float X[], int* INCX, float Y[], int* INCY, float A[], int* LDA);
void dsyr2_(char* UPLO, int* N, double* ALPHA, double X[], int* INCX, double Y[], int* INCY, double A[], int* LDA);

// SSPR2  performs the symmetric rank 2 operation
//    A := alpha*x*y**T + alpha*y*x**T + A,
// where alpha is a scalar, x and y are n element vectors and A is an n by n symmetric matrix, supplied in packed form.
void sspr2_(char* UPLO, int* N, float* ALPHA, float X[], int* INCX, float Y[], int* INCY, float AP[]);
void dspr2_(char* UPLO, int* N, double* ALPHA, double X[], int* INCX, double Y[], int* INCY, double AP[]);


/*
 * BLAS Level 3
 */
// SGEMM
// C := alpha*op( A )*op( B ) + beta*C
// where  op( X ) is one of
//    op( X ) = X   or   op( X ) = X**T,
// alpha and beta are scalars, and A, B and C are matrices, with op( A ) an m by k matrix,  op( B ) a k by n matrix and C an m by n matrix.
void sgemm_(char* TRANSA, char* TRANSB, int* M, int* N, int* K, float* ALPHA, float A[], int* LDA, float B[], int* LDB, float* BETA, float C[], int* LDC);
void dgemm_(char* TRANSA, char* TRANSB, int* M, int* N, int* K, double* ALPHA, double A[], int* LDA, double B[], int* LDB, double* BETA, double C[], int* LDC);


// C := alpha*A*B + beta*C,
//    or
// C := alpha*B*A + beta*C,
// where alpha and beta are scalars,  A is a symmetric matrix and  B and C are m by n matrices
void ssymm_(char* SIDE, char* UPLO, int* M, int* N, float* ALPHA, float A[], int* LDA, float B[], int* LDB, float* BETA, float C[], int* LDC);
void dsymm_(char* SIDE, char* UPLO, int* M, int* N, double* ALPHA, double A[], int* LDA, double B[], int* LDB, double* BETA, double C[], int* LDC);


// C := alpha*A*A**T + beta*C,
//    or
// C := alpha*A**T*A + beta*C,
// where  alpha and beta  are scalars, C is an  n by n  symmetric matrix and  A is an  n by k  matrix in the first case and a  k by n  matrix in the second case.
void ssyrk_(char* UPLO, char* TRANS, int* N, int* K, float* ALPHA, float A[], int* LDA, float* BETA, float C[], int* LDC);
void dsyrk_(char* UPLO, char* TRANS, int* N, int* K, double* ALPHA, double A[], int* LDA, double* BETA, double C[], int* LDC);


// C := alpha*A*B**T + alpha*B*A**T + beta*C,
//    or
// C := alpha*A**T*B + alpha*B**T*A + beta*C,
// where  alpha and beta  are scalars, C is an  n by n  symmetric matrix and  A and B is an  n by k  matrix in the first case and a  k by n  matrix in the second case.
void ssyr2k_(char* UPLO, char* TRANS, int* N, int* K, float* ALPHA, float A[], int* LDA, float B[], int* LDB, float* BETA, float C[], int* LDC);
void dsyr2k_(char* UPLO, char* TRANS, int* N, int* K, double* ALPHA, double A[], int* LDA, double B[], int* LDB, double* BETA, double C[], int* LDC);


//  B := alpha*op( A )*B,   or   B := alpha*B*op( A ),
//  where  alpha  is a scalar,  B  is an m by n matrix,  A  is a unit, or non-unit,  upper or lower triangular matrix  and  op( A )  is one of
//    op( A ) = A   or   op( A ) = A**T.
void strmm_(char* SIDE, char* UPLO, char* TRANSA, char* DIAG, int* M, int* N, float* ALPHA, float A[], int* LDA, float B[], int* LDB);
void dtrmm_(char* SIDE, char* UPLO, char* TRANSA, char* DIAG, int* M, int* N, double* ALPHA, double A[], int* LDA, double B[], int* LDB);


/*
*> STRSM  solves one of the matrix equations
*>
*>    op( A )*X = alpha*B,   or   X*op( A ) = alpha*B,
*>
*> where alpha is a scalar, X and B are m by n matrices, A is a unit, or
*> non-unit,  upper or lower triangular matrix  and  op( A )  is one  of
*>
*>    op( A ) = A   or   op( A ) = A**T.
*>
*> The matrix X is overwritten on B
 */
void strsm_(char* SIDE, char* UPLO, char* TRANSA, char* DIAG, int* M, int* N, float* ALPHA, float A[], int* LDA, float B[], int* LDB);
void dtrsm_(char* SIDE, char* UPLO, char* TRANSA, char* DIAG, int* M, int* N, double* ALPHA, double A[], int* LDA, double B[], int* LDB);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !EW_BLAS_H
