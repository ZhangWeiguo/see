#include <math.h>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <numeric>
#include <thread>
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

vector<vector<double>> Y;
mutex m;
void thread_fun(vector<double> r, double mi, double ma, int i) {
  vector<vector<double>> Y1;
  for (vector<double>::iterator it1 = r.begin(); it1 != r.end(); it1++) {
    if (*it1 < mi || *it1 > ma) continue;
    cout << "Thread " << i
         << " Rate Of Process: " << (*it1 - mi) / (ma - mi) * 100 << "%"
         << endl;
    for (vector<double>::iterator it2 = r.begin(); it2 != r.end(); it2++) {
      if (*it1 + *it2 > 1 || *it1 <= *it2) {
        break;
      }
      for (vector<double>::iterator it3 = r.begin(); it3 != r.end(); it3++) {
        if (*it1 + *it2 + *it3 > 1 || *it2 <= *it3) {
          break;
        }
        for (vector<double>::iterator it4 = r.begin(); it4 != r.end(); it4++) {
          if (*it1 + *it2 + *it3 + *it4 > 1.0000000000001 || *it3 <= *it4) {
            break;
          }
          vector<double> v{*it1, *it2, *it3, *it4};
          if (RestrictBasic(v)) {
            if (RestrictY(v)) {
              Y1.push_back(v);
            }
          }
        }
      }
    }
  }
  m.lock();
  for (auto it = Y1.begin(); it != Y1.end(); it++) Y.push_back(*it);
  m.unlock();
}

int main() {
  int start_time, end_time;
  start_time = time(0);
  vector<double> r, v;
  vector<vector<double>> I, X, Z;
  vector<double> x0(4, 0), z0(4, 0);
  int AllNum = 0;
  int ResNum = 0;
  double step;
  cout << "Please Input Your Step(0~1): ";
  cin >> step;
  cout << "Please Input Your X (split by a space): ";
  for (int i = 0; i < 4; i++) cin >> x0[i];
  cout << "Please Input Your Z (split by a space): ";
  for (int i = 0; i < 4; i++) cin >> z0[i];
  X.push_back(x0);
  Z.push_back(z0);
  double init = 0.0;
  while (init < (1.0 + step)) {
    r.push_back(init);
    init += step;
  }
  cout << "Step: " << step << "; Element Num: " << r.size() << endl;
  cout << "======================" << endl;
  cout << "Begin To Cal X Y Z" << endl;
  thread t1(thread_fun, r, 0.0, 0.4, 1);
  thread t2(thread_fun, r, 0.5, 0.6, 2);
  thread t3(thread_fun, r, 0.6, 0.7, 3);
  thread t4(thread_fun, r, 0.7, 0.8, 4);
  thread t5(thread_fun, r, 0.8, 0.9, 5);
  thread t6(thread_fun, r, 0.9, 1.0, 6);
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();
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
        if (RestrictOrder(*itx, *ity, *itz)) {
          if (FunMax(*itx, *ity) <= FunMin(*itx, *ity) &&
              FunMax(*ity, *itz) <= FunMin(*ity, *itz) &&
              FunMax(*itx, *itz) > FunMin(*itz, *itz) &&
              (((*itx)[3] - (*ity)[3]) / ((*ity)[2] - (*itx)[2]) >
                   ((*ity)[3] - (*itz)[3]) / ((*itz)[2] - (*ity)[2]) ||
               ((*ity)[0] - (*itx)[0]) / ((*itx)[1] - (*ity)[1]) >
                   ((*itz)[0] - (*ity)[0]) / ((*ity)[1] - (*itz)[1]))) {
            cout << "Found One Combination Satified:" << endl;
            ResNum++;
            PrintVector(*itx);
            PrintVector(*ity);
            PrintVector(*itz);
            return 0;
          }
        }
      }
    }
  }
  cout << "Satified Num: " << ResNum << endl;
  end_time = time(0);
  cout << "All Cost Time: " << end_time - start_time << "S" << endl;
  return 0;
}
