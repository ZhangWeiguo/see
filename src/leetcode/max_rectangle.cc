#include <iostream>
#include <vector>
using namespace std;

vector<int> common(vector<int> a, vector<int> b) {
  vector<int> c;
  int i, j;
  int j0 = 0;
  for (i = 0; i < a.size(); i++) {
    for (j = j0; j < b.size(); j++) {
      if (a[i] == b[j]) {
        c.push_back(a[i]);
        j0 = j + 1;
        break;
      }
    }
  }
  return c;
}

int continous(vector<int> a) {
  int k = 0;
  int n = 0;
  for (int i = 0; i < a.size(); i++) {
    if (i == 0) {
      k = 1;
      n = 1;
    } else {
      if (a[i] - a[i - 1] == 1) {
        k++;
        if (k > n) n = k;
      } else {
        if (k > n) {
          n = k;
        }
        k = 1;
      }
    }
  }
  return n;
}

class Solution {
 public:
  // Origin Doing
  int maximalRectangleOrigin(vector<vector<char>>& v) {
    vector<vector<int>> v_t;
    vector<int> v_t0;
    vector<vector<int>> v_t1;
    int N = 0;
    int M;
    for (int it = 0; it < v.size(); it++) {
      v_t0.clear();
      for (int jt = 0; jt < v[it].size(); jt++) {
        if (v[it][jt] == '1') v_t0.push_back(jt);
      }
      v_t.push_back(v_t0);
    }
    v_t0.clear();
    for (int i = 0; i < v.size(); i++) {
      int k = v_t0.size();
      int M = 1 * continous(v_t[i]);
      // cout << "Before:" << M << ";" << i << ";" << v_t[i].size() << endl;
      if (M > N) N = M;
      v_t1.push_back(v_t[i]);
      v_t0.push_back(1);
      for (int j = k - i; j < k; j++) {
        v_t1.push_back(common(v_t1[j], v_t[i]));
        v_t0.push_back(v_t0[j] + 1);
        M = continous(v_t1[v_t1.size() - 1]) * (v_t0[j] + 1);
        if (M > N) N = M;
        // cout << "Mid:" << M << ";" << i << ";" << j << ";" << v_t0[i] <<
        // endl;
      }
    }
    return N;
  }
  // Advanced by Largest Rectangle in Histogram
  int maximalRectangleHistogram(vector<vector<char>>& v) {
    int N = 0, n = 0, m = 0;
    if (v.size() > 0) {
      m = v[0].size();
    } else {
      return 0;
    }
    int v_t[m] = {};
    for (int i = 0; i < v.size(); i++) {
      for (int j = 0; j < v[i].size(); j++) {
        if (i == 0) {
          if (v[0][j] == '1') {
            v_t[j] = 1;
          } else {
            v_t[j] = 0;
          }
        } else {
          if (v[i][j] == '1') {
            v_t[j] += 1;
          } else {
            v_t[j] = 0;
          }
        }
        n = v_t[j];
        if (n == 0) continue;
        for (int k = j; k >= 0; k--) {
          n = min(n, v_t[k]);
          if (n == 0) continue;
          N = max(N, (j + 1 - k) * n);
        }
      }
    }
    return N;
  }
  int maximalRectangleDP(vector<vector<char>>& v) {
    int N = 0, n = 0, m = 0;
    if (v.size() > 0) {
      n = v[0].size();
    } else {
      return 0;
    }
    int left[n] = {}, right[n] = {}, height[n] = {};
    fill_n(right, n, n);
    int curr_left, curr_right;
    for (int i = 0; i < v.size(); i++) {
      curr_left = 0;
      curr_right = n;
      for (int j = 0; j < v[i].size(); j++) {
        if (v[i][j] == '1') {
          height[j] += 1;
          left[j] = max(left[j], curr_left);
        } else {
          height[j] = 0;
          left[j] = 0;
          curr_left = j + 1;
        }
      }
      for (int j = n - 1; j >= 0; j--) {
        if (v[i][j] == '1') {
          right[j] = min(right[j], curr_right);
        } else {
          right[j] = n;
          curr_right = j;
        }
      }
      for (int j = 0; j < n; j++) {
        N = max(N, (right[j] - left[j]) * height[j]);
      }
    }
    return N;
  }
};

int main() {
  Solution S;
  vector<vector<char>> v;
  vector<char> v0;
  char v00;
  int n = 0, m = 0;
  while (!cin.eof()) {
    cin.get(v00);
    if (v00 == '\n') {
      if (v0.size() > 0) v.push_back(v0);
      n++;
      m = 0;
      v0.clear();
    } else if (v00 >= '0' and v00 <= '1') {
      v0.push_back(v00);
      m++;
    }
  }
  cout << S.maximalRectangleHistogram(v) << endl;
  return 0;
}
