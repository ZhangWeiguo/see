#pragma once
#include <map>
#include <set>
#include <vector>

#include "mat.h"
using namespace std;

namespace see {
/*
改进点
(1) 整体效率偏慢 Inv, QR
(2) 迭代求解Ax = b未判断是否收敛
*/
bool setopposite(vector<int>& indexs, vector<vector<int>>& nozeroindex) {
  int I = indexs.size();
  indexs = vector<int>(I, 0);
  set<int> added;
  int upk = I - 1, bok = I - 1;
  int k = 0;
  while (true) {
    added.clear();
    for (int i = 0; i < I; i++) {
      added.insert(nozeroindex[i][indexs[i]]);
      if (added.size() != i + 1) break;
    }
    if (added.size() == I) {
      for (int i = 0; i < I; i++) indexs[i] = nozeroindex[i][indexs[i]];
      return true;
    } else {
      k = 0;
      while (true) {
        if (nozeroindex[bok].size() - 1 <= indexs[bok] && bok == (upk - k)) {
          bok--;
          upk = I - 1;
          if (bok >= 0) indexs[bok]++;
          for (int i = bok + 1; i < I; i++) indexs[i] = 0;
          break;
        } else {
          if (nozeroindex[upk - k].size() - 1 > indexs[upk - k]) {
            indexs[upk - k] += 1;
            for (int i = upk - k + 1; i < I; i++) indexs[i] = 0;
            break;
          } else
            k++;
        }
      }
    }
    if (bok < 0) break;
  }
  return false;
}

// 调整顺序保证对角元非0 无解返回false
template <class T>
bool SetOpposite(vector<vector<T>>& A, vector<int>& indexs) {
  int I = A.size();
  bool t = true;
  indexs = vector<int>(I, 0);
  for (int k = 0; k < I; k++) {
    if (abs(A[k][k]) < 1e-10 && t) t = false;
    indexs[k] = k;
  }
  if (t)
    return true;
  else {
    vector<set<int>> nozeroindexs(I);   // 非0行
    vector<set<int>> nozerocolumns(I);  // 非0列
    for (int i = 0; i < I; i++) {
      for (int j = 0; j < I; j++) {
        if (abs(A[i][j]) > 1e-10) {
          nozeroindexs[j].insert(i);
          nozerocolumns[i].insert(j);
        }
      }
    }
    for (int i = 0; i < I; i++) {
      if (nozeroindexs[i].size() * nozerocolumns[i].size() == 0) return false;
    }
    vector<vector<int>> nozeroindex(I);  // 非0行
    for (int i = 0; i < I; i++) {
      nozeroindex[i].assign(nozeroindexs[i].begin(), nozeroindexs[i].end());
    }
    return setopposite(indexs, nozeroindex);
  }
  return false;
}

// 矩阵的行列式
template <class T>
double Det(vector<vector<T>>& n) {
  double det = 1, tmp;
  int I = n.size();
  if (I == 0) return 0;
  int J = n[0].size();
  if (J == 0 || I != J) return 0;
  if (I == 1) return n[0][0];
  vector<int> indexs(I, 0);
  if (!SetOpposite(n, indexs)) return 0;
  auto m = n;
  for (int i = 0; i < I; i++) m[i] = n[indexs[i]];
  for (int i = 0; i < I - 1; i++) {
    for (int j = i + 1; j < I; j++) {
      tmp = m[j][i] / m[i][i];
      if (m[j][i] == 0) continue;
      for (int k = 0; k < I; k++) {
        m[j][k] -= m[i][k] * tmp;
      }
    }
  }
  for (int i = 0; i < I; i++) det *= m[i][i];
  return det;
}

// 矩阵的逆
template <class T>  // m = A ^ -1
bool Inv(vector<vector<T>>& A, vector<vector<T>>& m) {
  if (Det<T>(A) == 0) return false;
  int I = A.size();
  if (I == 0) return false;
  int J = A[0].size();
  if (J == 0 || I != J) return false;
  vector<int> indexs(I, 0);
  if (!SetOpposite(A, indexs)) return 0;
  vector<vector<T>> A1(I);
  for (int k = 0; k < I; k++) A1[k] = A[indexs[k]];
  for (int k = 0; k < I; k++) {
    m[k] = vector<T>(I, 0);
    m[k][indexs[k]] = 1;
  }
  double tmp;
  int ii, jj;
  for (int i = 0; i < I - 1; i++) {
    for (int j = i + 1; j < I; j++) {
      if (A1[j][i] != 0) {
        tmp = A1[j][i] / A1[i][i];
        for (int k = 0; k < I; k++) {
          A1[j][k] -= A1[i][k] * tmp;
          m[j][k] -= m[i][k] * tmp;
        }
      }
    }
  }
  for (int i = I - 1; i >= 1; i--) {
    for (int j = i - 1; j >= 0; j--) {
      if (A1[j][i] != 0) {
        tmp = A1[j][i] / A1[i][i];
        for (int k = 0; k < I; k++) {
          A1[j][k] -= A1[i][k] * tmp;
          m[j][k] -= m[i][k] * tmp;
        }
      }
    }
  }
  for (int i = 0; i < I; i++) {
    for (int j = 0; j < I; j++) m[i][j] /= A1[i][i];
  }
}

// 矩阵的广义逆
template <class T>  // m = n ^ -1
bool PInv(vector<vector<T>>& A, vector<vector<T>>& m) {}

// QR分解 I != J 未提供
template <class T>
void QR(vector<vector<T>>& A, vector<vector<T>>& Q, vector<vector<T>>& R) {
  int I = A.size();
  if (I == 0) return;
  int J = A[0].size();
  if (J == 0 || I != J) return;
  auto QQ = ZerosMat<double>(I, I);
  auto RR = ZerosMat<double>(I, I);
  auto H = ZerosMat<double>(I, I);
  auto v = vector<double>(I, 0);
  auto c = vector<double>(I, 0);
  double cc = 0, tc = 0;
  for (int j = 0; j < I - 1; j++) {
    cc = 0;
    for (int k = 0; k < I; k++) {
      if (k >= j) {
        if (j == 0)
          c[k] = A[k][j];
        else
          c[k] = R[k][j];
      } else
        c[k] = 0;
      cc += pow(c[k], 2);
    }
    tc = c[j];
    if (c[j] > 0)
      c[j] += pow(cc, 0.5);
    else
      c[j] -= pow(cc, 0.5);
    cc -= pow(tc, 2);
    cc += pow(c[j], 2);
    for (int k1 = 0; k1 < I; k1++) {
      for (int k2 = 0; k2 <= k1; k2++) {
        if (k1 == k2) {
          H[k1][k2] = 1 - 2 * (c[k1] * c[k2]) / cc;
        } else {
          tc = -2 * (c[k1] * c[k2]) / cc;
          H[k1][k2] = tc;
          H[k2][k1] = tc;
        }
      }
    }
    if (j == 0) {
      Q = H;
      MatDot<double>(R, H, A);
    } else {
      MatDot<double>(QQ, Q, H);
      MatDot<double>(RR, H, R);
      Q = QQ;
      R = RR;
    }
  }
}

// 对称矩阵的特征值 Jacobi方法
template <class T>
void EigValJacobi(vector<vector<T>>& A, vector<double>& eigvalue,
                  double min_e = 1e-5) {
  int I = A.size();
  if (I == 0) return;
  int J = A[0].size();
  if (J == 0 || I != J) return;
  auto R = EyesMat<double>(I);
  eigvalue = vector<double>(I, 0);
  auto AA = A;
  auto AB = A;
  double tao, t, cossita, sinsita;
  int p, q, sign;
  while (true) {
    p = -1;
    for (int i = 0; i < I; i++) {
      for (int j = 0; j < I; j++) {
        if (i != j && abs(AA[i][j]) > min_e) {
          p = i;
          q = j;
          break;
        }
      }
      if (p != -1) break;
    }
    if (p == -1) break;
    tao = (AA[p][p] - AA[q][q]) / (2 * AA[p][q]);
    sign = tao > 0 ? 1 : -1;
    t = tao == 0 ? 1 : sign / (abs(tao) + pow(1 + tao * tao, 0.5));
    cossita = pow((1 + t * t), -0.5);
    sinsita = t * cossita;
    R[p][p] = cossita;
    R[q][p] = -sinsita;
    R[p][q] = sinsita;
    R[q][q] = cossita;
    MatDot<double>(AB, R, AA);
    R[q][p] = sinsita;
    R[p][q] = -sinsita;
    MatDot<double>(AA, AB, R);
    R[p][p] = 1;
    R[q][p] = 0;
    R[p][q] = 0;
    R[q][q] = 1;
  }
  for (int k = 0; k < I; k++) eigvalue[k] = AA[k][k];
  sort(eigvalue.begin(), eigvalue.end());
}

// 一般矩阵的特征值求解 QR方法
template <class T>
void EigValQR(vector<vector<T>>& A, vector<T>& eigvalue, double min_e = 1e-5) {
  int I = A.size(), J = A[0].size();
  if (I == 0 || J == 0 || I != J) return;
  auto AA = A;
  auto Q = ZerosMat<double>(I, I);
  auto R = ZerosMat<double>(I, J);
  bool end = true;
  eigvalue = vector<double>(I, 0);
  while (true) {
    end = true;
    QR<T>(AA, Q, R);
    MatDot<double>(AA, R, Q);
    for (int i = 0; i < I; i++) {
      for (int j = 0; j < i; j++) {
        if (abs(AA[i][j]) > min_e) {
          end = false;
          break;
        }
      }
      if (!end) break;
    }
    if (end) break;
  }
  for (int k = 0; k < I; k++) eigvalue[k] = AA[k][k];
  sort(eigvalue.begin(), eigvalue.end());
}

// 对称矩阵的特征值和特征向量 Jacobi方法
template <class T>
void EigJacobi(vector<vector<T>>& A, vector<vector<T>>& eigvector,
               vector<T>& eigvalue, double min_e = 1e-5) {
  int I = A.size(), J = A[0].size();
  if (I == 0 || J == 0 || I != J) return;
  EigValJacobi(A, eigvalue, min_e);
  eigvector = ZerosMat<double>(I, I);
}

// 一般矩阵的特征值和特征向量 QR方法
template <class T>
void EigQR(vector<vector<T>>& A, vector<vector<T>>& eigvector,
           vector<T>& eigvalue, double min_e = 1e-5) {
  int I = A.size(), J = A[0].size();
  if (I == 0 || J == 0 || I != J) return;
  EigValQR(A, eigvalue, min_e);
  eigvector = ZerosMat<double>(I, I);
}

template <class T>
void Eig(vector<vector<T>>& n, vector<vector<T>>& eigvector,
         vector<T>& eigvalue, bool symmetry = true, double min_e = 1e-5) {
  if (symmetry)
    EigJacobi<T>(n, eigvector, eigvalue, min_e);
  else
    EigQR<T>(n, eigvector, eigvalue, min_e);
}

// ||Ax - b||2
template <class T>
double CalAxbE(vector<vector<T>>& A, vector<T>& x, vector<T>& b) {
  int I = A.size(), J = A[0].size();
  double e = 0, tmp;
  for (int i = 0; i < I; i++) {
    tmp = 0;
    for (int j = 0; j < J; j++) tmp += x[j] * A[i][j];
    tmp -= b[i];
    e += pow(tmp, 2);
  }
  return pow(e, 0.5);
}

// 解线性方程 A * x = b Jacobi迭代法求解 A可逆
// A = D + L + U => (D + L + U) * x = b
// D * x = b - (L + U) => x = D^(-1) * b - D^(-1) * (L + U) * x
// x(k + 1) = D^(-1) * b - D^(-1) * (L + U) * x(k)
// check 检查是否可以收敛 检查(1)A的对角元是否都不为0;(2)A的行列式不等于0
template <class T>
double CalAxbByJacobi(vector<vector<T>>& A, vector<T>& x, vector<T>& b,
                      double min_e = 1e-3, double max_iter = 50,
                      bool check = true) {
  int I = A.size();
  if (I == 0 || b.size() != I) return -1;
  int J = A[0].size();
  if (J == 0 || I != J) return -1;
  // 检查对角元素是否为0 检查是否可逆
  if (check) {
    for (int i = 0; i < J; i++) {
      if (A[i][i] == 0) return -1;
    }
    if (Det<T>(A) == 0) return -1;
  }
  // 判断是否能收敛
  {}
  if (max_iter < I || max_iter < J) max_iter = max(I, J);
  double e = 100, tmp = 0, emp = 0;
  int ite = 0;
  e = CalAxbE(A, x, b);
  vector<double> xx(J, 1);
  while (ite <= max_iter && e > min_e) {
    e = 0;
    for (int i = 0; i < J; i++) {
      tmp = 0;
      for (int j = 0; j < J; j++) {
        if (i != j) tmp += x[j] * A[i][j];
      }
      emp = xx[i];
      xx[i] = (b[i] - tmp) / A[i][i];
      emp = abs(emp - xx[i]);
      e = emp > e ? emp : e;
    }
    x.assign(xx.begin(), xx.end());
    ite++;
  }
  return e;
}

// 解线性方程 A * x = b Gauss-Seidel迭代法求解 A可逆
// A = D + L + U => (D + L + U) * x = b
// (D + L) * x = b - U => x = (D + L)^(-1) * b - (D + L)^(-1) * U * x
// x(k + 1) = (D + L)^(-1) * b - (D + L)^(-1) * U * x(k)
// check 检查是否可以收敛
template <class T>
double CalAxbByGaussSeidel(vector<vector<T>>& A, vector<T>& x, vector<T>& b,
                           double min_e = 1e-3, double max_iter = 50,
                           bool check = true) {
  int I = A.size();
  if (I == 0 || b.size() != I) return -1;
  int J = A[0].size();
  if (J == 0 || I != J) return -1;
  if (check) {
    for (int i = 0; i < J; i++) {
      if (A[i][i] == 0) return -1;
    }
    if (Det<T>(A) == 0) return -1;
  }
  // 判断是否能收敛
  {}
  if (max_iter < I || max_iter < J) max_iter = max(I, J);
  double e = 100, tmp = 0, emp = 0;
  int ite = 0;
  e = CalAxbE(A, x, b);
  while (ite <= max_iter && e > min_e) {
    e = 0;
    for (int i = 0; i < J; i++) {
      tmp = 0;
      for (int j = 0; j < J; j++) {
        if (i != j) tmp += x[j] * A[i][j];
      }
      emp = x[i];
      x[i] = (b[i] - tmp) / A[i][i];
      emp = abs(emp - x[i]);
      e = emp > e ? emp : e;
    }
    ite++;
    cout << ite << ":" << e << endl;
  }
  return e;
}

// 解线性方程 A * x = b Gauss-Seidel迭代法求解 A可逆
// check 检查是否可以收敛 w为松弛因子
// 松弛因子可选择最优因子 需要优化得到
// A = D + L + U => (D + L + U) * x = b
// (D + L) * x = b - U => x = (D + L)^(-1) * b - (D + L)^(-1) * U * x
// x(k + 1) = (D + L)^(-1) * b - (D + L)^(-1) * U * x(k)
// x(k + 1) = (1- w) x(k) + w * x(k+1)
template <class T>
double CalAxbBySOR(vector<vector<T>>& A, vector<T>& x, vector<T>& b, double w,
                   double min_e = 1e-3, double max_iter = 50,
                   bool check = true) {
  int I = A.size();
  if (I == 0 || b.size() != I) return -1;
  int J = A[0].size();
  if (J == 0 || I != J) return -1;
  if (check) {
    for (int i = 0; i < J; i++) {
      if (A[i][i] == 0) return -1;
    }
    if (Det<T>(A) == 0) return -1;
  }
  // 判断是否能收敛
  {}
  if (max_iter < I || max_iter < J) max_iter = max(I, J);
  double e = 0, tmp = 0, emp = 0;
  int ite = 0;
  e = CalAxbE(A, x, b);
  while (ite <= max_iter && e > min_e) {
    e = 0;
    for (int i = 0; i < J; i++) {
      tmp = 0;
      for (int j = 0; j < J; j++) {
        if (i != j) tmp += x[j] * A[i][j];
      }
      emp = x[i];
      x[i] = (b[i] - tmp) / A[i][i];
      x[i] = (1 - w) * emp + w * x[i];
      emp = abs(emp - x[i]);
      e = emp > e ? emp : e;
    }
    ite++;
    // cout << ite << ":" << e << endl;
  }
  return e;
}

// 最小二乘法 解线性方程 A * x = b
template <class T>
double CalAxbByOLS(vector<vector<T>>& A, vector<T>& x, vector<T>& b,
                   double min_e = 1e-3, double max_iter = 500,
                   double eta = 0.0001) {
  int I = A.size();
  if (I == 0 || b.size() != I) return -1;
  int J = A[0].size();
  vector<T> deltax(J, 0);
  T fx, ex;
  for (int k = 0; k < max_iter; k++) {
    if (k > 0)
      for (int j = 0; j < J; j++) deltax[j] = 0;
    for (int i = 0; i < I; i++) {
      fx = 0;
      for (int j = 0; j < J; j++) fx += A[i][j] * x[j];
      for (int j = 0; j < J; j++) deltax[j] += A[i][j] * 2 * (fx - b[i]);
    }
    for (int j = 0; j < J; j++) {
      x[j] -= eta * deltax[j];
    }
    ex = CalAxbE(A, x, b);
    if (ex <= min_e) break;
    // cout <<k << ": " << ex << endl;
  }
  return ex;
}

// 用逆 解线性方程 A * x = b 保证 b != 0 && A 可逆
template <class T>
double CalAxbByInv(vector<vector<T>>& A, vector<T>& x, vector<T>& b,
                   bool check = true) {
  int I = A.size();
  if (check) {
    if (Det<T>(A) == 0) return -1;
  }
  auto A1 = ZerosMat<T>(I, I);
  Inv(A, A1);
  for (int i = 0; i < I; i++) {
    x[i] = 0;
    for (int j = 0; j < I; j++) x[i] += A1[i][j] * b[j];
  }
  return CalAxbE(A, x, b);
}

// Gauss主元消去 解线性方程 A * x = b
// 得到 方程的准确解或基础解系
template <class T>
double CalAxbByGaussElimination(vector<vector<T>>& A, vector<vector<T>>& x,
                                vector<T>& b) {
  // 先进行初等行变换, 确定自由度
  // 再去掉自由部分 使用CalAxbByInv求解局部矩阵
  int I = A.size();
  if (I == 0 || b.size() != I) return -1;
  int J = A[0].size();
  if (J == 0) return -1;
  double e;
  if (I == J && Det<T>(A) != 0) {
    vector<T> x0(I, 0);
    e = CalAxbByInv(A, x0, b, false);
    x.emplace_back(x0);
  } else {
    for (int i = 0; i < I; i++) {
      for (int j = 0; j < I; j++) {
      }
    }
  }
  return e;
}

// 解线性方程 A * x = b (0:SOR 1:Gauss-Seidel 2:Jacobi 3:GaussElimination
// -1:OLS) 0,1,2  A为方阵且A可逆且G的谱半径小于1 w 为SOR的松弛因子 当且仅当way =
// 0生效 返回-1表示无法计算
template <class T>
double CalAxb(vector<vector<T>>& A, vector<T>& x, vector<T>& b,
              double min_e = 1e-3, double max_iter = 50, int way = -1,
              double w = 0.5) {
  int I = A.size();
  if (I == 0 || b.size() != I) return -1;
  int J = A[0].size();
  if (J == 0) return -1;
  if (I != J) way = -1;
  // 调整顺序保证对角元非0
  vector<int> indexs(I, 0);
  if (!SetOpposite(A, indexs)) way = -1;
  if (Det<T>(A) == 0) way = -1;
  double result;
  switch (way) {
    case 0:
      result = CalAxbBySOR(A, x, b, w, min_e, max_iter, false);
      break;
    case 1:
      result = CalAxbByGaussSeidel(A, x, b, min_e, max_iter, false);
      break;
    case 2:
      result = CalAxbByJacobi(A, x, b, min_e, max_iter, false);
      break;
    default:
      result = CalAxbByOLS(A, x, b, min_e, max_iter, false);
      break;
  }
  return result;
}

// 解线性方程 A * x = 0
template <class T>
double CalAxb(vector<vector<T>>& A, vector<T>& x, double min_e = 1e-3,
              double max_iter = 50, int way = -1, double w = 0.5) {
  int I = A.size();
  if (I == 0) return -1;
  int J = A[0].size();
  if (J == 0) return -1;
  vector<T> b(I, 0);
  if (I != J) {
    return CalAxb(A, x, b, min_e, max_iter, -1, w);
  }
  if (Det<T>(A) == 0) return CalAxb(A, x, b, min_e, max_iter, -1, w);
  vector<int> indexs(I, 0);
  if (!SetOpposite(A, indexs)) return CalAxb(A, x, b, min_e, max_iter, -1, w);
  auto A1 = A;
  return 0;
}

// SVD分解
template <class T>
void SVD(vector<vector<T>>& n) {}

template <class T>
double Cosine(vector<T>& v1, vector<T>& v2) {
  double x = 0, x1 = 0, x2 = 0;
  if (v1.size() == 0) return x;
  for (int i = 0; i < v1.size(); i++) {
    x += v1[i] * v2[i];
    x1 += pow(v1[i], 2);
    x2 += pow(v2[i], 2);
  }
  x = x / (pow(x1, 0.5) * pow(x2, 0.5));
  return x;
}

template <class T>
double Euclidean(vector<T>& v1, vector<T>& v2) {
  double x = 0;
  if (v1.size() == 0) return x;
  for (int i = 0; i < v1.size(); i++) {
    x += pow((v1[i] - v2[i]), 2);
  }
  x = pow(x, 0.5);
  return x;
}

};  // namespace see