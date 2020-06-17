#include <iostream>
#include <string>
#include <vector>
using namespace std;
/*
 The string "PAYPALISHIRING" is written in a zigzag pattern on a given number of
rows like this: (you may want to display this pattern in a fixed font for better
legibility)

P   A   H   N
A P L S I I G
Y   I   R

And then read line by line: "PAHNAPLSIIGYIR"
Write the code that will take a string and make this conversion given a number
of rows: string convert(string text, int nRows); convert("PAYPALISHIRING", 3)
should return "PAHNAPLSIIGYIR".
*/

class Solution {
 public:
  string convert(string s, int num) {
    string S1;
    vector<char>* V = new vector<char>[num];
    for (int i = 0; i < num; i++) {
      vector<char> v1;
      V[i] = v1;
    }
    int N = s.size();
    int n = 2 * num - 2;
    if (N == 1 || num == 1) {
      return s;
    }
    int j;
    for (int i = 0; i < N; i++) {
      int k = (i + 1) % n;
      if (k > num) {
        k = 2 * num - k - 1;
      } else if (k >= 1 && k <= num) {
        k--;
      } else {
        k = 1;
      }
      cout << k << N << endl;
      V[k].push_back(s[i]);
    }
    cout << 12334 << endl;
    for (int i = 0; i < num; i++) {
      for (vector<char>::iterator it = V[i].begin(); it != V[i].end(); it++) {
        S1 += (*it);
      }
    }
    return S1;
  }
};


string convert(string s, int numRows) {
    if (numRows == 1) return s;
    vector<string> ss(numRows, "");
    int index = 0;
    bool add = true;
    for (int i = 0; i < s.size(); i++) {
        ss[index].push_back(s[i]);
        if (index == numRows - 1 && add) {
            add = false;
        } else if (index == 0 && !add) {
            add = true;
        }
        if (add) index++;
        else index--;
    }
    string fs;
    for (auto &it: ss) {
        fs.append(it);
    }
    return fs;  
}

int main() {
  Solution S;
  string s = "ABCDE";
  int num = 4;
  string s1 = S.convert(s, num);
  cout << s1 << endl;
  return 0;
}
