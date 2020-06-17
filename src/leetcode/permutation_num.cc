#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
 public:
  void dfs_combine(vector<vector<int>>& all, vector<int>& candidates,
                   vector<int>& curr, int curr_sum, int min_index, int target) {
    if (curr_sum > target) return;
    for (int i = min_index; i < candidates.size(); i++) {
      int tmp = curr_sum + candidates[i];
      if (tmp > target) {
        break;
      }
      auto next = curr;
      next.emplace_back(candidates[i]);
      if (tmp == target) {
        all.emplace_back(next);
        break;
      } else {
        dfs_combine(all, candidates, next, tmp, i, target);
      }
    }
  }
  vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
    sort(candidates.begin(), candidates.end(), less<int>());
    vector<vector<int>> T;
    vector<int> tmp;
    dfs_combine(T, candidates, tmp, 0, 0, target);
    return T;
  }
};