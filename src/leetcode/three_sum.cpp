#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
/*
Given an array S of n integers, are there elements a, b, c in S such that a + b
+ c = 0? Find all unique triplets in the array which gives the sum of zero.
Note: The solution set must not contain duplicate triplets.
For example, given array S = [-1, 0, 1, 2, -1, -4],
A solution set is:
[
  [-1, 0, 1],
  [-1, -1, 2]
]
*/
class Solution {
 public:
  vector<vector<int>> threeSum(vector<int>& nums) {
    // sort
    vector<vector<int>> VV;
    int N = nums.size();
    sort(nums.begin(), nums.end(), less<int>());
    for (int k = 0; k < N; k++) {
      int a0 = nums[k];
      bool exist = false;
      if (k != 0 && a0 == nums[k - 1]) continue;
      int i = k + 1;
      int j = N - 1;
      while (i < j) {
        if (i > k + 1 && nums[i] == nums[i - 1]) {
          i++;
          continue;
        }
        int a1 = nums[i] + nums[j];
        if (a1 == -a0) {
          VV.emplace_back(vector<int>{a0, nums[i], nums[j]});
          i++;
          j--;
        } else if (a1 > -a0) {
          j--;
        } else if (a1 < -a0) {
          i++;
        }
      }
    }
    return VV;
  }
};

int main() {
  vector<int> V1;
  V1.push_back(-1);
  V1.push_back(0);
  V1.push_back(1);
  V1.push_back(2);
  V1.push_back(-1);
  V1.push_back(4);
  Solution S;
  vector<vector<int>> V2;
  V2 = S.threeSum(V1);
  cout << V2.size() << endl;
  return 0;
}
