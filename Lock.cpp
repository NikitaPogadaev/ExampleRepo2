#pragma once

#include <utility>

template <typename T>
concept MutexObject = requires(T t) {
  t.lock();
  t.unlock();
};

template <MutexObject Mutex>
class LockGuard {
 public:
  explicit LockGuard(Mutex& mutex)
      : mutex_(mutex) {
    mutex_.lock();
  }

  explicit LockGuard(Mutex&& mutex)
      : mutex_(std::forward<Mutex>(mutex)) {
    mutex_.lock();
  }

  LockGuard(const LockGuard&) = delete;
  LockGuard& operator=(const LockGuard&) = delete;

  LockGuard(LockGuard&&) = delete;
  LockGuard& operator=(LockGuard&&) = delete;

  ~LockGuard() {
    mutex_.unlock();
  }

 private:
  Mutex& mutex_;
};
