#include <cstdlib>
#include <iostream>
#include <EastWind_Math.h>


void Test_Int(){
  std::cout << "=================================================" << std::endl;
  std::cout << " >>> INT TEST BEGIN >>>" << std::endl;
  std::cout << "=================================================" << std::endl;
  std::cout << std::endl;

  
  std::cout << "******** Vec ********" << std::endl;
  std::cout << ">> Constructors <<" << std::endl;
  std::cout << "Default Constructor" << std::endl;
  EastWind::Vec<int, 0> v0;
  std::cout << "v0: " << v0 << std::endl;

  EastWind::Vec<int, 10> v1({1,2,3,4,5,6,7,8,9,0});
  std::cout << "v1: " << v1 << std::endl;
  std::cout << std::fixed;
  std::cout << "v1 norm-2: " << v1.norm(2) << std::endl; 
  std::cout << std::endl;


  std::cout << "******** Mat ********" << std::endl;
  std::cout << "Default Constructor" << std::endl;
  EastWind::Mat<int,3,3> E;
  std::cout << "E:" << E << std::endl;


  std::cout << "Array Constructor" << std::endl;
  EastWind::Mat<int,3,3> M1{
    {
      {1,2,3},
      {2,3,4},
      {3,4,5}
    }
  };
  std::cout << "M1:" << M1 << std::endl; 
  std::cout << "M1*M1=\n" << M1*M1 << std::endl;
  std::cout << std::endl;

  std::cout << "=================================================" << std::endl;
  std::cout << " <<< INT TEST END <<<" << std::endl;
  std::cout << "=================================================" << std::endl;
  std::cout << std::endl;
}

void Test_Float(){
  std::cout << std::fixed;
  std::cout << "=================================================" << std::endl;
  std::cout << " >>> FLOAT TEST BEGIN >>>" << std::endl;
  std::cout << "=================================================" << std::endl;
  std::cout << std::endl;

  
  std::cout << "******** Vec ********" << std::endl;

  std::cout << ">> Constructors <<" << std::endl;
  std::cout << "Default Constructor" << std::endl;
  EastWind::Vec<float, 3> v0;
  std::cout << "v0: " << v0 << std::endl;

  std::cout << "Single Element Constructor" << std::endl;
  EastWind::Vec<float, 3> v1(3.f);
  std::cout << "v1: " << v1 << std::endl;

  std::cout << "Array Constructor" << std::endl;
  EastWind::Vec<float, 3> v2({1.f,2.f,3.f});
  std::cout << "v2: " << v2 << std::endl;


  std::cout << "Copy Other Vec Constructor" << std::endl;
  EastWind::Vec<float, 3> v3(v2);
  std::cout << "v3: " << v3 << std::endl;

  std::cout << "Move Other Vec Constructor" << std::endl;
  EastWind::Vec<float, 3> v4(std::move(v1));
  std::cout << "v4: " << v4 << std::endl;


  std::cout << ">> Operators <<" << std::endl;
  std::cout << "v1 + v2: " << v1+v2 << std::endl;
  std::cout << "v1 + 1.5: " << v1+1.5f << std::endl;
  v1 += v2;
  std::cout << "v1 += v2: " << v1 << std::endl;
  v1+=1.5f;
  std::cout << "v1 += 1.5: " << v1 << std::endl;

  std::cout << "v1 - v2: " << v1+v2 << std::endl;
  std::cout << "v1 - 1.5: " << v1+1.5f << std::endl;
  v1 -= v2;
  std::cout << "v1 -= v2: " << v1 << std::endl;
  v1-=1.5f;
  std::cout << "v1 -= 1.5: " << v1 << std::endl;


  std::cout << "v1 * v2: " << v1*v2 << std::endl;

  std::cout << ">> Norm <<" << std::endl;
  std::cout << "v2 norm-1: " << v2.norm('1') << std::endl; 
  std::cout << "v2 norm-2: " << v2.norm('2') << std::endl; 
  std::cout << "v2 norm-inf: " << v2.norm('I') << std::endl; 
  std::cout << std::endl;


  std::cout << ">> Cross Product <<" << std::endl;
  EastWind::Vec<float,3> v1v2Cross = v1.cross(v2);
  std::cout << "v1 x v2: " << v1v2Cross << std::endl;
  std::cout << "v1 * v1v2Cross: " << v1*v1v2Cross << std::endl;
  std::cout << "v2 * v1v2Cross: " << v2*v1v2Cross << std::endl;
  
  // std::cout << ">> Swap <<" << std::endl;
  // std::cout << "Before Swap\n";
  // std::cout << "v1: " << v1 << std::endl;
  // std::cout << "v2: " << v2 << std::endl;
  // v1.swap(v2);
  // std::cout << "After Swap\n";
  // std::cout << "v1: " << v1 << std::endl;
  // std::cout << "v2: " << v2 << std::endl;

  std::cout << std::endl;
  std::cout << "******** Mat ********" << std::endl;

  std::cout << ">> Constructors <<" << std::endl;
  std::cout << "Default Constructor" << std::endl;
  EastWind::Mat<float,3,3> E;
  std::cout << "E:" << E << std::endl;

  std::cout << "Single Element Constructor" << std::endl;
  EastWind::Mat<float,3,3> S(2.f);
  std::cout << "S:" << S << std::endl;

  std::cout << "Array Constructor" << std::endl;
  EastWind::Mat<float,3,3> M1{
    {
      {1,2,3},
      {1,4,9},
      {1,8,27}
    }
  };
  std::cout << "M1:" << M1 << std::endl; 
  std::cout << "M1*M1=\n" << M1*M1 << std::endl;
  std::cout << std::endl;

  std::cout << "Vec -> Mat<T,m,1> Constructor" << std::endl;
  EastWind::Mat<float,3,1> C(v2);
  std::cout << "C:" << C << std::endl;

   std::cout << "Copy Other Mat Constructor" << std::endl;
  EastWind::Mat<float,3,3> M2(M1);
  std::cout << "M2: " << M2 << std::endl;

  std::cout << "Move Other Vec Constructor" << std::endl;
  EastWind::Mat<float,3,3> M3(std::move(M1));
  std::cout << "M3: " << M3 << std::endl; 


  std::cout << ">> Transpose <<" << std::endl;
  EastWind::Mat<float,1,3> CT = C.Trans();
  std::cout << "CT:" << CT << std::endl;

  std::cout << ">> Resize <<" << std::endl;
  EastWind::Mat<float,1,9> M2Resize = M2.resize<1,9>();
  std::cout << "M2Resize:" << M2Resize << std::endl;

  std::cout << ">> Operators <<" << std::endl;
  std::cout << "M1*M1 + M2: " << M1*M1+M2 << std::endl;
  M1 += M2*(M1+M1);
  std::cout << "M1 += M2*(M1+M1): " << M1 << std::endl;
  std::cout << "M1 - M2: " << M1-M2 << std::endl;
  M1 -= M2;
  std::cout << "M1 -= M2: " << M1 << std::endl;

  std::cout << "M1 * v2: " << M1*v2 << std::endl;

  std::cout << ">> Norm <<" << std::endl;
  std::cout << "M1 norm Max: " << M1.norm('M') << std::endl; 
  std::cout << "M1 norm-1: " << M1.norm('1') << std::endl; 
  std::cout << "M1 norm-Inf: " << M1.norm('I') << std::endl; 
  std::cout << "M1 norm-Frobenius: " << M1.norm('F') << std::endl; 
  std::cout << std::endl;

  // std::cout << ">> Swap <<" << std::endl;
  // std::cout << "Before Swap\n";
  // std::cout << "M1: " << M1 << std::endl;
  // std::cout << "M2: " << M2 << std::endl;
  // M1.swap(M2);
  // std::cout << "After Swap\n";
  // std::cout << "M1: " << M1 << std::endl;
  // std::cout << "M2: " << M2 << std::endl;

  std::cout << "=================================================" << std::endl;
  std::cout << " <<< FLOAT TEST END <<<" << std::endl;
  std::cout << "=================================================" << std::endl;
  std::cout << std::endl;
}

void Test_Double(){
  std::cout << std::fixed;
  std::cout << "=================================================" << std::endl;
  std::cout << " >>> DOUBLE TEST BEGIN >>>" << std::endl;
  std::cout << "=================================================" << std::endl;
  std::cout << std::endl;

  
  std::cout << "******** Vec ********" << std::endl;

  std::cout << ">> Constructors <<" << std::endl;
  std::cout << "Default Constructor" << std::endl;
  EastWind::Vec<double, 3> v0;
  std::cout << "v0: " << v0 << std::endl;

  std::cout << "Single Element Constructor" << std::endl;
  EastWind::Vec<double, 3> v1(3.f);
  std::cout << "v1: " << v1 << std::endl;

  std::cout << "Array Constructor" << std::endl;
  EastWind::Vec<double, 3> v2({1.f,2.f,3.f});
  std::cout << "v2: " << v2 << std::endl;


  std::cout << "Copy Other Vec Constructor" << std::endl;
  EastWind::Vec<double, 3> v3(v2);
  std::cout << "v3: " << v3 << std::endl;

  std::cout << "Move Other Vec Constructor" << std::endl;
  EastWind::Vec<double, 3> v4(std::move(v1));
  std::cout << "v4: " << v4 << std::endl;


  std::cout << ">> Operators <<" << std::endl;
  std::cout << "v1 + v2: " << v1+v2 << std::endl;
  std::cout << "v1 + 1.5: " << v1+1.5f << std::endl;
  v1 += v2;
  std::cout << "v1 += v2: " << v1 << std::endl;
  v1+=1.5f;
  std::cout << "v1 += 1.5: " << v1 << std::endl;

  std::cout << "v1 - v2: " << v1+v2 << std::endl;
  std::cout << "v1 - 1.5: " << v1+1.5f << std::endl;
  v1 -= v2;
  std::cout << "v1 -= v2: " << v1 << std::endl;
  v1-=1.5f;
  std::cout << "v1 -= 1.5: " << v1 << std::endl;


  std::cout << "v1 * v2: " << v1*v2 << std::endl;

  std::cout << ">> Norm <<" << std::endl;
  std::cout << "v2 norm-1: " << v2.norm('1') << std::endl; 
  std::cout << "v2 norm-2: " << v2.norm('2') << std::endl; 
  std::cout << "v2 norm-inf: " << v2.norm('I') << std::endl; 
  std::cout << std::endl;


  std::cout << ">> Cross Product <<" << std::endl;
  EastWind::Vec<double,3> v1v2Cross = v1.cross(v2);
  std::cout << "v1 x v2: " << v1v2Cross << std::endl;
  std::cout << "v1 * v1v2Cross: " << v1*v1v2Cross << std::endl;
  std::cout << "v2 * v1v2Cross: " << v2*v1v2Cross << std::endl;
  
  // std::cout << ">> Swap <<" << std::endl;
  // std::cout << "Before Swap\n";
  // std::cout << "v1: " << v1 << std::endl;
  // std::cout << "v2: " << v2 << std::endl;
  // v1.swap(v2);
  // std::cout << "After Swap\n";
  // std::cout << "v1: " << v1 << std::endl;
  // std::cout << "v2: " << v2 << std::endl;

  std::cout << std::endl;
  std::cout << "******** Mat ********" << std::endl;

  std::cout << ">> Constructors <<" << std::endl;
  std::cout << "Default Constructor" << std::endl;
  EastWind::Mat<double,3,3> E;
  std::cout << "E:" << E << std::endl;

  std::cout << "Single Element Constructor" << std::endl;
  EastWind::Mat<double,3,3> S(2.f);
  std::cout << "S:" << S << std::endl;

  std::cout << "Array Constructor" << std::endl;
  EastWind::Mat<double,3,3> M1{
    {
      {1,2,3},
      {1,4,9},
      {1,8,27}
    }
  };
  std::cout << "M1:" << M1 << std::endl; 
  std::cout << "M1*M1=\n" << M1*M1 << std::endl;
  std::cout << std::endl;

  std::cout << "Vec -> Mat<T,m,1> Constructor" << std::endl;
  EastWind::Mat<double,3,1> C(v2);
  std::cout << "C:" << C << std::endl;

   std::cout << "Copy Other Mat Constructor" << std::endl;
  EastWind::Mat<double,3,3> M2(M1);
  std::cout << "M2: " << M2 << std::endl;

  std::cout << "Move Other Vec Constructor" << std::endl;
  EastWind::Mat<double,3,3> M3(std::move(M1));
  std::cout << "M3: " << M3 << std::endl; 


  std::cout << ">> Transpose <<" << std::endl;
  EastWind::Mat<double,1,3> CT = C.Trans();
  std::cout << "CT:" << CT << std::endl;

  std::cout << ">> Resize <<" << std::endl;
  EastWind::Mat<double,1,9> M2Resize = M2.resize<1,9>();
  std::cout << "M2Resize:" << M2Resize << std::endl;

  std::cout << ">> Operators <<" << std::endl;
  std::cout << "M1*M1 + M2: " << M1*M1+M2 << std::endl;
  M1 += M2*(M1+M1);
  std::cout << "M1 += M2*(M1+M1): " << M1 << std::endl;
  std::cout << "M1 - M2: " << M1-M2 << std::endl;
  M1 -= M2;
  std::cout << "M1 -= M2: " << M1 << std::endl;

  std::cout << "M1 * v2: " << M1*v2 << std::endl;

  std::cout << ">> Norm <<" << std::endl;
  std::cout << "M1 norm Max: " << M1.norm('M') << std::endl; 
  std::cout << "M1 norm-1: " << M1.norm('1') << std::endl; 
  std::cout << "M1 norm-Inf: " << M1.norm('I') << std::endl; 
  std::cout << "M1 norm-Frobenius: " << M1.norm('F') << std::endl; 
  std::cout << std::endl;

  // std::cout << ">> Swap <<" << std::endl;
  // std::cout << "Before Swap\n";
  // std::cout << "M1: " << M1 << std::endl;
  // std::cout << "M2: " << M2 << std::endl;
  // M1.swap(M2);
  // std::cout << "After Swap\n";
  // std::cout << "M1: " << M1 << std::endl;
  // std::cout << "M2: " << M2 << std::endl;

  std::cout << "=================================================" << std::endl;
  std::cout << " <<< DOUBLE TEST END <<<" << std::endl;
  std::cout << "=================================================" << std::endl;
  std::cout << std::endl;
}


int main(){
  // Test_Int();
  Test_Float();
  Test_Double();
};


// deprecated
// int main(){
//   Test_Int();
//
//   EastWind::Vec<float, 10> v1({1.8f,2.1f,3.3f,4.f,5.f,6.f,7.f,8.f,9.f,10.f}), 
//     v2(1.f), 
//     v3;
//   EastWind::Vec<double, 10> v4({1.1,2.8,3.,4.,5.,6.,7.,8.,9.,10.}), 
//     v5(1.), 
//     v6;
//   // std::cout << std::fixed;
//
//   std::cout << "v1: " << v1 << std::endl; 
//   std::cout << "v2: " << v2 << std::endl; 
//   std::cout << "v3: " << v3 << std::endl; 
//
//
//   std::cout << "v1+v2\n" << (v3 = v1 + v2) << std::endl;
//   std::cout << "v1-v2\n" << (v3 = v1 - v2) << std::endl;
//
//   v1 = EastWind::Vec<float, 10>({1.8f,2.1f,3.3f,4.f,5.f,6.f,7.f,8.f,9.f,10.f}); 
//   v1 += v2;
//   std::cout << "v1+=v2\n" << v1 << std::endl;
//
//   v1 = EastWind::Vec<float, 10>({1.8f,2.1f,3.3f,4.f,5.f,6.f,7.f,8.f,9.f,10.f}); 
//   v1 -= v2;
//   std::cout << "v1-=v2\n" << v1 << std::endl;
//
//   v1 = EastWind::Vec<float, 10>({1.8f,2.1f,3.3f,4.f,5.f,6.f,7.f,8.f,9.f,10.f}); 
//   std::cout << "v1·v2 = " << v1.dot(v2) << std::endl;
//   std::cout << "v4·v5 = " << v4*v5 << std::endl;
//
//   v1.swap(v2);
//   std::cout << "Swap: v1 <-> v2 " << std::endl;
//   std::cout << "v1:" << v1 << std::endl;
//   std::cout << "v2:" << v2 << std::endl;
//
//   
//   EastWind::Vec<float, 4> u1({1.f,0.f,0.f,0.f}), u2({0.f,1.f,0.f,0.f}), u3;
//   std::cout << "u1:" << u1 << std::endl;
//   std::cout << "u2:" << u2 << std::endl;
//   std::cout << "u3 = u1 x u2" << std::endl
//             << (u3 = u1.cross(u2)) << std::endl;
//
//
//   std::cout << "====================================" << std::endl;
//
//   EastWind::Mat<float, 3, 5> mat1, mat2(2), mat3(v2);
//   std::cout << "mat1:" << mat1 << std::endl;
//   std::cout << "mat2:" << mat2 << std::endl;
//   std::cout << "mat3:" << mat3 << std::endl;
//
//   std::cout << "mat3^T:" << mat3.Trans() << std::endl;
//
//   std::cout << "mat3(1,1):" << mat3(1,1) << std::endl;
//   // std::cout << "mat3[1][1]:" << mat3[1][1] << std::endl;
//   std::cout << std::endl;
//
//   std::cout << "mat1+mat2 = " << std::endl << mat1+mat2 << std::endl;
//   std::cout << "mat2+mat3 = " << std::endl << mat3+mat2 << std::endl;
//
//   EastWind::Vec<float, 5> v({7,8,9,1,2});
//   std::cout << "v:" << v << std::endl;
//   std::cout << "mat1·v = " << std::endl;
//   std::cout << mat1 * v << std::endl;
//
//   std::cout << "mat1·mat2 = " << std::endl;
//   std::cout << mat1*(mat2.Trans()) << std::endl;
//
//   std::cout << "mat2·mat3 = " << std::endl;
//   std::cout << mat2*(mat3.Trans()) << std::endl;
//
//   std::cout << "====================================" << std::endl;
//   EastWind::Mat<int, 15, 15> largeMat;
//   largeMat(1,5) = 15;
//   std::cout << "Large Matrix: " << largeMat << std::endl;
//   std::cout << "largeMat is triangular: " << (largeMat.is_tri()? "true": "false") << std::endl;
//   std::cout << "largeMat is symmetric: " << (largeMat.is_sym()? "true": "false") << std::endl;
//
//   std::cout << "largeMat·largeMat = " << std::endl;
//   std::cout << largeMat*largeMat << std::endl;
//
//   std::cout << "====================================" << std::endl;
//   auto random_float = [](){return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);};
//   EastWind::Mat<float, 5, 5> randMat(
//     {
//       {random_float(), random_float(), random_float(), random_float(), random_float()},
//       {random_float(), random_float(), random_float(), random_float(), random_float()},
//       {random_float(), random_float(), random_float(), random_float(), random_float()},
//       {random_float(), random_float(), random_float(), random_float(), random_float()},
//       {random_float(), random_float(), random_float(), random_float(), random_float()},
//     }
//   );
//   std::cout << "randMat:\n" << randMat << std::endl; 
//   std::cout << "randMat·randMat = " << std::endl;
//   std::cout << randMat*randMat << std::endl;
//
//   std::cout << "randMat^(-1) = " << std::endl;
//   std::cout << randMat.Inverse() << std::endl;
//
//   
//   EastWind::Mat<float, 3, 3> singularMat(
//     {
//       {1,2,3},
//       {2,3,4},
//       {4,5,6}
//     }
//   );
//   std::cout << "singularMat:\n" << singularMat << std::endl; 
//   std::cout << "singularMat^(-1) = " << std::endl;
//   std::cout << singularMat.Inverse() << std::endl;
//
//   // Test static assert
//   // EastWind::Mat<float, 3, 4> mm;
//   // mm.Inverse();
// }
