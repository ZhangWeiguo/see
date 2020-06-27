#include "string_split.h"

namespace basic {
using namespace std;

bool SplitStr(const string &str, string &deli, vector<string> *strs) {
  if (strs == NULL) return false;
  int n = 0, index = 0;
  int len_d = deli.size();
  int len_s = str.size();
  while (n != str.npos) {
    n = str.find(deli, index);
    if (n != str.npos) {
      string s_t = str.substr(index, n - index);
      index = n + len_d;
      strs->push_back(s_t);
    }
  }
  strs->push_back(str.substr(index));
  return true;
};

bool MergeStr(const vector<string> &strs, string deli, string *str) {
  if (str == NULL) return false;
  int i = 0;
  for (auto it = strs.begin(); it != strs.end(); it++, i++) {
    if (i == 0) {
      (*str) += *it;
    } else {
      (*str) += (deli + *it);
    }
  }
  return true;
};


} // namespace basic