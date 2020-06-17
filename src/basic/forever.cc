#include "forever.h"

#include <ctime>
#include <iostream>

namespace basic {

long long Time::NowNano() {
  auto n = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());
  return n.count();
}

long long Time::NowMir() {
  auto n = duration_cast<microseconds>(system_clock::now().time_since_epoch());
  return n.count();
}

long long Time::NowMil() {
  auto n = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
  return n.count();
}

long long Time::NowSec() {
  auto n = duration_cast<seconds>(system_clock::now().time_since_epoch());
  return n.count();
}

string Time::Unixtime2Str(long long t, string format) {
  string s;
  if (format.size() == 0) return s;
  seconds t0(t);
  time_point<system_clock, duration<int>> tp_seconds(t0);
  auto t1 = system_clock::to_time_t(tp_seconds);
  auto t2 = localtime(&t1);
  char* d = new char;
  strftime(d, 1024, format.data(), t2);
  s = string(d);
  delete d;
  return s;
}

void Replace(string& s, string old_s, string new_s) {
  if (s.size() == 0 || old_s.size() == 0) return;
  auto index = s.find(old_s);
  while (index != s.npos) {
    s = s.replace(index, old_s.size(), new_s);
    index = s.find(old_s);
  }
}

long long Time::Str2Unixtime(string s, string format) {
  tm t0;
  int year = 0, month = 0, mday = 0, hour = 0, min = 0, sec = 0;
  FormatTime(s, format, year, month, mday, hour, min, sec);
  t0.tm_hour = hour;
  t0.tm_sec = sec;
  t0.tm_min = min;
  t0.tm_hour = hour;
  t0.tm_year = year - 1900;
  t0.tm_mday = mday;
  t0.tm_mon = month - 1;
  auto t1 = mktime(&t0);
  auto t2 = system_clock::from_time_t(t1);
  return duration_cast<seconds>(t2.time_since_epoch()).count();
}

// %Y %m %d %H %M %S 只有这几个
// "2019-04-04 11:44:56", "%Y-%m-%d %H:%M:%S"
void FormatTime(string s, string format, int& year, int& month, int& mday,
                int& hour, int& min, int& sec) {
  int i = 0, k = 0;
  int index[6], val[6];
  string tmp;
  char last = '$';
  for (int i = 0; i < format.size(); i++) {
    if (last == '%') {
      switch (format[i]) {
        case 'Y':
          index[k++] = 0;
          break;
        case 'm':
          index[k++] = 1;
          break;
        case 'd':
          index[k++] = 2;
          break;
        case 'H':
          index[k++] = 3;
          break;
        case 'M':
          index[k++] = 4;
          break;
        case 'S':
          index[k++] = 5;
          break;
        default:
          break;
      }
    }
    last = format[i];
  }
  k = 0;
  for (int i = 0; i < s.size(); i++) {
    if (s[i] >= '0' && s[i] <= '9') {
      tmp.push_back(s[i]);
    } else {
      if (tmp.length() > 0) {
        val[k++] = atoi(tmp.data());
        tmp.clear();
      }
    }
    if (i == s.size() - 1 && tmp.length() > 0) {
      val[k++] = atoi(tmp.data());
      tmp.clear();
    }
  }
  year = val[index[0]];
  month = val[index[1]];
  mday = val[index[2]];
  hour = val[index[3]];
  min = val[index[4]];
  sec = val[index[5]];
}

Clock::Clock() {
  start_time = (long double)(Time::NowMil()) / 1000;
  status = 0;
}

Clock::~Clock() {}

void Clock::Start() {
  if (status != 0 && status != 3) return;
  status = 1;
  auto t0 = (long double)(Time::NowMil()) / 1000;
  start_time = t0;
  stop_time = t0;
  all_sleep_time = 0;
}

void Clock::Stop() {
  if (status == 0 || status == 3) return;
  status = 3;
  stop_time = (long double)(Time::NowMil()) / 1000;
}

void Clock::Pause() {
  if (status != 1) return;
  status = 2;
  auto t0 = (long double)(Time::NowMil()) / 1000;
  stop_time = t0;
  sleep_start_time = t0;
}

void Clock::Continue() {
  if (status != 2) return;
  status = 1;
  auto t0 = (long double)(Time::NowMil()) / 1000;
  all_sleep_time += (t0 - sleep_start_time);
}

double Clock::GetTime() {
  if (status == 1) {
    stop_time = (long double)(Time::NowMil()) / 1000;
  }
  return stop_time - start_time - all_sleep_time;
}

}  // namespace basic
