#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
using namespace std;

class Solution {
 public:
  string fractionToDecimal(int num, int den) {
    bool neg = (num < 0) ^ (den < 0) ? true : false;
    long long int n = num;
    long long int d = den;
    n = abs(n);
    d = abs(d);
    long long int tmp = n / d;
    string res = "";
    if (neg && n != 0) res += "-";
    res += to_string(tmp);
    n %= d;
    if (n == 0) {
      return res;
    }
    res += ".";
    bool repeat = false;
    unordered_map<long long int, int> mp;
    while (n != 0 && !repeat) {
      if (mp.find(n) == mp.end()) {
        mp[n] = res.size();
      } else {
        repeat = true;
        res.push_back(')');
        res.insert(mp[n], "(");
        break;
      }
      n = n * 10;
      res += to_string(n / d);
      n %= d;
    }
    return res;
  }
};

int main() {
  Solution S;
  cout << S.fractionToDecimal(5, 4) << endl;
}