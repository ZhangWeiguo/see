#include<vector>
#include<string>
#include<iostream>
using namespace std;

// KMP 子串查找算法
// 在 s 中查找是否存在子串 p
int kmp(string s_parent, string s_child) {
  if (s_parent.size() < s_child.size()) return -1;
  int next[s_child.size()];
  int begin = 0, value = -1;
  next[begin] = value;
  while(begin < s_child.size()) {
    if (value == -1 || s_child[begin] == s_child[value]) {
      next[++begin] = ++value;
    } else {
      value = next[value];
    }
  }
  int bp = 0, bc = 0;
  while (bp < (int)s_parent.size() && bc < (int)s_child.size()) {
    if (bc == -1 || s_parent[bp] == s_child[bc]) {
      bp++;
      bc++;
    } else {
      bc = next[bc];
    }

  }
  if (bc == s_child.size()) return bp - bc;
  return -1;
}

int kmp_viloance(string s_parent, string s_child) {
  if (s_parent.size() < s_child.size()) return -1;
  for (int i = 0; i < s_parent.size(); i++) {
    bool find = true;
    for (int j = 0; j < s_child.size(); j++) {
      if (s_parent[i+j] != s_child[j]) {
        find = false;
        break;
      }
    }
    if (find) return i;
  }
  return -1;
}


int main() {
  string parent("tetesygottestttg"), child("test");
  cout << kmp(parent, child) << endl;
  return 0;
}