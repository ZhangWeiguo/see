#include<iostream>
#include<vector>
using namespace std;


int jump(vector<int>& nums) {
  if (nums.size() == 1) return 0;
  int start = 0, end = nums[0], steps = 1;
  while (end < nums.size() - 1) {
    int max_reach = end;
    for (int i = start; i <= end; i++) {
      if (i + nums[i] > max_reach) {
        max_reach = i + nums[i];
      }
    }
    steps++;
    start = end + 1;
    end = max_reach;
  }
  return steps;
}

int main() {
  vector<int> x = {5,6,4,4,6,9,4,4,7,4,4,8,2,6,8,1,5,9,6,5,2,7,9,7,9,6,9,4,1,6,8,8,4,4,2,0,3,8,5};
  cout << jump(x) << endl;
  return 0;
}
