
#include<vector>
#include<algorithm>
#include<iostream>
using namespace std;


vector<int> twoSum(vector<int>& nums, int target) {
    vector<int> x(2, 0);
    vector<pair<int, int>> indexs(nums.size());
    for (int i = 0; i < nums.size(); i++) {
        indexs[i] = make_pair(i, nums[i]);
    }
    sort(indexs.begin(), indexs.end(),
        [](pair<int,int> &a, pair<int, int> &b){return a.second < b.second;});
    for (int i = 0; i < nums.size(); i++) {
        cout << indexs[i].first << ":" << indexs[i].second << endl;
    }
    int begin = 0, end = nums.size() - 1;
    while (begin < end) {
        int tmp = indexs[begin].second + indexs[end].second;
        if (tmp == target) {
            x[0] = indexs[begin].first; x[1] = indexs[end].first;
            break;
        } else if (tmp > target) {
            end--;
        } else {
            begin++;
        }
    }
    return x;
}

int main() {
  vector<int> x = {3, 2, 3};
  auto y = twoSum(x, 6);
  for_each(y.begin(), y.end(), [](int x){cout << x << endl;});
  return 0;
}