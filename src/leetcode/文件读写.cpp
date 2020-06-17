#include <fstream>
#include <iostream>
#include <string>
using namespace std;
// ios::app	追加模式。所有写入都追加到文件末尾。
// ios::ate	文件打开后定位到文件末尾。
// ios::in	打开文件用于读取。
// ios::out	打开文件用于写入。
// ios::trunc 如果该文件已经存在，其内容将在打开文件之前被截断，即把文件长度设为
// 0
int read(char filename[], int type) {
  ifstream f;
  f.open(filename, ios::in);

  if (!f) {
    cout << "File Not Exists!" << endl;
    return -1;
  }

  cout << "==================" << endl;
  char S[200];
  string S1;
  switch (type) {
    // 0 逐字，1逐词，2 逐行字符数组，3 逐行字符串
    case 0:
      while (!f.eof()) {
        char x = f.get();
        cout << x;
      }
    case 1:
      while (f >> S) {
        cout << S << endl;
      }
    case 2:
      while (f.getline(S, 200)) {
        cout << S << endl;
      }
    case 3:
      while (getline(f, S1)) {
        cout << S1 << endl;
      }
  };
  cout << endl;
  cout << "==================" << endl;
  f.close();
  return 0;
}

int write(char filename[], string S) {
  ofstream f;
  f.open(filename, ios::in | ios::trunc);
  f << S << endl;
  f.close();
  return 0;
}

int main() {
  int read(char filename[], int type);
  int write(char filename[], string S);
  char filename[] = "git1.txt";
  string S = "Nobody like you and me";
  read(filename, 3);

  // write(filename, S);
}
