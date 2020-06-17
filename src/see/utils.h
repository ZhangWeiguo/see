#ifndef SEE_UTILS_H
#define SEE_UTILS_H
#include <vector>

#include "math.h"
using namespace std;
namespace see {
inline double sigmoid(double x) { return 1.0 / (1.0 + exp(-1 * x)); }
inline double relu(double x) { return x > 0 ? 1.0 : 0.0; }
inline double relu(double x, double y) { return x > y ? 1.0 : 0.0; }

inline double norm(vector<double>& a, int n = 1) {
  double dis = 0;
  if (n >= 2)
    for (int i = 0; i < a.size(); i++) dis += pow(a[i], n);
  else
    for (int i = 0; i < a.size(); i++) dis += abs(a[i]);
  return dis;
}

inline double linearKernel(vector<double>& a, vector<double>& b) {
  double dis = 0;
  for (int i = 0; i < a.size(); i++) dis += a[i] * b[i];
  return dis;
}

// d >= 1
inline double polyKernel(vector<double>& a, vector<double>& b, double d) {
  double dis = 0;
  for (int i = 0; i < a.size(); i++) dis += a[i] * b[i];
  return pow(dis, d);
}

// d > 0
inline double gaussKernel(vector<double>& a, vector<double>& b, double d) {
  double dis = 0, dd = 2 * d * d;
  for (int i = 0; i < a.size(); i++) dis += pow(a[i] - b[i], 2);
  return exp(-dis / dd);
}

// d > 0
inline double laplaceKernel(vector<double>& a, vector<double>& b, double d) {
  double dis = 0;
  for (int i = 0; i < a.size(); i++) dis += abs(a[i] - b[i]);
  return exp(-dis / d);
}

// d1 > 0, d2 < 0
inline double sigmoidKernel(vector<double>& a, vector<double>& b, double d1,
                            double d2) {
  double dis = 0, e1 = 0, e2 = 0;
  for (int i = 0; i < a.size(); i++) dis += a[i] * b[i];
  dis = d1 * dis + d2;
  e1 = exp(-dis);
  e2 = exp(dis);
  return (e2 - e1) / (e2 + e1);
}

}  // namespace see
#endif