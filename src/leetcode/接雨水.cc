#include <vector>
using namespace std;

class Solution {
public:
    int trap(vector<int>& height) {
      if (height.size() <= 1) return 0;
      vector<int> after_max = height;
      int area = 0, begin = 1, end = height.size() - 1;
      int curr_max = 0;
      for (int i = end - 1; i >= 0; i--) {
        after_max[i] = max(after_max[i], after_max[i + 1]);
      }
      int curr_height = height[0], curr_begin = 0, tmp_reduce = 0;
      while (begin <= end) {
        if (height[begin] >= curr_height || height[begin] == after_max[begin]) {
          area += ((begin - curr_begin - 1) * min(curr_height, height[begin]) - tmp_reduce);
          curr_begin = begin;
          begin++;
          tmp_reduce = 0;
          curr_height = height[curr_begin];
        } else {
          tmp_reduce += height[begin];
          begin++;
        }
      }
      return area;
    }
};