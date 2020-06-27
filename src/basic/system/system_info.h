#pragma once
#include <cxxabi.h>
#include <unistd.h>

namespace system {
using namespace std;

inline int GetCPUNum() { return sysconf(_SC_NPROCESSORS_CONF); }

inline int GetMemorySize() {
  return (long long)sysconf(_SC_PAGESIZE) * (long long)sysconf(_SC_PHYS_PAGES) /
         (1024 * 1024);
}

}  // namespace system
