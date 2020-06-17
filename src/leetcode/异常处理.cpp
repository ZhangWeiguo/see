#include <exception>
#include <iostream>
using namespace std;

int test(int x) {
  if (x > 0) {
    return 0;
  } else {
    throw("x<=0 Is Not Promited!");
    return -1;
  }
}

int main() {
  try {
    test(-20);
  } catch (const char* error) {
    cerr << error << endl;
  }
  return 0;
}