#include<functional>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
using namespace std;

template <class T>
void print(const vector<T> &L) {
  for (auto x: L){
    cout << x << ";";
  }
  cout << endl;
};

// 冒泡排
template <class T>
void sort(vector<T> &L) {
  for (long int i = 0; i < L.size() - 1; i++) {
    for (long int j = i + 1; j < L.size(); j++) {
      if (L[i] > L[j]) {
        std::swap(L[i], L[j]);
      }
    }
  }
};

// 快排
template <class T>
void sort_quick(vector<T> &L, int begin, int end) {
  if (end <= begin) return;
  int b = begin, e = end, m = begin;
  T mid = L[begin];
  while (b < e) {
    while (L[e] >= mid && b < e) e--;
    while (L[b] <= mid && b < e) b++;
    if (b < e) {
      swap(L[e], L[b]);
    } else {
      swap(L[m], L[b]);
      m = b;
    }
  }
  sort_quick<T>(L, begin, m - 1);
  sort_quick<T>(L, m + 1, end);
}

template <class T>
void sift(vector<T> &L, size_t begin, size_t end) {
  size_t b = begin;
  size_t m, left, right;
  while (b < end) {
    left = 2 * b + 1, right = 2 * b + 2;
    if (left < end) {
      if (right < end && L[right] > L[left]) {
        m = right;
      } else {
        m = left;
      }
      if (L[b] < L[m]) {
        swap(L[b], L[m]);
      } else {
        break;
      }
    }
    b = left;
  }
}

// 堆排序
template <class T>
void sort_heap(vector<T> &L) {
  size_t len = L.size(), mid = L.size() / 2;
  if (len == 1) return;
  for (int i = mid; i >= 0; i--) sift(L, i, len);
  for (int i = len - 1; i >= 0; i--) {
    print<T>(L);
    swap(L[i], L[0]);
    sift<T>(L, 0, i);
  }
}

template <class T>
void merge(vector<T> &L, size_t begin1, size_t begin2, size_t end) {
  size_t L1 = begin2 - begin1, L2 = end - begin2 + 1;
  vector<T> nL;
  nL.reserve(L1 + L2);
  for (size_t i = 0, j = 0; i < L1 || j < L2;) {
    size_t index1 = begin1 + i, index2 = begin2 + j;
    if (index1 <= begin2 - 1) {
      if (index2 <= end && L[index1] > L[index2]) {
        nL.emplace_back(L[index2]);
        j++;
      } else {
        nL.emplace_back(L[index1]);
        i++;
      }
    } else if (index2 <= end) {
      nL.emplace_back(L[index2]);
      j++;
    }
  }
  for (int i = 0; i < L1 + L2; i ++) {
    L[begin1 + i] = nL[i];
  }
}


// 归并排序
template <class T>
void sort_merge(vector<T> &L, size_t begin, size_t end) {
  cout << "begin:" << begin << ":" << end << endl;
  size_t len = end - begin + 1;
  size_t mid = begin + len / 2 - 1;
  if (len <= 1) return;
  sort_merge<T>(L, begin, mid);
  sort_merge<T>(L, mid + 1, end);
  merge<T>(L, begin, mid + 1, end);
  print<T>(L);
}

// 二分搜索
template <class T>
int binary_search(vector<T> L, T s) {
      int begin = 0, end = L.size() - 1;
      while (end >= begin) {
        int mid = begin + (end - begin) / 2;
        if (L[mid] == s) return mid;
        else if (L[mid] < s) {
          begin = mid + 1;
        } else {
          end = mid - 1;
        }
      }
      return -1;
}

void test_sort() {
  vector<int> a = {1, -3, 4, 2, 5, 8, 7};
  print<int>(a);
  // sort<int>(a);
  // sort_quick<int>(a, 0, a.size() - 1);
  // sort_heap<int>(a);
  sort_merge<int>(a, 0, a.size() - 1);
  print<int>(a);
  int index = binary_search<int>(a, 7);
  cout << index << endl;
}


int main() {
  // cout << kmp("iamyour", "amy") << endl;
  vector<int> a = {5, 7, 9, 10, -3};
  sort(a);
  print<int>(a);
  return 0;
}