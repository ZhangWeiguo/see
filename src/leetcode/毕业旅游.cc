#include<iostream>
#include<vector>
#include<set>
#include<limits.h>
using namespace std;

/*
题目描述
小明目前在做一份毕业旅行的规划。打算从北京出发，分别去若干个城市，然后再回到北京，
每个城市之间均乘坐高铁，且每个城市只去一次。由于经费有限，
希望能够通过合理的路线安排尽可能的省一些路上的花销。
给定一组城市和每对城市之间的火车票的价钱，找到每个城市只访问一次并返回起点的最小车费花销。
输入描述:
城市个数n（1<n≤20，包括北京）

城市间的车票价钱 n行n列的矩阵 m[n][n]
输出描述:
最小车费花销 s
*/


int min_consume_dfs(int **consume, int len, set<int> walked, int index, int curr_consume) {
  int next_consume = -1;
  if (walked.size() == len) return curr_consume + consume[index][0];
  for (int i = 1; i < len; i++) {
    if (walked.count(i) == 0) {
      set<int> new_walked(walked);
      new_walked.insert(i);
      int new_consume = min_consume_dfs(consume, len, new_walked, i, curr_consume + consume[index][i]);
      if (next_consume == -1) {
        next_consume = new_consume;
      } 
      next_consume = min(next_consume, new_consume);
    }
  }
  return next_consume;
}


int min_consume(int **consume, int len) {
  int state_size = 1 << len;
  int all_consume[len][state_size];
  for (int i =0; i < len; i++) {
    all_consume[i][0] = consume[0][i];
  }
  for (int i = 1; i < state_size; i++) {    // i 为已经走过的节点集合
    for (int j = 0; j < len; j++) {         // j 为下一个要走的节点
      all_consume[j][i] = INT_MAX;
      if ((i>>(j-1))&1 == 0) {              // j 不在i的集合中
        for (int k = 0; k < len; k++) {     // k 为已经走过的节点中上一步的节点
          if ((i>>(k-1))&1 == 1) {          // k 在i的集合中
            all_consume[j][i] = min(
              all_consume[j][i],
              all_consume[j][i^(1<<(k-1))] + consume[j][k]);
          }
        }
      }
    }
  }
  return all_consume[0][state_size - 1];
}

void print(int **consume, int I, int J) {
  for (int i = 0; i < I; i++) {
    for (int j = 0; j < J; j++) {
      cout << consume[i][j] << "; ";
    }
    cout << endl;
  }
}

int main() {
  int len, x;
  cin >> len;
  int **consume = new int*[len];
  for (int i = 0; i < len; i++) {
    consume[i] = new int[len];
    for (int j = 0; j < len; j++) {
      cin >> x;
      consume[i][j] = x;
    }  
  }
  // print(consume, len, len);
  cout << min_consume_dfs(consume, len, {0}, 0, 0) << endl;
  for (int i = 0; i < len; i++){
    delete[] consume[i];
  }
  return 0;
}