#include <iostream>
#include <vector>
using namespace std;

void PrintVector(vector<int>* T) {
  for (vector<int>::iterator it = (*T).begin(); it != (*T).end(); it++) {
    cout << (*it) << "  ";
  }
  cout << endl;
}

int Partition(vector<int>* T0, int start, int end) {
  int x;
  while (start < end) {
    while (start < end && (*T0)[start] <= (*T0)[end]) {
      end--;
    }
    if (start < end) {
      x = (*T0)[start];
      (*T0)[start] = (*T0)[end];
      (*T0)[end] = x;
      start++;
    }

    while (start < end && (*T0)[start] <= (*T0)[end]) {
      start++;
    }
    if (start < end) {
      x = (*T0)[start];
      (*T0)[start] = (*T0)[end];
      (*T0)[end] = x;
      end--;
    }
  }
  return end;
}

void QuickSort(vector<int>* T, int start, int end) {
  if (end <= 1 || start >= end) {
    return;
  }
  int i;
  i = Partition(T, start, end);
  QuickSort(T, 0, start - 1);
  QuickSort(T, start + 1, end);
};

int main() {
  cout << "Begin!" << endl;
  vector<int>* V1;
  V1 = new vector<int>;

  (*V1).push_back(34);
  (*V1).push_back(5);
  (*V1).push_back(67);
  (*V1).push_back(1);
  (*V1).push_back(51);
  (*V1).push_back(3);
  (*V1).push_back(14);
  (*V1).push_back(95);
  (*V1).push_back(60);
  PrintVector(V1);
  int N = V1->size();
  QuickSort(V1, 0, N);
  PrintVector(V1);
  cout << "End!" << endl;
  return 0;
}
