#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

class Solution {
public:
  static void nextPermutationWithRepeat(vector<int>& nums) {
      if (nums.size() <= 1) return;
      int i = nums.size() - 1, j = i - 1;
      while(j >= 0 && nums[j] >= nums[j+1]) j--;
      if (j >= 0) {
          while(nums[i] <= nums[j] && i >= 0) i--;
          if (i != j) swap(nums[i], nums[j]);
      }
      reverse(nums.begin() + j + 1, nums.end());
  }

  static bool nextPermutationWithoutRepeat(vector<int> &nums) {
    int i = nums.size() - 1, j = nums.size() - 2;
    while (nums[j] > nums[j+1] && j >= 0) j--;
    cout << j << endl;
    if (j == -1) return false;
    if (j >= 0) {
      while(nums[i] <= nums[j]) i--;
      swap(nums[i], nums[j]);
    }
    reverse(nums.begin() + j + 1, nums.end());
    return true;
  }
};



int main() {
  vector<int> v = {1, 2, 3};
  for_each(v.begin(), v.end(), [](int it){cout << it << " ";}); cout << endl;
  Solution::nextPermutationWithoutRepeat(v);
  for_each(v.begin(), v.end(), [](int it){cout << it << " ";}); cout << endl;
  return 0;
}