#include "matc.h"

#include <iostream>

#include "mat.h"
using namespace std;
int main() {
  vector<vector<double>> T = {{1, 2, 3}, {2, 0, 5}, {3, 5, 3}};
  auto eve = see::ZerosMat<double>(3, 3);
  auto eva = vector<double>(3, 0);
  cout << "Test For Jacobi" << endl;
  see::EigJacobi<double>(T, eve, eva);
  for (int k = 0; k < 3; k++) cout << eva[k] << endl;
  cout << "Test For QR" << endl;
  see::EigQR<double>(T, eve, eva);
  for (int k = 0; k < 3; k++) cout << eva[k] << endl;

  vector<vector<double>> A = see::RandomMat<double>(100, 100, 1, 2);
  vector<double> b(100, 1);
  vector<double> x(100, 1);

  cout << "Test For Ax = b: OLS" << endl;
  cout << see::CalAxbByOLS(A, x, b, 1e-3, 1000, 0.00001) << endl;
  cout << see::CalAxbE(A, x, b) << endl;

  cout << "Test For Ax = b: Inv" << endl;
  cout << see::CalAxbByInv(A, x, b) << endl;
  cout << see::CalAxbE(A, x, b) << endl;

  // vector<double> b(100, 1);
  // vector<double> x(100, 1);
  // cout << "Det:    " << see::Det(A) << endl;
  // see::CalAxbBySOR(A, x, b, 0.5, 1e-3, 100, false);
  // see::CalAxbByJacobi(A, x, b, 0.5);
  // see::CalAxbByGaussSeidel(A, x, b, 0.5);
  // see::CalAxbByOLS(A, x, b);
  // see::CalAxbByGaussElimination(A, x, b);

  return 0;
}