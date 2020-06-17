#pragma once
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <vector>

#include "algorithm"
#include "math.h"
#include "omp.h"
#include "stdio.h"
#include "time.h"
#include "utils.h"
using namespace std;

namespace see {
typedef vector<vector<double>> DoubleMat;
typedef vector<vector<int>> IntMat;

template <class T>
vector<vector<T>> ZerosMat(int m, int n) {
  vector<vector<T>> v(m, vector<T>(n, 0));
  return v;
}

template <class T>
vector<vector<T>> OnesMat(int m, int n) {
  vector<vector<T>> v(m, vector<T>(n, 1));
  return v;
}

template <class T>
vector<vector<T>> RandomMat(int m, int n, T min_value = 0, T max_value = 1) {
  vector<vector<T>> v;
  default_random_engine e;
  uniform_real_distribution<double> uniform_random(min_value, max_value);
  time_t now;
  time(&now);
  e.seed(now);
  for (int i = 0; i < m; i++) {
    vector<T> v1 = vector<T>(n, 1);
    for (int j = 0; j < n; j++) {
      v1[j] = (T)(uniform_random(e));
    }
    v.push_back(v1);
  }
  return v;
}

template <class T>
vector<vector<T>> EyesMat(int m) {
  vector<vector<T>> v = ZerosMat<T>(m, m);
  for (int i = 0; i < m; i++) v[i][i] = 1;
  return v;
}

template <class T>  // m = apply(a)
void MatApply(vector<vector<T>>& m, T fun(T a)) {
  for (auto i = m.begin(); i != m.end(); i++) {
    for (auto j = i->begin(); j != i->end(); j++) {
      *j = fun(*j);
    }
  }
}

template <class T>
void MatSum(vector<vector<T>>& m, vector<vector<T>>& n, bool x) {
  m.clear();
  if (x) {
    if (n.size() == 0) return;
    m = vector<T>(0, n[0].size());
    for (int i = 0; i < n[0].size(); i++) {
      m[i] = 0;
      for (int j = 0; j < n.size(); j++) {
        m[i] += n[j][i];
      }
    }
  } else {
    m = vector<T>(0, n.size());
    for (int i = 0; i < n.size(); i++) {
      m[i] = 0;
      for (int j = 0; j < n[i].size(); j++) {
        m[i] += n[i][j];
      }
    }
  }
}

template <class T>
void MatSum(T& m, vector<vector<T>>& n) {
  m = 0;
  for (int i = 0; i < n.size(); i++) {
    for (int j = i + 1; j < n[i].size(); j++) {
      m += n[i][j];
    }
  }
}

template <class T>
void MatAvg(vector<T>& m, vector<vector<T>>& n, bool x) {
  m.clear();
  int k;
  if (x) {
    if (n.size() == 0) return;
    m = vector<T>(n[0].size(), 0);
    for (int i = 0; i < n[0].size(); i++) {
      m[i] = 0;
      k = 0;
      for (int j = 0; j < n.size(); j++) {
        m[i] += n[j][i];
        k++;
      }
      m[i] = m[i] / k;
    }
  } else {
    m = vector<T>(n.size(), 0);
    for (int i = 0; i < n.size(); i++) {
      m[i] = 0;
      k = 0;
      for (int j = 0; j < n[i].size(); j++) {
        m[i] += n[i][j];
        k++;
      }
      m[i] = m[i] / k;
    }
  }
}

template <class T>
void MatAvg(T& m, vector<vector<T>>& n) {
  m = 0;
  int k = 0;
  for (int i = 0; i < n.size(); i++) {
    for (int j = i + 1; j < n[i].size(); j++) {
      m += n[i][j];
      k++;
    }
  }
  m = m / k;
}

template <class T>  // m = m + a
void MatAdd(vector<vector<T>>& m, double a) {
  for (auto i = m.begin(); i != m.end(); i++) {
    for (auto j = i->begin(); j != i->end(); j++) {
      *j += a;
    }
  }
}

template <class T>  // m = m + n
void MatAdd(vector<vector<T>>& m, vector<vector<T>>& n) {
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[i].size(); j++) {
      m[i][j] += n[i][j];
    }
  }
}

template <class T>  // n = m1 + m2
void MatAdd(vector<vector<T>>& n, vector<vector<T>>& m1,
            vector<vector<T>>& m2) {
  for (int i = 0; i < m1.size(); i++) {
    for (int j = 0; j < m1[i].size(); j++) {
      n[i][j] = m1[i][j] + m2[i][j];
    }
  }
}

template <class T>  // n = m1 + m2
void MatAdd(vector<vector<T>>& n, vector<vector<T>>& m1, vector<T>& m2,
            bool x) {
  for (int i = 0; i < m1.size(); i++) {
    for (int j = 0; j < m1[i].size(); j++) {
      if (x)
        n[i][j] = m1[i][j] + m2[j];
      else
        n[i][j] = m1[i][j] + m2[i];
    }
  }
}

template <class T>  // n = m + a
void MatAdd(vector<vector<T>>& n, vector<vector<T>>& m, double a) {
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[i].size(); j++) {
      n[i][j] = m[i][j] + a;
    }
  }
}

template <class T>  // n = w0 + w1 * m1
void MatAdd(vector<vector<T>>& n, vector<vector<T>>& m1, T w0, T w1) {
  for (int i = 0; i < m1.size(); i++) {
    for (int j = 0; j < m1[i].size(); j++) {
      n[i][j] = w0 + w1 * m1[i][j];
    }
  }
}

template <class T>  // n = w0 + w1 * m1 + w2 * m2
void MatAdd(vector<vector<T>>& n, vector<vector<T>>& m1, vector<vector<T>>& m2,
            T w0, T w1, T w2) {
  for (int i = 0; i < m1.size(); i++) {
    for (int j = 0; j < m1[i].size(); j++) {
      n[i][j] = w0 + w1 * m1[i][j] + w2 * m2[i][j];
    }
  }
}

template <class T>  // n = w0 + w1 * m1 + w2 * m2 + w3 * m3
void MatAdd(vector<vector<T>>& n, vector<vector<T>>& m1, vector<vector<T>>& m2,
            vector<vector<T>>& m3, T w0, T w1, T w2, T w3) {
  for (int i = 0; i < m1.size(); i++) {
    for (int j = 0; j < m1[i].size(); j++) {
      n[i][j] = w0 + w1 * m1[i][j] + w2 * m2[i][j] + w3 * m3[i][j];
    }
  }
}

template <class T>  // n = w0 + w1 * m1 + w2 * m2 + w3 * m3 + w4 * m4
void MatAdd(vector<vector<T>>& n, vector<vector<T>>& m1, vector<vector<T>>& m2,
            vector<vector<T>>& m3, vector<vector<T>>& m4, T w0, T w1, T w2,
            T w3, T w4) {
  for (int i = 0; i < m1.size(); i++) {
    for (int j = 0; j < m1[i].size(); j++) {
      n[i][j] =
          w0 + w1 * m1[i][j] + w2 * m2[i][j] + w3 * m3[i][j] + w4 * m4[i][j];
    }
  }
}

template <class T>  // n = w0 + w1 * m1 + w2 * m2 + w3 * m3 + w4 * m4 + w5 * m5
void MatAdd(vector<vector<T>>& n, vector<vector<T>>& m1, vector<vector<T>>& m2,
            vector<vector<T>>& m3, vector<vector<T>>& m4, vector<vector<T>>& m5,
            T w0, T w1, T w2, T w3, T w4, T w5) {
  for (int i = 0; i < m1.size(); i++) {
    for (int j = 0; j < m1[i].size(); j++) {
      n[i][j] = w0 + w1 * m1[i][j] + w2 * m2[i][j] + w3 * m3[i][j] +
                w4 * m4[i][j] + w5 * m5[i][j];
    }
  }
}

template <class T>  // m = m * a
void MatMut(vector<vector<T>>& m, double a) {
  for (auto i = m.begin(); i != m.end(); i++) {
    for (auto j = i->begin(); j != i->end(); j++) {
      *j = (*j) * a;
    }
  }
}

template <class T>  // n = m * a
void MatMut(vector<vector<T>>& n, vector<vector<T>>& m, double a) {
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[i].size(); j++) {
      n[i][j] = m[i][j] * a;
    }
  }
}

template <class T>  // m = m * n
void MatMut(vector<vector<T>>& m, vector<vector<T>>& n) {
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[i].size(); j++) {
      m[i][j] = m[i][j] * n[i][j];
    }
  }
}

template <class T>  // n = m1 * m2
void MatMut(vector<vector<T>>& n, vector<vector<T>>& m1,
            vector<vector<T>>& m2) {
  for (int i = 0; i < m1.size(); i++) {
    for (int j = 0; j < m1[i].size(); j++) {
      n[i][j] = m1[i][j] * m2[i][j];
    }
  }
}

template <class T>  // n = m1 dot m2
void MatDot(vector<vector<T>>& n, vector<vector<T>>& m1,
            vector<vector<T>>& m2) {
  for (int i = 0; i < n.size(); i++) {
    for (int j = 0; j < n[i].size(); j++) {
      n[i][j] = 0;
      for (int p = 0; p < m1[i].size(); p++) {
        n[i][j] += m1[i][p] * m2[p][j];
      }
    }
  }
}

template <class T>  // n = m1 T
void MatTranspose(vector<vector<T>>& n, vector<vector<T>>& m) {
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[i].size(); j++) {
      n[j][i] = m[i][j];
    }
  }
}

template <class T>
void MatPrint(vector<vector<T>>& m, int p = 10, int q = 10) {
  int p0 = 0, q0 = 0;
  for (auto i = m.begin(); i != m.end(); i++) {
    if (p0++ >= p) break;
    q0 = 0;
    for (auto j = i->begin(); j != i->end(); j++) {
      if (q0++ >= q) break;
      cout << left << setw(10) << *j << " ";
    }
    cout << endl;
  }
}

// 有时候看着丑的东东往往效率高, 越直接越经得起推敲, 不建议使用 initializer_list
template <class T>  // m = ns x ws
void MatApply(vector<vector<T>>& m, initializer_list<vector<vector<T>>>& ns,
              initializer_list<T> ws) {
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[i].size(); j++) {
      m[i][j] = 0;
      int k = 0;
      while (k < ws.size()) {
        m[i][j] += (*(ns.begin() + k))[i][j] * *(ws.begin() + k);
        k++;
      }
    }
  }
}

template <class T>  // m = ns x ws + m * w
void MatApply(vector<vector<T>>& m, initializer_list<vector<vector<T>>>& ns,
              initializer_list<T> ws, T w) {
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[i].size(); j++) {
      int k = 0;
      m[i][j] = m[i][j] * w;
      while (k < ws.size()) {
        m[i][j] += (*(ns.begin() + k))[i][j] * *(ws.begin() + k);
        k++;
      }
    }
  }
}

template <class T>  // m = fun(n1)
void MatApply(vector<vector<T>>& m, vector<vector<T>>& n1, T fun(T)) {
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[i].size(); j++) {
      m[i][j] = fun(n1[i][j]);
    }
  }
}

template <class T>  // m = fun(m, n1)
void MatApply(vector<vector<T>>& m, vector<vector<T>>& n1, T fun(T, T)) {
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[i].size(); j++) {
      m[i][j] = fun(m[i][j], n1[i][j]);
    }
  }
}

template <class T>  // m = fun(n1, n2)
void MatApply(vector<vector<T>>& m, vector<vector<T>>& n1,
              vector<vector<T>>& n2, T fun(T, T)) {
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[i].size(); j++) {
      m[i][j] = fun(n1[i][j], n2[i][j]);
    }
  }
}

template <class T>  // m = fun(m, n1, n2)
void MatApply(vector<vector<T>>& m, vector<vector<T>>& n1,
              vector<vector<T>>& n2, T fun(T, T, T)) {
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[i].size(); j++) {
      m[i][j] = fun(m[i][j], n1[i][j], n2[i][j]);
    }
  }
}

template <class T>  // m = fun(n1, n2, n3)
void MatApply(vector<vector<T>>& m, vector<vector<T>>& n1,
              vector<vector<T>>& n2, vector<vector<T>>& n3, T fun(T, T, T)) {
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[i].size(); j++) {
      m[i][j] = fun(n1[i][j], n2[i][j], n3[i][j]);
    }
  }
}

template <class T>  // m = fun(m, n1, n2, n3)
void MatApply(vector<vector<T>>& m, vector<vector<T>>& n1,
              vector<vector<T>>& n2, vector<vector<T>>& n3, T fun(T, T, T, T)) {
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[i].size(); j++) {
      m[i][j] = fun(m[i][j], n1[i][j], n2[i][j], n3[i][j]);
    }
  }
}

template <class T>  // m = fun(n1, n2, n3, n4)
void MatApply(vector<vector<T>>& m, vector<vector<T>>& n1,
              vector<vector<T>>& n2, vector<vector<T>>& n3,
              vector<vector<T>>& n4, T fun(T, T, T, T)) {
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[i].size(); j++) {
      m[i][j] = fun(n1[i][j], n2[i][j], n3[i][j], n4[i][j]);
    }
  }
}

template <class T>  // m = fun(m, n1, n2, n3, n4)
void MatApply(vector<vector<T>>& m, vector<vector<T>>& n1,
              vector<vector<T>>& n2, vector<vector<T>>& n3,
              vector<vector<T>>& n4, T fun(T, T, T, T, T)) {
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[i].size(); j++) {
      m[i][j] = fun(m[i][j], n1[i][j], n2[i][j], n3[i][j], n4[i][j]);
    }
  }
}
}  // namespace see
