#include <iostream>
#include <vector>
using namespace std;

void PrintVector(vector<int>* T) {
  for (vector<int>::iterator it = (*T).begin(); it != (*T).end(); it++) {
    cout << (*it) << "  ";
  }
  cout << endl;
}

void Sift(vector<int>* T0, int start, int end) {
  int i, j, k;
  i = start;
  j = (i - start) * 2 + 1 + start;
  k = j + 1;
  int x;
  while (j <= end) {
    if (k <= end) {
      if ((*T0)[j] >= (*T0)[k]) {
        j = k;
      }
    }
    if ((*T0)[i] >= (*T0)[j]) {
      x = (*T0)[i];
      (*T0)[i] = (*T0)[j];
      (*T0)[j] = x;
      i = j;
      j = (i - start) * 2 + 1 + start;
      k = j + 1;
    } else {
      return;
    }
  }
  return;
}

void HeapSort(vector<int>* T) {
  int N = T->size();
  for (int i = N / 2; i >= 0; i--) {
    Sift(T, i, N - 1);
  }
  for (int i = 0; i < N - 1; i++) {
    Sift(T, i, N - 1);
  }
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
  HeapSort(V1);
  PrintVector(V1);
  cout << "End!" << endl;
  return 0;
}
