rm -r build
# cmake -B build -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_Fortran_COMPILER=gfortran
./run.sh
