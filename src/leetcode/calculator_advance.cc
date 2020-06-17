#include <algorithm>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;
class Solution {
 public:
  // 加入了乘除法的 模拟计算器
  int calculate(string& s) {
    int I0 = 0;
    stack<string> C0;
    stack<int> C1;
    vector<string> V0;
    string S0;
    int i = 0, k = 0;
    int start = 0, end = 0;
    while (i < s.size()) {
      if (s[i] >= '0' && s[i] <= '9') {
        k = 1;
        while (i + k < s.size()) {
          if (s[i + k] >= '0' && s[i + k] <= '9') {
            k++;
          } else {
            break;
          }
        }
        S0 = s.substr(i, k);
        i += (k - 1);
        V0.push_back(S0);
      } else if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') {
        if (C0.empty()) {
          C0.push(string(1, s[i]));
        } else {
          switch (s[i]) {
            case '+':
              while (!C0.empty()) {
                if (C0.top() == "+" || C0.top() == "-" || C0.top() == "*" ||
                    C0.top() == "/") {
                  V0.push_back(C0.top());
                  C0.pop();
                } else {
                  break;
                }
              }
              break;
            case '-':
              while (!C0.empty()) {
                if (C0.top() == "+" || C0.top() == "-" || C0.top() == "*" ||
                    C0.top() == "/") {
                  V0.push_back(C0.top());
                  C0.pop();
                } else {
                  break;
                }
              }
              break;
            case '*':
              while (!C0.empty()) {
                if (C0.top() == "*" || C0.top() == "/") {
                  V0.push_back(C0.top());
                  C0.pop();
                } else {
                  break;
                }
              }
              break;
            case '/':
              while (!C0.empty()) {
                if (C0.top() == "*" || C0.top() == "/") {
                  V0.push_back(C0.top());
                  C0.pop();
                } else {
                  break;
                }
              }
              break;
            default:
              break;
          }
          C0.push(string(1, s[i]));
        }
      } else if (s[i] == '(') {
        C0.push(string(1, s[i]));
      } else if (s[i] == ')') {
        while (C0.top() != "(") {
          V0.push_back(C0.top());
          C0.pop();
        }
        C0.pop();
      }
      i++;
    }
    while (!C0.empty()) {
      V0.push_back(C0.top());
      C0.pop();
    }
    for (i = 0; i < V0.size(); i++) {
      if (V0[i] == "+" || V0[i] == "-" || V0[i] == "*" || V0[i] == "/") {
        I0 = C1.top();
        C1.pop();
        if (V0[i] == "+") {
          I0 += C1.top();
          C1.pop();
        } else if (V0[i] == "-") {
          I0 = C1.top() - I0;
          C1.pop();
        } else if (V0[i] == "*") {
          I0 = C1.top() * I0;
          C1.pop();
        } else if (V0[i] == "/") {
          I0 = C1.top() / I0;
          C1.pop();
        }
        C1.push(I0);
      } else {
        C1.push(atoi(V0[i].data()));
      }
    }
    return C1.top();
  }
};

int main() {
  Solution S;
  // string s = "1+2*3+(11+(4+5+2)-3)+(6+8)";
  string s = "1+1+1";
  cout << S.calculate(s) << endl;
}