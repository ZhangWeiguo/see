#include<functional>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
using namespace std;


template <class T>
void print(const vector<T> &L) {
  for (auto x: L){
    cout << x << ";";
  }
  cout << endl;
};


// 大数乘法
string multiply(string s1, string s2) {
  std::vector<int> s;
  size_t L1 = s1.size(), L2 = s2.size(), L = L1 + L2;
  s.resize(L, 0);
  print<int>(s);
  for (int i = 0; i < L1; i++) {
    for (int j = 0; j < L2; j++) {
      size_t i_index = L1 - i - 1, j_index = L2 - j - 1;
      int ai = s1[i_index] - 48;
      int aj = s2[j_index] - 48;
      int aij = ai * aj;
      size_t index = (1 + i_index) + (1 + j_index) - 1;
      while (aij > 0) {
        s[index--] += aij % 10;
        aij /= 10;
        if (s[index+1] >= 10) {
          aij += s[index+1] / 10;
          s[index+1]  = s[index+1] % 10;
        }
      }
    }
  }
  string ss;
  bool begin = false;
  for (int i = 0; i < L; i++) {
    if (s[i] > 0 && !begin) {
      begin = true;
    }
    if (begin) {
      char x = s[i] + 48;
      ss.push_back(x);
    }
  }
  return ss;
}