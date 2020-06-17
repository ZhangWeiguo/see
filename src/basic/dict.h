#pragma once
#include <cxxabi.h>
#include <stdarg.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

/* 如果你的抽象类的派生类有任何一个纯虚函数没有实现
那么不管其他做的再多，这个类仍然是一个抽象类
抽象类不能被实例化 */
class ValueBase {
 public:
  virtual ~ValueBase(){};
  virtual string* GetExplain() const = 0;
  virtual void SetExplain(string e) = 0;
  virtual bool IsSet() = 0;
};

template <class T>
class Value : public ValueBase {
 public:
  Value(T v, string e = "") {
    value = new T;
    explain = new string;
    *value = v;
    *explain = e;
    set = true;
  }
  Value(string e = "") {
    value = new T;
    explain = new string;
    *explain = e;
    set = false;
  }
  ~Value() { delete explain, value; }
  string* GetExplain() const { return explain; }
  T* GetValue() { return value; }
  void SetValue(T v) {
    *value = v;
    set = true;
  }
  void SetExplain(string e) { *explain = e; }

  bool IsSet() { return set; }

 private:
  T* value;
  string* explain;
  bool set;
};

// 自定义字典, k 是string, v 是任意自定义类型
/*
    basic::Dict p;
    p.Add<double>("age", 12.3, "age of me");
    p.Add<string>("name", "zhangweiguo", "name of me");
    cout<<*(p.Get<double>("age"))<<endl;
    cout<<*(p.GetExplain("age"))<<endl;
    cout<<*(p.Get<string>("name"))<<endl;
    cout<<*(p.GetExplain("name"))<<endl;
    p.Set<string>("name","dog");
    cout<<*(p.Get<string>("name"))<<endl;
    cout<< "After Del Key" << endl;
    p.Del("name"); // p.Clear();
    string *ss = p.GetExplain("name");
    cout<<ss<<endl;
*/
class Dict {
 public:
  Dict() { para = new map<string, ValueBase*>; }
  ~Dict() { delete para; }

  template <class T>
  void Add(string s, T v, string e) {
    Value<T>* vv = new Value<T>;
    vv->SetValue(v);
    vv->SetExplain(e);
    (*para)[s] = vv;
  }
  template <class T>
  void Add(string s, string e) {
    Value<T>* vv = new Value<T>;
    vv->SetExplain(e);
    (*para)[s] = vv;
  }

  void Del(string s) {
    if (para->count(s) > 0) para->erase(s);
  }
  void Clear() { para->clear(); }

  template <class T>
  void Set(string s, T v) {
    if (para->count(s) > 0) {
      Value<T>* kv = (Value<T>*)(para->find(s)->second);
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

  template <class T>
  T* Get(string s) {
    if (para->count(s) > 0) {
      Value<T>* v = (Value<T>*)(para->find(s)->second);
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
  void Copy(Dict p) {
    para->clear();
    vector<string> ks = p.GetKeys();
    for (int i = 0; i < ks.size(); i++) {
      (*para)[ks[i]] = p.GetBase(ks[i]);
    }
  }
  ValueBase* GetBase(string s) {
    if (para->count(s) > 0) {
      return (para->find(s)->second);
    }
    return NULL;
  }

 private:
  map<string, ValueBase*>* para;
};