#include <algorithm>
#include <iostream>
#include <stack>
#include <string>
using namespace std;

int cal_unit(stack<string>& C) {
  string symbol;
  int I0 = atoi(C.top().data());
  C.pop();
  while (!C.empty()) {
    if (C.top() == ")") {
      C.pop();
      break;
    }
    if (C.top() == "+" || C.top() == "-") {
      symbol = C.top();
    } else {
      if (symbol == "+") {
        I0 += atoi(C.top().data());
      } else if (symbol == "-") {
        I0 -= atoi(C.top().data());
      }
    }
    C.pop();
  }
  return I0;
}

class Solution {
 public:
  // stack 模拟计算
  int calculateOrigin(string s) {
    stack<string> C;
    string S0;
    int I0 = 0;
    for (auto it = s.rbegin(); it != s.rend(); it++) {
      if (*it >= '0' && *it <= '9') {
        S0.insert(0, 1, *it);
      } else {
        if (S0.length() > 0) {
          C.push(S0);
          S0.clear();
        }
        if (*it == '+' || *it == '-' || *it == ')') {
          C.push(string(1, *it));
        } else if (*it == '(') {
          I0 = cal_unit(C);
          C.push(to_string(I0));
        }
      }
    }
    if (S0.length() > 0) {
      C.push(S0);
      S0.clear();
    }
    I0 = cal_unit(C);
    return I0;
  }

  // 递归 模拟计算
  int calculate(string& s) {
    int I0 = 0;
    int i = 0, i0 = 0;
    int k = 0, k0 = 0;
    string s0;
    char symbol = '0';
    while (i < s.size()) {
      if (s[i] >= '0' && s[i] <= '9') {
        s0.push_back(s[i]);
      } else {
        if (s0.length() > 0) {
          if (symbol == '0') {
            I0 = atoi(s0.data());
          } else {
            if (symbol == '+') I0 += atoi(s0.data());
            if (symbol == '-') I0 -= atoi(s0.data());
          }
          s0.clear();
        }
        if (s[i] == '+' || s[i] == '-') {
          symbol = s[i];
        } else if (s[i] == '(') {
          k = 0;
          k0 = 1;
          while (k0 > 0) {
            k++;
            if (s[i + k] == ')') k0--;
            if (s[i + k] == '(') k0++;
          }
          auto st = s.substr(i + 1, k - 1);
          i += k;
          switch (symbol) {
            case '+':
              I0 += calculate(st);
              break;
            case '-':
              I0 -= calculate(st);
              break;
            case '0':
              I0 = calculate(st);
            default:
              break;
          }
        }
      }
      i++;
    }
    if (s0.length() > 0) {
      if (symbol == '0') {
        I0 = atoi(s0.data());
      } else {
        if (symbol == '+') I0 += atoi(s0.data());
        if (symbol == '-') I0 -= atoi(s0.data());
      }
      s0.clear();
    }
    return I0;
  }
};

int main() {
  Solution S;
  string s = "(1+(4+5+2)-3)+(6+8)";
  cout << S.calculateOrigin(s) << endl;
  cout << S.calculate(s) << endl;
}