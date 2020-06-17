#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

class Solution {
 public:
  int longestValidParentheses(string s) {
    stack<int> d;
    d.push(-1);
    int max_len = 0;
    int k = 0;
    for (auto &it : s) {
      if (it == '(') {
        d.push(k);
      } else {
        d.pop();
        if (d.empty()) {
          d.push(k);
        }
        max_len = max(max_len, k - d.top());
      }
      k++;
    }
    return max_len;
  }
};

int main() {}