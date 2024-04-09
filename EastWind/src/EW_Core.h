#ifndef EW_CORE_H
#define EW_CORE_H

#include <memory>
#ifdef EW_PLATFORM_WINDOWS

  #ifdef EW_BUILD_DLL
    #define EW_DLL __declspec(dllexport)
  #else
    #define EW_DLL __declspec(dllimport)
  #endif

#endif

#ifdef EW_ENABLE_ASSERTS
  #define EW_ASSERT(x, ...) {if (!(x)) {EW_ERROR("Assertion Failed"__VA_ARGS__); __builtin_debugtrap();}}
#else
  #define EW_ASSERT(x, ...)
#endif

#define BIT(x) (1<<x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace EastWind {
  template<typename T>
  using Scope = std::unique_ptr<T>;

  template<typename T>
  using Ref = std::shared_ptr<T>;
}


#endif // !EW_CORE_H
