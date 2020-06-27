#pragma once
#include <string>
#include <vector>

namespace basic {
using namespace std;

bool SplitStr(const string &str, string &deli, vector<string> *strs);

bool MergeStr(const vector<string> &strs, string deli, string *str);

} // namespace basic


