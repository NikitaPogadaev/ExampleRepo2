#pragma once

#include<iostream>

// #include <boost/atomic.hpp>

#include"Fibfunction.h"

namespace fibnum{
    #define x_ first
    #define y_ second

    class FibObj {
    public:
      virtual ~FibObj() {}
      virtual int result(int n) = 0;
    };

    class Fib : public FibObj {

        typedef std::pair<std::pair<int, int>, std::pair<int, int>> matrix;
        int p = 1000000007;


        matrix mult(matrix a, matrix b);

        std::pair<int, int> mult(matrix a, std::pair<int, int> b);

        matrix copyOf(matrix a);

        matrix pow(matrix x, int e);


        std::pair<int, int> sum(std::pair<int, int> a, std::pair<int, int> b);

    public:

        std::pair<int, int> fib(int n);


        int result(int n);


    };

};




class FibComposition {
public:
    FibComposition(int n = 0, fibnum::FibObj* ptr = nullptr): n(n), fib_obj(ptr){}

    int FibNonTrivialCalc();
    int FibRecCalc();

private:
    int n;
    fibnum::FibObj* fib_obj;
};
