#include "file.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

namespace basic {

FileRead::FileRead(string p, bool binary) : File(p) {
  string ph = GetPath();
  if (binary)
    this->file.open(ph, ios::in | ios::binary);
  else
    this->file.open(ph, ios::in);
  if (!file.is_open()) throw "File Not Exists";
}
FileRead::~FileRead() {
  if (file.is_open()) file.close();
}
bool FileRead::Read(string& s) {
  s.clear();
  string st;
  if (file.is_open()) {
    while (!file.eof()) {
      getline(file, st);
      s = s + st + "\n";
    }
    return true;
  }
  return false;
}
bool FileRead::ReadLine(string& s) {
  s.clear();
  if (file.is_open() && !file.eof()) {
    getline(file, s);
    return true;
  }
  return false;
}
bool FileRead::Read(char* s, size_t n) {
  if (file.is_open() && !file.eof()) {
    file.get(s, n);
    return true;
  }
  return false;
}
bool FileRead::ReadLine(char* s, size_t n) {
  if (file.is_open() && !file.eof()) {
    file.getline(s, n, '\n');
    return true;
  }
  return false;
}
long long FileRead::GetSize() {
  long long curr = file.tellg();
  file.seekg(0, ios_base::end);
  long long count = file.tellg();
  file.seekg(curr, ios_base::beg);
  return count;
}
long long FileRead::GetLine() {
  if (GetSize() == 0) return 0;
  long long curr = file.tellg();
  file.seekg(ios_base::beg);
  long long count = 1;
  string c;
  while (ReadLine(c)) count++;
  file.clear();
  file.seekg(curr, ios_base::beg);
  return count;
}
bool FileRead::JumpToSize(long long loc) {
  file.clear();
  if (loc > GetSize()) return false;
  file.seekg(loc, ios_base::beg);
  return true;
}
bool FileRead::JumpToLine(long long line) {
  file.clear();
  int curr = file.tellg();
  file.seekg(0, ios_base::beg);
  int count = 1;
  string c;
  while (true) {
    if (count >= line) break;
    if (!ReadLine(c)) break;
  }
  return true;
}

void FileRead::Close() { file.close(); }

FileWrite::FileWrite(string p, bool binary) : File(p, binary) {
  string ph = GetPath();
  if (binary)
    file.open(ph, ios::out | ios::binary);
  else
    file.open(ph, ios::out);
  if (!file.is_open()) throw "File Path Not Exists";
}
FileWrite::~FileWrite() { file.close(); }
bool FileWrite::Write(string s) {
  if (!file.is_open()) return false;
  file.write(s.c_str(), s.size());
  return true;
}
bool FileWrite::WriteLine(string s) {
  if (!file.is_open()) return false;
  string st = s + "\n";
  file.write(st.c_str(), st.size());
  return true;
}
bool FileWrite::Write(char* s, size_t n) {
  if (!file.is_open()) return false;
  file.write(s, n);
  return true;
}
bool FileWrite::WriteLine(char* s, size_t n) {
  if (!file.is_open()) return false;
  file.write(s, n);
  file.put('\n');
  return true;
}
void FileWrite::Close() { file.close(); }

FileAppend::FileAppend(string p, bool binary) : File(p, binary) {
  string ph = GetPath();
  if (binary)
    file.open(ph, ios::out | ios::app | ios::binary);
  else
    file.open(ph, ios::out | ios::app);
  if (!file.is_open()) throw "File Not Exists";
}
FileAppend::~FileAppend() { file.close(); }
bool FileAppend::Write(string s) {
  if (!file.is_open()) return false;
  file.write(s.c_str(), s.size());
  return true;
}
bool FileAppend::WriteLine(string s) {
  if (!file.is_open()) return false;
  string st = s + "\n";
  file.write(st.c_str(), st.size());
  return true;
}
void FileAppend::Close() { file.close(); }
bool FileAppend::Write(char* s, size_t n) {
  if (!file.is_open()) return false;
  file.write(s, n);
  return true;
}
bool FileAppend::WriteLine(char* s, size_t n) {
  if (!file.is_open()) return false;
  file.write(s, n);
  file.put('\n');
  return true;
}
}  // namespace basic