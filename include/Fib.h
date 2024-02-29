#pragma once

#include<iostream>

#include <boost/atomic.hpp>

#include"Fibfunction.h"

namespace fibnum{
    #define x_ first
    #define y_ second
    class Fib {

        typedef std::pair<std::pair<long long, long long>, std::pair<long long, long long>> matrix;
        long long p = 1000000007;


        matrix mult(matrix a, matrix b);

        std::pair<long long, long long> mult(matrix a, std::pair<long long, long long> b);

        matrix copyOf(matrix a);

        matrix pow(matrix x, long long e);


        std::pair<long long, long long> sum(std::pair<long long, long long> a, std::pair<long long, long long> b);

    public:

        std::pair<long long, long long> fib(int n);


        long long result(int n);

        boost::atomic<int> pp{0};

    };

};


class spinlock {
private:
  typedef enum {Locked, Unlocked} LockState;
  boost::atomic<LockState> state_;

public:
  spinlock() : state_(Unlocked) {}

  void lock()
  {
    while (state_.exchange(Locked, boost::memory_order_acquire) == Locked) {
      /* busy-wait */
    }
  }
  void unlock()
  {
    state_.store(Unlocked, boost::memory_order_release);
  }
};

