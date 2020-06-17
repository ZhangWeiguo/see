#include<iostream>
#include<vector>
using namespace std;

// 假设你正在爬楼梯。需要 n 阶你才能到达楼顶。
// 每次你可以爬 1 或 2 个台阶。你有多少种不同的方法可以爬到楼顶

class Solution {
public:
  // 递归方式
  int climbStairs(int n) {
    if (n == 1) return 1;
    else if (n == 2) return 2;
    else return climbStairs(n - 1) + climbStairs(n -2);
  }

  // 递推方式
  int climbStairs(int n) {
    if (n == 1) return 1;
    else if (n == 2) return 2;
    int a = 1, b = 2;;
    int c = 0;
    for (int i = 3; i <= n ; i++) {
      c = a + b;
      a = b;
      b = c;
    }
    return c;
  }

  // 排列组合
  int climbStairsNormal(int n) {
      int x = 0, s = 0;
      for (int i = 0; i <= n / 2; i++) {
          if (i == 0) {
              x = 1;
          } else {
              x = x * (n + 2 - 2 * i) * (n + 1 - 2 * i) / (n + 1 - i) / i;
          }
          s += x;
      }
      return s;
  }
};