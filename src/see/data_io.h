#ifndef DATA_IO_H
#define DATA_IO_H
#include <iostream>
#include <map>
#include <vector>
using namespace std;

namespace see {
enum DataType { INTEGER, DOUBLE, STRING };

class IO {
 public:
  static vector<vector<string>> ReadCSV(string filename);
  static void WriteCSV(string filename, vector<vector<string>> D);
};

}  // namespace see
#endif