#include <iostream>
#include <vector>
using namespace std;

/*
给定一个数组 nums 和一个值 val，你需要原地移除所有数值等于 val
的元素，返回移除后数组的新长度。
不要使用额外的数组空间，你必须在原地修改输入数组并在使用 O(1)
额外空间的条件下完成。
元素的顺序可以改变。你不需要考虑数组中超出新长度后面的元素。
示例 1:
给定 nums = [3,2,2,3], val = 3,
函数应该返回新的长度 2, 并且 nums 中的前两个元素均为 2。
你不需要考虑数组中超出新长度后面的元素。
*/
class Solution {
 public:
  int removeElement(vector<int>& nums, int val) {
    int start = -1, end = -1, i, k, x;
    int N = nums.size();
    k = 0;
    if (N == 0) {
      return 0;
    }
    for (i = 0; i < N; i++) {
      if (nums[i] == val) {
        k++;

        if (start == -1) {
          start = i;
          end = i;
        } else {
          end++;
        }
      } else {
        if (start != -1) {
          x = nums[i];
          nums[i] = nums[start];
          nums[start] = x;
          start++;
          end++;
        }
      }
      if (start + k >= N) {
        break;
      }
    }
    return k;
  }
};

void printVector(vector<int> v) {
  for (int i = 0; i < v.size(); i++) {
    if (i == v.size() - 1) {
      cout << i << endl;
      continue;
    }
    cout << v[i] << ",";
  }
}

int main() {
  int a[5] = {1, 2, 3, 2, 5};
  vector<int> V(a, a + 5);
  Solution S;
  printVector(V);
  int k = S.removeElement(V, 2);
  cout << k << endl;
  printVector(V);
  return 0;
}