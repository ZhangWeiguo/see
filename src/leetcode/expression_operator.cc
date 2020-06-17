#include <iostream>
#include <vector>
using namespace std;

void split(vector<string>& goal, string left_num, vector<int> curr_num,
           vector<char> curr_symbol, int target) {
  if (left_num.size() == 0) {
    int target_pre;
    string target_str;
    for (int i = 0; i < curr_num.size(); i++) {
      if (i == 0) {
        target_pre = curr_num[i];
        target_str += to_string(curr_num[i]);
      } else {
        switch (curr_symbol[i - 1]) {
          case '+':
            target_pre += curr_num[i];
            break;
          case '-':
            target_pre -= curr_num[i];
            break;
          default:
            break;
        }
        target_str += to_string(curr_symbol[i]);
        target_str += to_string(curr_num[i]);
      }
    }
    if (target_pre == target) {
      goal.push_back(target_str);
    }
    return;
  } else {
    vector<int> next_num;
    vector<char> next_symbol;
    string next_left_num;
    for (int i = 1; i < left_num.size(); i++) {
      int end = next_symbol.size() - 1;
      while (next_symbol.size() > 0) {
      }

      next_num = curr_num;
      next_num.push_back(atoi(left_num.substr(0, i).data()));

      next_symbol = curr_symbol;
      next_symbol.push_back('+');
      next_left_num = left_num.substr(i);
      split(goal, next_left_num, next_num, next_symbol, target);

      next_symbol = curr_symbol;
      next_symbol.push_back('-');
      next_left_num = left_num.substr(i);
      split(goal, next_left_num, next_num, next_symbol, target);

      next_symbol = curr_symbol;
      next_symbol.push_back('*');
      next_left_num = left_num.substr(i);
      split(goal, next_left_num, next_num, next_symbol, target);
    }
  }
}

class Solution {
 public:
  vector<string> addOperators(string num, int target) {
    vector<string> goal;
    return goal;
  }
};

int main() { return 0; }