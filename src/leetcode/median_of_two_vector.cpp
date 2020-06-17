#include <iostream>
#include <vector>
using namespace std;

class Solution {
 public:
  double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    for (vector<int>::iterator it = nums2.begin(); it != nums2.end(); it++) {
      nums1.push_back(*it);
    }
    int N = nums1.size();
    for (int i = 0; i < N; i++) {
      int m = nums1[i];
      int k = i;
      for (int j = i + 1; j < N; j++) {
        if (nums1[j] < m) {
          m = nums1[j];
          k = j;
        }
      }
      nums1[k] = nums1[i];
      nums1[i] = m;
    }

    if (N % 2 == 0) {
      return (nums1[N / 2 - 1] + nums1[N / 2]) / 2;
    } else {
      return nums1[N / 2];
    }
  }
};

int main() {
  vector<int> T1(10, 99);
  vector<int> T2(20, 33);
  int x;
  Solution S;
  x = S.findMedianSortedArrays(T1, T2);
  cout << x << endl;
  return 0;
}
