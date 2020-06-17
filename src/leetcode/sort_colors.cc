#include <iostream>
#include <vector>
using namespace std;

class Solution {
 public:
  void sortColors(vector<int>& nums) {
    int n0 = 0, n1 = 0, n2 = 0;
    for (auto it = nums.begin(); it != nums.end(); it++) {
      if (*it == 0) {
        n0++;
      } else if (*it == 1) {
        n1++;
      } else if (*it == 2) {
        n2++;
      }
    }
    n1 = n0 + n1;
    n2 = n1 + n2;
    for (int i = 0; i < nums.size(); i++) {
      if (i < n0) {
        nums[i] = 0;
      } else if (i < n1) {
        nums[i] = 1;
      } else if (i < n2) {
        nums[i] = 2;
      }
    }
  }
};
