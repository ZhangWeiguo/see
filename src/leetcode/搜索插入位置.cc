#include <vector>
using namespace std;


class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int begin = 0, end = nums.size() - 1, mid = 0;
        while (begin < end) {
          if (nums[begin] >= target) return begin;
          if (nums[end] == target) {return end;}
          else if (nums[end] < target) {return end + 1;}
          mid = (begin + end) / 2;
          if (begin == mid) break;
          if (nums[mid] ==  target) return mid;
          else if (nums[mid] > target) {
            end = mid;
          } else {
            begin = mid;
          }
        }
      if (nums[mid] >= target) return mid;
      else return mid + 1;
    }
};