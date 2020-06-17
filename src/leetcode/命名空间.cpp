#include <iostream>
using namespace std;

namespace MySpace1 {
int N = 100;
class people {
 private:
  string name;
  int age;

 public:
  people() {}
  ~people() {}
};
}  // namespace MySpace1

int main() { cout << MySpace1::N << endl; }
