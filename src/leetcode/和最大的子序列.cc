#include<vector>
using namespace std;


// 寻找 T 中和最大的子序列 T.size() >= 1
template<class T>
T maximum_subarray(vector<T> L, int &begin, int &end) {
  begin = 100000;
  if (L.size() == 0) return -1;
  T ma = L[0], l_ma = L[0];
  begin = 0;
  end = 0;
  int tmp_begin = 0, tmp_end = 0;
  if (L.size() == 1) return ma;
  for (int i = 1; i < L.size(); i++) {
    if (ma + L[i] > L[i]) {
      ma += L[i];
      tmp_end = i;
    } else {
      ma = L[i];
      tmp_begin = i;
      tmp_end = i;
    }
    if (ma > l_ma) {
      l_ma = ma;
      begin = tmp_begin;
      end = tmp_end;
    }
  }
  return l_ma;
}
