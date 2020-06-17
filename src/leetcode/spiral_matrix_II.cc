#include <iostream>
#include <vector>
using namespace std;

class Solution {
 public:
  vector<vector<int>> generateMatrix(int n) {
    vector<vector<int>> v(n, vector<int>(n, 0));
    if (n == 0) return v;
    int i = 0, j = 0, k = 0, N = n * n;
    int IU = 1, ID = n - 1, JL = 0, JR = n - 1;
    int direct = 0;
    while (k < N) {
      v[i][j] = ++k;
      if (direct == 0) {
        if (j == JR) {
          direct = 1;
          i++;
          JR--;
        } else {
          j++;
        }
      } else if (direct == 1) {
        if (i == ID) {
          direct = 2;
          j--;
          ID--;
        } else {
          i++;
        }

      } else if (direct == 2) {
        if (j == JL) {
          direct = 3;
          i--;
          JL++;
        } else {
          j--;
        }

      } else if (direct == 3) {
        if (i == IU) {
          direct = 0;
          j++;
          IU++;
        } else {
          i--;
        }
      }
    }
    return v;
  }
};

int main() {
  Solution S;
  auto x = S.generateMatrix(3);
  return 0;
}