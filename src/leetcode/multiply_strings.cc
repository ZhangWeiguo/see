#include <iostream>
#include <string>
using namespace std;
string addstr(string s1, string s2) {
  string s;
  int c, n;
  int c1, c2;
  int N1 = s1.size();
  int N2 = s2.size();
  n = 0;
  for (int i = 0; i < N1 || i < N2 || n != 0; i++) {
    c1 = 0;
    c2 = 0;
    if (i < s1.size()) c1 = atoi(s1.substr(N1 - 1 - i, 1).data());
    if (i < s2.size()) c2 = atoi(s2.substr(N2 - 1 - i, 1).data());
    c = n + c1 + c2;
    if (c > 9) {
      n = 1;
      c = c - 10;
    } else {
      n = 0;
    }
    if (i >= N1 - 1 && i >= N2 - 1 && n == 0 && c == 0) break;
    s = to_string(c) + s;
  }
  if (s == "") s = "0";
  return s;
}

string multiplyOne(string s1, string s2) {
  string s;
  int c, n;
  int N1 = s1.size();
  int c2 = atoi(s2.c_str());
  if (c2 == 0) return "0";
  n = 0;
  for (int i = 0; i < N1 || n != 0; i++) {
    c = 0;
    if (i < N1) c = atoi(s1.substr(N1 - 1 - i, 1).data());
    c = c * c2 + n;
    if (c > 9) {
      n = c / 10;
      c = c - 10 * n;
    } else {
      n = 0;
    }
    if (i >= N1 - 1 && n == 0 && c == 0) break;
    s = to_string(c) + s;
  }
  if (s == "") s = "0";
  return s;
}

class Solution {
 public:
  // 模拟乘法运算规则
  string multiplyOrigin(string num1, string num2) {
    string s = "";
    string s_0 = "";
    if (num1.size() > num2.size()) swap(num1, num2);
    for (int i = num1.size() - 1; i >= 0; i--) {
      if (s == "") {
        s = multiplyOne(num2, num1.substr(i, 1));
      } else {
        string s_t = multiplyOne(num2, num1.substr(i, 1));
        s_0 += "0";
        s_t += s_0;
        s = addstr(s, s_t);
      }
    }
    return s;
  }
  // 直接模型乘法运算
  string multiplyAdvance(string num1, string num2) {
    string S;
    int n1 = num1.size();
    int n2 = num2.size();
    int c = 0, n = 0, k = 0, p = 0;
    int N = n1 + n2;
    int s[N] = {};
    int s2[n2] = {};
    for (int i = n1 - 1; i >= 0; i--) {
      int x = atoi(num1.substr(i, 1).data());
      for (int j = n2 - 1; j >= 0; j--) {
        k = (n1 - 1 - i) + (n2 - 1 - j);
        n = 0;
        if (i == n1 - 1) s2[j] = atoi(num2.substr(j, 1).data());
        c = s[k] + x * s2[j];
        if (c > 9) n = c / 10;
        c = c % 10;
        s[k] = c;
        p = 1;
        while (n > 0) {
          s[k + p] += n;
          n = s[k + p] / 10;
          if (s[k + p] > 9) s[k + p] -= 10;
          p++;
        }
      }
    }
    for (int i = N - 1; i >= 0; i--) {
      if ((S.size() == 0 && s[i] != 0) || S.size() > 0) {
        S.push_back(s[i] + '0');
      }
    }
    if (S == "") S = "0";
    return S;
  }
};

int main() {
  Solution S;
  cout << S.multiplyOrigin("2", "2") << ";" << 2 * 2 << endl;
  cout << S.multiplyOrigin("23", "12") << ";" << 23 * 12 << endl;
  cout << S.multiplyOrigin("123", "9012") << ";" << 123 * 9012 << endl;
  cout << S.multiplyOrigin("890", "2340") << ";" << 890 * 2340 << endl;
  cout << S.multiplyAdvance("23", "20") << ";" << 23 * 20 << endl;
  cout << S.multiplyAdvance("9", "9") << ";" << 123 * 456 << endl;
  cout << "Hello World!" << endl;
}
