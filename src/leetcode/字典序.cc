#include<iostream>
#include<vector>
using namespace std;


// 给定一个整数 n, 返回从 1 到 n 的字典顺序。
// 例如，给定 n =1 3，返回 [1,10,11,12,13,2,3,4,5,6,7,8,9] 。
class Solution {
public:
    void get_sorted(vector<int> &x, int root_value, int max_value) {
        if (root_value <= max_value) x.emplace_back(root_value);
        for (int i = 0; i < 10; i++) {
            int new_value = root_value * 10 + i;
            if (new_value <= max_value) {
                get_sorted(x, new_value, max_value);
            } else {
                break;
            }
        }
    }
    vector<int> lexicalOrder(int n) {
        vector<int> x;
        for (int i = 1; i < 10; i++)
            get_sorted(x, i, n);
        return x;
    }
};


// 给定整数n和m, 将1到n的这n个整数按字典序排列之后, 求其中的第m个数。
// start为根节点数目
// 返回start下面有多少个节点数

int get_start(int start, int max_v) {
  int num = 0;
  while (start <= max_v) {
    if (max_v < start * 10) {
      break;
    } else if (max_v >= start * 10 + 9) {
      num += 10;
      start *= 10;
    } else {
      num += (max_v - start * 10 + 1);
      break;
    }
  }
  return num;
}

int find_kth_value(int max_v, int k) {
  int curr_value = 1, size = max_v, curr_index = 1;
  while(curr_value <= max_v) {
    if (curr_index == k) return curr_value;
    int start = get_start(curr_value, max_v);
    if (k <= curr_index + start) {
      curr_value *= 10;
      curr_index += 1;
    } else {
      curr_value += 1;
      curr_index += (start + 1);
    }
  }
  return curr_value;
}

int main() {
  cout << find_kth_value(11, 4) << endl;
  return 0;
}