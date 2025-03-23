#include <iostream>

#include "Utility/MD5.h"


int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "ew_md5sum: need an input file!" << std::endl;
    return -1;
  }
  std::string path = argv[1];
  // std::cout << path;

  std::cout
    << "ew_md5sum: "
    << EastWind::MD5::instance().compute_from_path(path)
    << std::endl;
  return 0;
}