#ifndef BASIC_UTILS_H
#define BASIC_UTILS_H
#include <cxxabi.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "stdarg.h"
using namespace std;

namespace basic {

// 获取变量的类型名称
template <class T>
static inline string TypeName(const T& variable) {
  string name = typeid(variable).name();
  int status = 0;
  char* p = abi::__cxa_demangle(name.c_str(), 0, 0, &status);
  string result(p);
  delete p;
  return result;
}

// 连续值的范围 () 判断是否在范围内
template <class T>
struct RangeContious {
 public:
  RangeContious(const T& low, const T& high) : low(low), high(high) {}
  ~RangeContious() {}
  const bool operator()(T s) {
    if (!(s >= low && s <= high)) return false;
    return true;
  }

 private:
  T low, high;
};

// 离散值的范围 () 判断是否在范围内
template <class T>
struct RangeDiscrete {
  /*
  string 不能用, 只能用char*代替
  RangeDiscrete<char*> RD(2, (char*)"zhangsan", (char*)"zhangsi");
  RD.Add((char*)"zhangwu");
  RD.Add((char*)"zhangliu");
  cout << RD((char*)"zhangsan") << endl;;
  */
 public:
  RangeDiscrete() {}
  RangeDiscrete(set<T> a) { alt = a; }
  RangeDiscrete(int num, ...) {
    va_list t;
    va_start(t, num);
    for (int i = 0; i < num; i++) {
      alt.insert(va_arg(t, T));
    }
    va_end(t);
  }
  ~RangeDiscrete() {}
  const bool operator()(T s) {
    if (find(alt.begin(), alt.end(), s) == alt.end()) return false;
    return true;
  }
  void Add(const T& v) { alt.insert(v); }

 private:
  set<T> alt;
};

inline vector<string> SplitStr(string& str, string deli = ",") {
  vector<string> v;
  int n = 0, index = 0;
  int len_d = deli.size();
  int len_s = str.size();
  while (n != str.npos) {
    n = str.find(deli, index);
    if (n != str.npos) {
      string s_t = str.substr(index, n - index);
      index = n + len_d;
      v.push_back(s_t);
    }
  }
  v.push_back(str.substr(index));
  return v;
};

template <class T>
inline vector<T> SplitStr(string& str, string deli = ",") {
  vector<T> v;
  T v0;
  stringstream iss;
  int n = 0, index = 0;
  int len_d = deli.size();
  int len_s = str.size();
  while (n != str.npos) {
    n = str.find(deli, index);
    if (n != str.npos) {
      iss << str.substr(index, n - index);
      index = n + len_d;
      iss >> v0;
      v.push_back(v0);
      iss.clear();
      v0 = 0;
    }
  }
  iss << str.substr(index);
  iss >> v0;
  v.push_back(v0);
  iss.clear();
  v0 = 0;
  return v;
};

inline string MergeStr(vector<string>& strs, string deli = ",") {
  string str;
  int i = 0;
  for (auto it = strs.begin(); it != strs.end(); it++, i++) {
    if (i == 0) {
      str += *it;
    } else {
      str += (deli + *it);
    }
  }
  return str;
};

template <class T>
inline string MergeStr(vector<T>& strs, string deli = ",") {
  string str;
  int i = 0;
  for (auto it = strs.begin(); it != strs.end(); it++, i++) {
    if (i == 0) {
      str += to_string(*it);
    } else {
      str += (deli + to_string(*it));
    }
  }
  return str;
};

inline int GetCPUNum() { return sysconf(_SC_NPROCESSORS_CONF); }

inline int GetMemorySize() {
  return (long long)sysconf(_SC_PAGESIZE) * (long long)sysconf(_SC_PHYS_PAGES) /
         (1024 * 1024);
}
}  // namespace basic

#endif