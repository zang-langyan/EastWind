#ifndef EW_MD5_H
#define EW_MD5_H
#include <string.h>

namespace EastWind {

class MD5 {
public:
  static MD5& instance() {
    static MD5 ins;
    return ins;
  }

  MD5() : buffer_size(1 << 20) {
    buffer = new char[buffer_size];
  }

  ~MD5() {
    delete[] buffer;
  }

  std::string compute_from_path(const std::string& path);
  std::string compute_from_string(const std::string& raw_string);

private:
  struct MD5Context {
    uint32_t buf[4];
    uint32_t bits[2];
    unsigned char in[64];
  };
  MD5Context ctx;
  unsigned char digest[16]; // output of md5 algorithm

  void initialize();
  void update(const unsigned char* data, size_t len);
  void finalize();

  std::string tobase16();
private:
  char* buffer;
  size_t buffer_size;
};

}
#endif
