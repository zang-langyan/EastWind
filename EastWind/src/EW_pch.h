#ifndef EW_PCH_H
#define EW_PCH_H

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <cctype>
#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <thread>
#include <mutex>

#ifdef EW_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

inline void ltrim(std::string& s){
  s.erase(
      s.begin(), 
      std::find_if(s.begin(), s.end(), [](unsigned char ch){
        return !std::isspace(ch);
      })
  );
}

inline void rtrim(std::string& s){
  s.erase(
      std::find_if(s.rbegin(), s.rend(), [](unsigned char ch){
        return !std::isspace(ch);
      }).base(),
      s.end()
  );
}

inline void trim(std::string& s){
  ltrim(s);
  rtrim(s);
}

#endif // !EW_PCH_H
