#include <ctime>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct People {
  string name;
  int age;
};

void swap(int& a, int& b) {
  int c;
  c = a;
  a = b;
  b = c;
}
void swap_copy(int* a, int* b) {
  int c;
  c = *a;
  *a = *b;
  *b = c;
}

void test_exchange() {
  int a = 10;
  int b = 12;
  swap(a, b);
  cout << "a=" << a << endl;
  cout << "b=" << b << endl;
  swap_copy(&a, &b);
  cout << "a=" << a << endl;
  cout << "b=" << b << endl;
}

void test_time() {
  time_t now;
  now = time(0);
  cout << sizeof(now) << endl;
  cout << now << endl;
  cout << 1900 + localtime(&now)->tm_year << endl;
  char tmp[64];
  cout << strftime(tmp, sizeof(tmp), "%Y-%m-%d", localtime(&now));
}

int main() {
  People p1;
  p1.name = "zhangweiguo";
  p1.age = 14;
  cout << sizeof(p1) << "   " << p1.name << endl;
  return 0;
}