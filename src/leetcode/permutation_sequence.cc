#include <iostream>
#include <set>
#include <string>
#include <vector>
using namespace std;

class Solution {
 public:
  string getPermutation(int n, int k) {
    string s, left;
    long int m = 1;
    int k0 = n, k1 = 0;
    for (int i = 0; i < n; i++) {
      m = m * (i + 1);
      left.push_back((i + 1) + '0');
    }

    while (k0 > 0) {
      m = m / k0;
      if (k % m == 0) {
        k1 = k / m;
      } else {
        k1 = k / m + 1;
      }
      s.push_back(left[k1 - 1]);
      left.erase(k1 - 1, 1);
      k0--;
      k = k - (m * (k1 - 1));
    }
    return s;
  }
};

int main() {
  Solution S;
  S.getPermutation(3, 5);
}