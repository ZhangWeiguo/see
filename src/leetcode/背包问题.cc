#include<iostream>
#include<vector>
using namespace std;

// 为什么j要从N到1 
// 因为第j行的依赖正上方的数和左上角的数
int max_value(vector<int> &weights, vector<int> &values, int total_content) {
  vector<int> m(total_content + 1, 0);
  int max_values = 0;
  for (int i = 1; i <= weights.size(); i++) {
    for (int j = total_content; j >= 1; j--) {
      int last_j = j - weights[i];
      if (j < weights[i - 1]) {
        m[j] = m[j];
      } else {
        m[j] = max(m[j], m[j-weights[i-1]] + values[i-1]);
      }
      if (i == weights.size() && j == total_content) break;
    }
  }
  return m[total_content];
}

// 可重复
int max_value(vector<int> &weights, vector<int> &values, int total_content) {
  vector<int> m(total_content + 1, 0);
  int max_values = 0;
  for (int i = 1; i <= weights.size(); i++) {
    for (int j = total_content; j >= 1; j--) {
      for (int k = 1; k < j / weights[i - 1]; k++) {
        if (j < weights[i -1] * k) {
          m[j] = m[j];
        } else {
          m[j] = max(m[j], m[j - k * weights[i - 1]] + k * weights[i - 1]);
        }
      }
    }
  }
  return m[total_content];
}

int main() {
  int goods_num, total_content;
  cin >> goods_num >> total_content;
  vector<int> weights(goods_num), values(goods_num);
  for (int i = 0; i < goods_num; i++) {
    cin >> values[i] >> weights[i];
  }
  cout << max_value(weights, values, total_content) << endl;
}