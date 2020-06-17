#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
/*
    // basic::FileRead f("utils.h");
    // string s;
    // f.Read(s);
    // cout << s.size() << endl;
    // cout << s << endl;
    // f.Close();

    // basic::FileAppend f("text");
    // f.Write("This is a story about\nMy mother;\n");
    // f.WriteLine("Let me talk it");
    // f.WriteLine("Let you ");
*/
namespace basic {
class File {
 public:
  File(string p, bool binary = true) : path(p) {}
  ~File() {}
  bool Read(string& s);
  bool ReadLine(string& s);
  bool Write(string s);
  bool WriteLine(string s);
  bool Read(char* s, size_t n);
  bool ReadLine(char* s, size_t n);
  bool Write(char* s, size_t n);
  bool WriteLine(char* s, size_t n);
  void Close();

 protected:
  string GetPath() { return path; }
  void SetPath(string p) { path = p; }

 private:
  string path;
};

class FileRead : public File {
 public:
  FileRead(string p, bool binary = true);
  ~FileRead();
  bool Read(string& s);
  bool ReadLine(string& s);
  bool Read(char* s, size_t n);
  bool ReadLine(char* s, size_t n);
  long long GetSize();
  long long GetLine();  //低性能
  bool JumpToSize(long long loc);
  bool JumpToLine(long long line);  //低性能
  void Close();

 private:
  ifstream file;
};

class FileWrite : public File {
 public:
  FileWrite(string p, bool binary = true);
  ~FileWrite();
  bool Write(string s);
  bool WriteLine(string s);
  bool Write(char* s, size_t n);
  bool WriteLine(char* s, size_t n);
  void Close();

 private:
  ofstream file;
};

class FileAppend : public File {
 public:
  FileAppend(string p, bool binary = true);
  ~FileAppend();
  bool Write(string s);
  bool WriteLine(string s);
  bool Write(char* s, size_t n);
  bool WriteLine(char* s, size_t n);
  void Close();

 private:
  ofstream file;
};
}  // namespace basic