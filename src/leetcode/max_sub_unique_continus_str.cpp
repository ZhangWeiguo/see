#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
using namespace std;

// Given a string, find the length of the longest substring without repeating characters.
// For example, the longest substring without repeating letters for “abcabcbb” is “abc”
// which the length is 3. For “bbbbb” the longest substring is “b”, with the length of 1.

class Solution {
 public:
  int lengthOfLongestSubstring(string s) {
    int n = s.length();
    if (n <= 1) return n;
    unordered_map<char, vector<int> > index_map;
    unordered_map<int, int> move_map;
    for (int i = 0; i < s.length(); i++) {
      char x = s[i];
      if (index_map.count(x) == 0) {
        index_map[x] = {i};
      } else {
        index_map[x].emplace_back(i);
      }
    }
    for (auto &it: index_map) {
      for (int i = 0; i < it.second.size(); i++) {
        if (i == 0) move_map[it.second[0]] = -1;
        else move_map[it.second[i]] = it.second[i - 1];
      }
    }
    for_each(move_map.begin(), move_map.end(), [](pair<const int, int> &it){cout << it.first << ": " << it.second << endl;});
    int begin = 0, max_dis = 1, end = 1;
    while(end < n) {
      int tmp = move_map[end];
      if (tmp >= begin) {
        begin = tmp + 1;
        end = begin + 1;
      } else {
        max_dis = max(max_dis, end - begin + 1);
        end++;
      }
    }
    return max_dis;
  }
};

int main() {
  string s = "zhangweiguo";
  Solution S;
  cout << S.lengthOfLongestSubstring(s) << endl;
}
