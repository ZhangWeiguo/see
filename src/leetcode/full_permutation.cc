#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
using namespace std;


class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        sort(nums.begin(), nums.end(), less<int>());
        unordered_map<int, vector<vector<int>>> T = {};
        unordered_map<int, vector<unordered_set<int>>> TS = {};
        int len = nums.size(), last_len = 0;
        for (int i = 0; i < len; i++) {
          if (i == 0) last_len = 1;
          else last_len = T[i-1].size();
          T[i] = vector<vector<int>>(last_len * (len - i));
          TS[i] = vector<unordered_set<int>>(last_len * (len - i));
          if (i == 0) {
            for (int j = 0; j < len; j++) {
              T[i][j] = vector<int>{nums[j]};
              TS[i][j] = unordered_set<int>{nums[j]};
            }
          } else {
            int index = 0;
            for (int k = 0; k < T[i - 1].size(); k++) {
              for (int j = 0; j < len; j++) {
                if (TS[i - 1][k].count(nums[j]) > 0) continue;
                vector<int> kj = T[i - 1][k];
                unordered_set<int> kjs = TS[i - 1][k];
                kj.emplace_back(nums[j]);
                kjs.insert(nums[j]);
                T[i][index] = kj;
                TS[i][index] = kjs;
                index++;
              }
            }
            T[i - 1].clear();
            vector<vector<int>>().swap(T[i - 1]);
            TS[i - 1].clear();
            vector<unordered_set<int>>().swap(TS[i - 1]);
          }
        }
        return T[len - 1];
    }
};


int main() {
  vector<int> V = {1, 2, 3, 4};
  Solution S;
  auto X = S.permute(V);
  for(auto &T: X) {
    for_each(T.begin(), T.end(), [](int &a){cout << a << ": ";}); cout << endl;
  }
  return 0;
}