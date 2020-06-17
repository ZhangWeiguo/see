#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
using namespace std;


class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        sort(nums.begin(), nums.end(), less<int>());
        unordered_map<int, int> value_num;
        unordered_map<int, vector<vector<int>>> T = {};
        unordered_map<int, vector<unordered_map<int, int>>> TS = {};
        int len = nums.size(), last_len = 0;
        for (int i = 0; i < len; i++) {
            if (value_num.count(nums[i]) > 0) value_num[nums[i]]++;
            else value_num[nums[i]] = 1;
        }
        for (int i = 0; i < len; i++) {
          T[i] = vector<vector<int>>();
          TS[i] = vector<unordered_map<int, int>>();
          if (i == 0) {
            for (auto &jt: value_num) {
                T[i].emplace_back(vector<int>{jt.first});
                TS[i].emplace_back(unordered_map<int, int>{{jt.first, 1}});
            }
          } else {
            for (int k = 0; k < T[i - 1].size(); k++) {
              for (auto &jt: value_num) {
                if (TS[i - 1][k][jt.first] >= value_num[jt.first]) continue;
                vector<int> kj = T[i - 1][k];
                unordered_map<int, int> kjs = TS[i - 1][k];
                kj.emplace_back(jt.first);
                kjs[jt.first]++;
                T[i].emplace_back(move(kj));
                TS[i].emplace_back(move(kjs));
              }
            }
            T[i - 1].clear();
            vector<vector<int>>().swap(T[i - 1]);
            TS[i - 1].clear();
            vector<unordered_map<int, int>>().swap(TS[i - 1]);
          }
        }
        return T[len - 1];
    }
};


int main() {
  vector<int> V = {1, 1, 3, 4};
  Solution S;
  auto X = S.permuteUnique(V);
  for(auto &T: X) {
    for_each(T.begin(), T.end(), [](int &a){cout << a << ": ";}); cout << endl;
  }
  return 0;
}