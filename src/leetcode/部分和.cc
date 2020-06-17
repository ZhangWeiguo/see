#include<iostream>
#include<vector>
using namespace std;


bool dfs_search(vector<int> T, int sum, int curr_sum, int begin, vector<bool> &index) {
  if (begin >= T.size()) return false;
  if (curr_sum + T[begin] == sum) {
    index[begin] = true;
    return true;
  } else {
    if (dfs_search(T, sum, curr_sum, begin + 1, index)) {
      index[begin] = false;
      return true;
    } else if (dfs_search(T, sum, curr_sum + T[begin], begin + 1, index)) {
      index[begin] = true;
      return true;
    } else {
      return false;
    }
  }
}

bool part_sum(vector<int> T, int sum) {
  vector<bool> index(T.size(), false);
  bool result = dfs_search(T, sum, 0, 0, index);
  if (result) {
    cout << "YES" << endl;
    for (int i = 0; i < T.size(); i++) {
      if (index[i]) cout << i << ":" << T[i] << endl;
    }
  } else {
    cout << "NO" << endl;
  }
  return result;
}

int main() {
  vector<int> T = {1, 2, 4, 7};
  part_sum(T, 13);
  return 0;
}