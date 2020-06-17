#include "data_io.h"

#include <iostream>

#include "../basic/file.h"
#include "../basic/utils.h"
using namespace std;

namespace see {

vector<vector<string>> IO::ReadCSV(string filename) {
  vector<vector<string>> D;
  basic::FileRead F(filename);
  string line;
  vector<string> v;
  while (F.ReadLine(line)) {
    v = basic::SplitStr(line, ",");
    D.push_back(v);
  }
  F.Close();
  return D;
}

void IO::WriteCSV(string filename, vector<vector<string>> D) {
  basic::FileWrite F(filename);
  string line;
  for (auto it = D.begin(); it != D.end(); it++) {
    line = basic::MergeStr(*it);
    F.WriteLine(line);
  }
  F.Close();
}

}  // namespace see
