#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>

#include "dict.h"
using namespace std;

namespace basic {
class ArgValueBase : public ValueBase {
 public:
  virtual void SetValueFromStr(string s) = 0;
};

template <class T>
class ArgValue : public ArgValueBase {
 public:
  ArgValue(T v, string e = "") {
    value = new T;
    explain = new string;
    *value = v;
    *explain = e;
    set = true;
  }
  ArgValue(string e = "") {
    value = new T;
    explain = new string;
    *explain = e;
    set = false;
  }
  ~ArgValue() { delete explain, value; }
  string* GetExplain() const { return explain; }
  T* GetValue() {
    if (IsSet()) return value;
    return NULL;
  }
  void SetValue(T v) {
    *value = v;
    set = true;
  }
  void SetExplain(string e) { *explain = e; }

  bool IsSet() { return set; }

  // 专门为命令行解析的类 s中不能有空格
  void SetValueFromStr(string s) {
    stringstream k;
    k << s;
    k >> *value;
    set = true;
  }

 private:
  T* value;
  string* explain;
  bool set;
};

class ArgParameter {
 public:
  ArgParameter() { para = new map<string, ArgValueBase*>; }
  ~ArgParameter() { delete para; }

  template <class T>
  void AddPara(string s, T v, string e) {
    ArgValue<T>* vv = new ArgValue<T>;
    vv->SetValue(v);
    vv->SetExplain(e);
    (*para)[s] = vv;
  }
  template <class T>
  void AddPara(string s, string e) {
    ArgValue<T>* vv = new ArgValue<T>;
    vv->SetExplain(e);
    (*para)[s] = vv;
  }

  void DelPara(string s) {
    if (para->count(s) > 0) para->erase(s);
  }
  void Clear() { para->clear(); }

  template <class T>
  void SetPara(string s, T v) {
    if (para->count(s) > 0) {
      ArgValue<T>* kv = (ArgValue<T>*)(para->find(s)->second);
      kv->SetValue(v);
      (*para)[s] = kv;
    }
  }
  void SetExplain(string s, string e) {
    if (para->count(s) > 0) {
      auto kv = (para->find(s)->second);
      kv->SetExplain(e);
      (*para)[s] = kv;
    }
  }
  void SetParaFromStr(string s, string v) {
    if (para->count(s) > 0) {
      auto kv = para->find(s)->second;
      kv->SetValueFromStr(v);
      (*para)[s] = kv;
    }
  }

  template <class T>
  T* GetPara(string s) {
    if (para->count(s) > 0) {
      ArgValue<T>* v = (ArgValue<T>*)(para->find(s)->second);
      return v->GetValue();
    }
    return NULL;
  }
  string* GetExplain(string s) {
    if (para->count(s) > 0) {
      return (para->find(s)->second)->GetExplain();
    }
    return NULL;
  }
  vector<string> GetKeys() {
    vector<string> ks;
    for (auto it = para->begin(); it != para->end(); it++) {
      ks.push_back(it->first);
    }
    return ks;
  }
  void Copy(ArgParameter p) {
    para->clear();
    vector<string> ks = p.GetKeys();
    for (int i = 0; i < ks.size(); i++) {
      (*para)[ks[i]] = p.GetBase(ks[i]);
    }
  }
  ArgValueBase* GetBase(string s) {
    if (para->count(s) > 0) {
      return (para->find(s)->second);
    }
    return NULL;
  }

 private:
  map<string, ArgValueBase*>* para;
};

// 命令行参数解析
/*
        cout<< "Test For ArgParser" << endl;
        basic::ArgParser ap;
        ap.Add<string>("name","zhangweiguo","my name");
        ap.Add<double>("age");
        cout<< *(ap.Get<string>("name")) << endl;
        cout<< *(ap.GetExplain("name")) << endl;
        ap.SetExplain("name", "your name");
        cout<< *(ap.GetExplain("name")) << endl;
        cout<< ap.Get<double>("age") << endl;
        ap.Set<double>("age", 12.0);
        ap.SetFromStr("age", "20.3");
        cout<< *(ap.Get<double>("age")) << endl;
        basic::Value<double> v1;
        cout<< v1.IsSet() << endl;
        basic::Value<double> v2(12.3);
        cout<< v2.IsSet() << endl;

        cout<< "Test For Comdline Parse" << endl;
        basic::ArgParser ap;
        ap.Add<string>("str", "string: the name of yours");
        ap.Add<int>("int", "int: the name of yours");
        ap.Add<double>("double", "the name of yours");
        ap.Add<unsigned int>("uint", 12.3, "my age");
        ap.Add<bool>("bool", "has go away?");

                cout<< "str:" << ap.Get<string>("str") << endl;
                cout<< "int:" << ap.Get<string>("int") << endl;
                cout<< "double:" << ap.Get<string>("double") << endl;
                cout<< "uint:" << ap.Get<string>("uint") << endl;
                cout<< "bool:" << ap.Get<string>("bool") << endl;
                cout<< "Is OK:" << ap.Check() << endl;
                ap.Parse(argc, argv);

                if (ap.Check())
                {
                        cout<< "str:" << *(ap.Get<string>("str")) << endl;
                        cout<< "int:" << *(ap.Get<int>("int")) << endl;
                        cout<< "double:" << *(ap.Get<double>("double")) << endl;
                        cout<< "uint:" << *(ap.Get<unsigned int>("uint")) <<
   endl; cout<< "bool:" << *(ap.Get<bool>("bool")) << endl; } else
                {
                        cout<< "Is OK:" << ap.Check() << endl;
                }
*/
class ArgParser : public ArgParameter {
 public:
  ArgParser(){};
  ~ArgParser(){};

  template <class T>
  void Add(string k, T v, string e) {
    AddPara<T>(k, v, e);
  }

  template <class T>
  void Add(string k, string e) {
    AddPara<T>(k, e);
  }

  template <class T>
  void Set(string k, T v) {
    SetPara(k, v);
  }

  void SetFromStr(string k, string v) { SetParaFromStr(k, v); }
  template <class T>
  T* Get(string k) {
    return GetPara<T>(k);
  }

  // 从命令行解析
  void Parse(int argc, char* argv[]) {
    string strtemp;
    string key, value;
    for (int i = 1; i < argc; i++) {
      strtemp = argv[i];
      if (strtemp[0] == '-') {
        if (key != "") {
          SetFromStr(key, "1");
        }
        key = strtemp.substr(1);
        if (i == argc - 1) {
          SetFromStr(key, "1");
        }
      } else {
        if (key != "") {
          SetFromStr(key, strtemp);
          key = "";
        }
      }
    }
  }

  // 检查合法性
  bool Check() {
    vector<string> ks = GetKeys();
    for (auto it = ks.begin(); it != ks.end(); it++) {
      auto v = GetBase(*it);
      if (!v->IsSet()) return false;
    }
    return true;
  }
};

}  // namespace basic
