#include<vector>
#include<iostream>
using namespace std;

int maxArea(vector<int> &height) {
  int begin = 0, end = height.size() - 1, max_area = 0;
  while (begin < end) {
    int tmp_area = (end - begin) * min(height[begin], height[end]);
    max_area = max(max_area, tmp_area);
    if (begin - end == 1) break;
    else {
      if (height[begin] > height[end]) {
        end--;
      } else {
        begin++;
      }
    }
  }
  return max_area;
}


int main() {
  return 0;
}