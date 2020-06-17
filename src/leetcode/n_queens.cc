#include <stdio.h>
#include <math.h>
#include <iostream>
#include <list>
#include <set>
#include <vector>
using namespace std;


vector<vector<string>> solveNQueens(int n) {
  vector<vector<string> > result;
  vector<vector<int> > paths;
  for (int i = 0; i < n; i++) {
    if (i == 0) {
      for (int j = 0; j < n; j++) {
        paths.emplace_back(vector<int>{j});
      }
    } else {
      vector<vector<int> > new_paths;
      for (auto &it: paths) {
        for (int j = 0; j < n; j++) {
          bool add = true;
          for (int k = 0; k < it.size(); k++) {
            if (it[k] == j || abs(it[k] - j) == abs(k - i)) {
              add = false;
              break;
            }
          }
          if (add) {
            auto jt = it;
            jt.emplace_back(j);
            new_paths.emplace_back(move(jt));
            cout << i << "th: ";
            for (auto &kt: new_paths[new_paths.size() - 1]) cout << kt << ": ";
            cout << endl;
          }
        }
      }
      paths = new_paths;
    }
  }
  for (auto &it: paths) {
    vector<string> s(n, string(n, '.'));
    for (int i = 0; i < n; i++) {
      s[i][it[i]] = 'Q';
    }
    result.emplace_back(move(s));
  }
  return result;
}


class Solution {
 public:
  vector<vector<string>> solveNQueensAdvance(int n) {
    vector<vector<string>> S;
    vector<string> S0;
    string S00(n, '.');
    set<vector<int>> I;
    set<vector<int>> I_T;
    vector<int> kt;
    bool add;
    for (int i = 0; i < n; i++) {
      if (i == 0) {
        for (int j = 0; j < n; j++) {
          I.insert(vector<int>{j});
        }
      } else {
        I_T.clear();
        for (auto it = I.begin(); it != I.end(); it++) {
          for (int j = 0; j < n; j++) {
            add = true;
            int k = 0;
            for (auto jt = it->begin(); jt != it->end(); jt++, k++) {
              if (*jt == j || abs((*it)[k] - j) == abs(k - i)) {
                add = false;
                break;
              }
            }
            if (add) {
              kt = *it;
              kt.push_back(j);
              I_T.insert(kt);
            }
          }
        }
        I = I_T;
      }
    }
    for (auto it = I.begin(); it != I.end(); it++) {
      S0.clear();
      for (auto jt = it->begin(); jt != it->end(); jt++) {
        S00[*jt] = 'Q';
        S0.push_back(S00);
        S00[*jt] = '.';
      }
      S.push_back(S0);
    }
    return S;
  }
};

int main() {
  Solution S;
  cout << solveNQueens(4).size() << endl;
  // cout << S.solveNQueensAdvance(5).size() << endl;
  return 0;
}