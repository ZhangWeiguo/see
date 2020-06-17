#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class Solution {
 public:
  int maxChunksToSorted(vector<int>& v) {
    int min_value, max_value, n = 1;
    max_value = v[0];
    for (auto it = v.begin(); it != v.end() - 1; it++) {
      min_value = *(min_element(it + 1, v.end()));
      max_value = max(max_value, *it);
      if (max_value <= min_value) {
        n++;
      }
    }
    return n;
  }
};

int main() {
  vector<int> v = {2, 0, 1};
  Solution S;
  cout << S.maxChunksToSorted(v) << endl;
  return 0;
}