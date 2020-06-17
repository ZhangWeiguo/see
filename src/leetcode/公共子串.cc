#include<iostream>
#include<vector>
#include<string>
using namespace std;

int max_common_list(string A, string B) {
  int a[A.size()+1][B.size()+1];
  int max = 0;
  for (int i = 0; i < A.size(); i++) {
  }
}

int max_common_string(string A, string B) {
  int a[A.size()+1][B.size()+1];
  int max = 0;
  for (int i = 0; i < A.size() + 1; i++) {
    for (int j = 0; j < B.size() + 1; j++) {
      if (i == 0 || j == 0) {
        a[i][j] = 0;
        continue;
      }
      if (A[i - 1] == B[j - 1]) {
        a[i][j] = a[i-1][j-1] + 1;
        if (a[i][j] > max) max = a[i][j];
      } else {
        a[i][j] = 0;
      }
    }
  }
  return max;
}

int main() {
  return 0;
}