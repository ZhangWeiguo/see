#include <iostream>
#include <vector>
using namespace std;

class Solution {
 public:
  vector<int> spiralOrder(vector<vector<int>>& matrix) {
    vector<int> v;
    const int n = matrix.size();
    if (n == 0) return v;
    const int m = matrix[0].size();
    int i = 0, j = 0, k = 0, N = n * m;
    int IU = 1, ID = n - 1, JL = 0, JR = m - 1;
    int direct = 0;
    while (k < n * m) {
      v.push_back(matrix[i][j]);
      cout << i << ";" << j << endl;
      k++;
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
  vector<vector<int>> V = {
      {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
  auto x = S.spiralOrder(V);
  return 0;
}