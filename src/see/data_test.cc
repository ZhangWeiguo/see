#include "data.cc"

#include <iostream>
#include <vector>

#include "data.h"
#include "mat.h"

using namespace std;

int main() {
  see::OneHotEncoder oe;
  see::DoubleMat d0, d1, d2;
  d0.push_back({1, 0, 2});
  d0.push_back({1});
  d0.push_back({2});
  d0.push_back({0});
  d0.push_back({1});
  cout << "Origin Data:" << endl;
  see::MatPrint(d0);
  oe.Train(&d0);
  oe.Transform(&d0, &d1);
  cout << "Transform Data:" << endl;
  see::MatPrint(d1);
  oe.InvTransform(&d1, &d2);
  cout << "InvTransform Data:" << endl;
  see::MatPrint(d2);
  return 0;
}