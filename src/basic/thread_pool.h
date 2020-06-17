#pragma once
#include <algorithm>
#include <atomic>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "utils.h"
using namespace std;

namespace basic {

class ThreadPool {
 public:
  ThreadPool() {
    thread_num = GetCPUNum();
    pool.clear();
    stoped.store(false);
    Run();
  }

  ThreadPool(int t_n) {
    thread_num = t_n >= 1 ? t_n : 1;
    pool.clear();
    stoped.store(false);
    Run();
  }

  ~ThreadPool() { Join(); };

  template <class F, class... Args>
  auto AddJob(F&& f, Args&&... args) {
    using retType = decltype(f(args...));
    auto task = make_shared<packaged_task<retType()>>(
        bind(forward<F>(f), forward<Args>(args)...));
    future<retType> call_back = task->get_future();
    {
      lock_guard<mutex> g_lock{lock};
      tasks.emplace([task]() { (*task)(); });
    }
    called.notify_one();
    return call_back;
  }

 private:
  vector<thread> pool;
  queue<function<void()>> tasks;
  mutex lock;
  int thread_num;
  condition_variable called;  // 是否有未处理的任务
  atomic<bool> stoped;        // 是否停止

  void Run() {
    for (int i = 0; i < thread_num; i++) {
      pool.emplace_back(thread([this] {
        while (!this->stoped) {
          function<void()> task;
          {
            unique_lock<mutex> u_lock(this->lock);
            called.wait(u_lock, [this] {
              return this->stoped.load() || !this->tasks.empty();
            });
            if (stoped.load() && tasks.empty()) {
              return;
            }
            task = move(this->tasks.front());
            tasks.pop();
          }
          task();
        }
      }));
    }
  }
  void Join() {
    stoped.store(true);
    called.notify_all();
    for_each(pool.begin(), pool.end(), mem_fn(&thread::join));
  }
};

}  // namespace basic
