#include "mat.h"

#include <chrono>
#include <initializer_list>
#include <iostream>
#include <vector>

#include "../basic/forever.h"
#include "omp.h"
using namespace std;

// v1 = v1 + v2 + v3 + v4 + v5 + v6

see::DoubleMat v1 = see::ZerosMat<double>(10000, 5000);
see::DoubleMat v2 = see::ZerosMat<double>(10000, 5000);
see::DoubleMat v3 = see::ZerosMat<double>(10000, 5000);
see::DoubleMat v4 = see::ZerosMat<double>(10000, 5000);
see::DoubleMat v5 = see::ZerosMat<double>(10000, 5000);

void test_mat() {
  see::MatAdd<double>(v1, v2);
  see::MatAdd<double>(v1, v3);
  see::MatAdd<double>(v1, v4);
  see::MatAdd<double>(v1, v5);
  see::MatAdd<double>(v1, v2);
}

void test_vector1() {
  for (int i = 0; i < v1.size(); i++) {
    for (int j = 0; j < v2[i].size(); j++) {
      v1[i][j] += (v2[i][j] + v3[i][j] + v4[i][j] + v5[i][j]);
    }
  }
}

void test_vector2() {
  for (int i = 0; i < v1.size(); i++) {
    for (int j = 0; j < v2[i].size(); j++) {
      v1[i][j] += v2[i][j];
      v1[i][j] += v3[i][j];
      v1[i][j] += v4[i][j];
      v1[i][j] += v5[i][j];
    }
  }
}

void test_vector3() {
  initializer_list<vector<vector<double>>> ns = {v2, v3, v4, v5};
  initializer_list<double> ws = {1.0, 1.0, 1.0, 1.0};
  see::MatApply<double>(v1, ns, ws, 1.0);
}

inline double fun(double x0, double x1, double x2, double x3, double x4) {
  return x0 + x1 + x2 + x2 + x2 + x2;
}
void test_vector4() { see::MatApply<double>(v1, v2, v3, v4, v5, fun); }

void test_vector5() {
  see::MatAdd<double>(v1, v1, v2, v3, v4, v5, 0, 1, 1, 1, 1, 1);
}

void test(string name, void fun()) {
  auto t0 = basic::Time::NowMil();
  fun();
  auto t1 = basic::Time::NowMil();
  cout << left << setw(20) << name << "Time Cost: " << (t1 - t0) << endl;
}

int main() {
  test("**Mat", test_mat);
  test("**Mat", test_mat);
  test("**Mat", test_mat);
  test("**Mat", test_mat);

  test("Vector1", test_vector1);
  test("Vector1", test_vector1);
  test("Vector1", test_vector1);
  test("Vector1", test_vector1);

  test("Vector2", test_vector2);
  test("Vector2", test_vector2);
  test("Vector2", test_vector2);
  test("Vector2", test_vector2);

  test("**Vector3", test_vector3);
  test("**Vector3", test_vector3);
  test("**Vector3", test_vector3);
  test("**Vector3", test_vector3);

  test("**Vector4", test_vector4);
  test("**Vector4", test_vector4);
  test("**Vector4", test_vector4);
  test("**Vector4", test_vector4);

  test("**Vector5", test_vector5);
  test("**Vector5", test_vector5);
  test("**Vector5", test_vector5);
  test("**Vector5", test_vector5);
  return 0;
}