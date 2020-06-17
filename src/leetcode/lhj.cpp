#include <math.h>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

bool Equal(double a1, double a2);
double Max(double a1, double a2);
double FunMax(vector<double> v1, vector<double> v2);
double FunMin(vector<double> v1, vector<double> v2);
bool RestrictBasic(vector<double> v);
bool RestrictX(vector<double> v);
bool RestrictY(vector<double> v);
bool RestrictZ(vector<double> v);
bool RestrictOrder(vector<double> v1, vector<double> v2, vector<double> v3);
void PrintVector(vector<double> v);

double Max(double a1, double a2) {
  if (a1 >= a2) {
    return a1;
  }
  return a2;
}

double FunMax(vector<double> v1, vector<double> v2) {
  double a1, a2;
  a1 = (v1[0] + v1[1] - v2[0]) / (v2[1] + v2[2]);
  a2 = 1.0 - (v1[3] - v2[3]) / (v2[2] - v1[2]);
  return Max(a1, a2);
};

double FunMin(vector<double> v1, vector<double> v2) {
  double a1, a2, a3;
  a1 = v2[0] / (v1[0] + v1[1]);
  a2 = (v2[0] - v1[0]) / (v1[1] - v2[1]);
  a3 = 1 - v2[3] / (v1[2] + v1[3]);
  return Max(Max(a1, a2), a3);
};

bool RestrictBasic(vector<double> v) {
  if (Equal(v[0] + v[1] + v[2] + v[3], 1.0)) {
    return true;
  }
  return false;
}

bool RestrictX(vector<double> v) {
  if (v[0] >= v[1] && v[1] > v[2] && v[2] > v[3]) {
    return true;
  }
  return false;
}

bool RestrictY(vector<double> v) {
  if (v[0] > v[1] && v[1] > v[2] && v[2] > v[3]) {
    return true;
  }
  return false;
}

bool RestrictZ(vector<double> v) {
  if (v[0] > v[1] && v[1] >= v[2] && v[2] > v[3]) {
    return true;
  }
  return false;
}

bool RestrictOrder(vector<double> v1, vector<double> v2, vector<double> v3) {
  if (v3[0] > v2[0] && v2[0] > v1[0] && v1[0] >= v1[1] && v1[1] > v2[1] &&
      v2[1] > v3[1] && v3[1] >= v3[2] && v3[2] > v2[2] && v2[2] > v1[2] &&
      v1[2] > v1[3] && v1[3] > v2[3] && v2[3] > v3[3] && v3[3] >= 0) {
    return true;
  }
  return false;
}

void PrintVector(vector<double> v) {
  for (vector<double>::iterator it = v.begin(); it != v.end(); it++) {
    cout << *it << " ";
  }
  cout << endl;
}

bool Equal(double a1, double a2) {
  if (fabs(a1 - a2) < 1e-8) {
    return true;
  }
  return false;
}

int main() {
  int start_time, end_time;
  start_time = time(0);
  vector<double> r, v;
  vector<vector<double>> I, X, Y, Z;
  int AllNum = 0;
  int ResNum = 0;
  double step;
  int sample_combine_rate, sample_rate;
  cout << "Please Input Your Step(0~1): ";
  cin >> step;
  cout << "Please Input Your Sample Rate(0~100): ";
  cin >> sample_rate;
  cout << "Please Input Your Sample Combine Rate(0~100): ";
  cin >> sample_combine_rate;
  double init = 0.0;
  while (init < (1.0 + step)) {
    r.push_back(init);
    init += step;
  }
  cout << "Step: " << step << "; Element Num: " << r.size() << endl;
  cout << "======================" << endl;
  cout << "Begin To Cal X Y Z" << endl;
  int k = 0;
  for (vector<double>::iterator it1 = r.begin(); it1 != r.end(); it1++) {
    for (vector<double>::iterator it2 = r.begin(); it2 != r.end(); it2++) {
      for (vector<double>::iterator it3 = r.begin(); it3 != r.end(); it3++) {
        for (vector<double>::iterator it4 = r.begin(); it4 != r.end(); it4++) {
          if (rand() % 100 > sample_rate) {
            continue;
          }
          v.clear();
          v.push_back(*it1);
          v.push_back(*it2);
          v.push_back(*it3);
          v.push_back(*it4);
          if (RestrictBasic(v)) {
            k++;
            if (RestrictX(v)) {
              X.push_back(v);
            }
            if (RestrictY(v)) {
              Y.push_back(v);
            }
            if (RestrictZ(v)) {
              Z.push_back(v);
            }
            if (k % 10000 == 0) {
              cout << "X " << X.size() << " ";
              cout << "Y " << Y.size() << " ";
              cout << "Z " << Z.size() << endl;
            }
          }
        }
      }
    }
  }
  cout << "End To Cal X Y Z" << endl;
  cout << "======================" << endl;
  cout << "X Len: " << X.size() << endl;
  cout << "Y Len: " << Y.size() << endl;
  cout << "Z Len: " << Z.size() << endl;
  cout << "======================" << endl;
  for (vector<vector<double>>::iterator itx = X.begin(); itx != X.end();
       itx++) {
    for (vector<vector<double>>::iterator ity = Y.begin(); ity != Y.end();
         ity++) {
      for (vector<vector<double>>::iterator itz = Z.begin(); itz != Z.end();
           itz++) {
        if (rand() % 100 > sample_combine_rate) {
          continue;
        }
        if (RestrictOrder(*itx, *ity, *itz)) {
          if (FunMax(*itx, *ity) <= FunMin(*itx, *ity) &&
              FunMax(*ity, *itz) <= FunMin(*ity, *itz)) {
            AllNum++;
            if (FunMax(*itx, *itz) > FunMin(*itz, *itz)) {
              cout << "Found One Combination Satified:" << endl;
              ResNum++;
              PrintVector(*itx);
              PrintVector(*ity);
              PrintVector(*itz);
            }
          }
        }
      }
    }
  }
  cout << "All      Num: " << AllNum << endl;
  cout << "Satified Num: " << ResNum << endl;
  end_time = time(0);
  cout << "All Cost Time: " << end_time - start_time << "S" << endl;
  return 0;
}
