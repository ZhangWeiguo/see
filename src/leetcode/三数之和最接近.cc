#include<iostream>
#include<vector>
#include<algorithm>
#include<math.h>
using namespace std;

// 给定一个包括 n 个整数的数组 nums 和 一个目标值 target。
// 找出 nums 中的三个整数，使得它们的和与 target 最接近。返回这三个数的和。
// 假定每组输入只存在唯一答案。

int threeSumClosest(vector<int>& nums, int target) {
    sort(nums.begin(), nums.end(), less<int>());
    int min_dis = 1e8, max_sum = 0;
    for(int i = 0; i < nums.size() - 2; i++) {
      int first = nums[i];
      if (i != 0 && first == nums[i - 1]) continue;
      int begin = i + 1, end = nums.size() - 1;
      while (begin < end) {
        int dis = nums[i] + nums[begin] + nums[end] - target;
        int err = abs(dis);
        if (err < min_dis) {
          min_dis = err;
          max_sum = nums[i] + nums[begin] + nums[end];
        }
        if (dis == 0) return target;
        else if (dis < 0) {
          begin++;
        } else {
          end--;
        }
      }
    }
    return max_sum;
}

int main() {
  vector<int> T = {-1,2,1,-4};
  cout << threeSumClosest(T, 1) << endl;
  return 0;
}