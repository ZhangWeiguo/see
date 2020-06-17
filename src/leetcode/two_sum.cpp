#include <iostream>
#include <vector>
using namespace std;

template <class T>
class Solution {
 public:
  vector<T> twoSum(vector<T> nums, T target) {
    int N;
    N = nums.size();
    vector<T> V1;
    for (int i = 0; i < N; i++) {
      for (int j = i + 1; j < N; j++) {
        if (nums[i] + nums[j] == target) {
          V1.push_back(i);
          V1.push_back(j);
          return V1;
        }
      }
    }
    return V1;
  }
};

int main() {
  Solution<int> S;
  vector<int> v1, v2;
  v1.push_back(1);
  v1.push_back(4);
  v1.push_back(10);
  v2 = S.twoSum(v1, 14);
  for (vector<int>::iterator iter = v2.begin(); iter != v2.end(); iter++) {
    cout << *iter << endl;
  }
  return 0;
}
