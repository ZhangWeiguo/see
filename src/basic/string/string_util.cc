#include "string_util.h"

namespace basic {
void Replace(string& s, string old_s, string new_s) {
  if (s.size() == 0 || old_s.size() == 0) return;
  auto index = s.find(old_s);
  while (index != s.npos) {
    s = s.replace(index, old_s.size(), new_s);
    index = s.find(old_s);
  }
}
}  // namespace basic