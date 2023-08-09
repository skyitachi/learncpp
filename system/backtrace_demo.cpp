//
// Created by Shiping Yao on 2023/8/9.
//
#include <regex>
#include <string>
#include <execinfo.h>
#include <cxxabi.h>
#include <iostream>

#include <modern/defer.h>

void f1();
void f2();
void f3();

std::regex symbol_pat("^([^(]*)\\((\\w+)\\+\\w+\\).*");
const size_t TOPMOST_STACK_FRAMES = 20;
__thread void* frame_array[TOPMOST_STACK_FRAMES]={0};

inline char const * demangle_alloc( char const * name ) {
  int status = 0;
  size_t size = 0;
  return abi::__cxa_demangle(name, NULL, &size, &status);
}

//inline std::string demangle( char const * name) {
//  std::string s(name);
//  size_t size = 0;
//  return abi::__cxa_demangle(name, NULL, &size, &status);
//}

inline std::string demangle( char const * name) {
  std::string s(name);
  char const *demangled=demangle_alloc(name);
  DEFER([&](){
    free(const_cast<char*>(demangled));
  });
  if (demangled != nullptr) {
    s = std::string(demangled);
  }
  return s;
}

std::ostream& bt(std::ostream &out, int level) {
  int n = backtrace(frame_array, TOPMOST_STACK_FRAMES);
  out << " [BACKTRACE]" << '\n';
  if (n < 1) {
    return out;
  }
  char **symbols = backtrace_symbols(frame_array, n);
  DEFER([&]() {
    free(symbols);
  });

  std::smatch groups;
  for (int i = level; i < n; ++i) {
    std::string sym = symbols[i];
    if (regex_match(sym, groups, symbol_pat)) {
      out << "#" << i << ": " << demangle(((std::string) groups[2]).c_str()) << '\t' << (std::string) groups[1] << '\n';
    } else {
      out << "#" << i << ": " << symbols[i] << '\n';
    }
  }
  return out;
}

void f1() {
 f2();
}

void f2() {
 f3();
}

void f3() {
  bt(std::cout, 0);
}

int main() {
  f1();
}
