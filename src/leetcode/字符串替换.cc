#include<iostream>
#include<string>
#include<vector>
using namespace std;

// 给定一个仅由小写字母x和y组成且长度不超过105的字符串
// 每次可以将字符串中的一个子串xy替换成字符串yyx
// 那么至少要替换多少次才能让字符串中不存在子串xy

int replace(string old) {
  int y_num = 0;
  int replace_num = 0;
  for (int i = 0; i < old.size(); i++) {
    if (old[i] == 'x') {
      y_num = ((y_num << 1) + 1) % 1000000007;
    } else {
      replace_num = (replace_num + y_num) % 1000000007;
    }
  }
  return replace_num;
}

