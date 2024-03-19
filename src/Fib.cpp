#include"../include/Fib.h"

namespace fibnum{
    fibnum::Fib::matrix Fib::mult(fibnum::Fib::matrix a, fibnum::Fib::matrix b){
            fibnum::Fib::matrix c;
            c.y_.y_ = (a.y_.y_ * b.y_.y_ + a.y_.x_ * b.x_.y_) % p;
            c.x_.y_ = (a.x_.y_ * b.y_.y_ + a.x_.x_ * b.x_.y_) % p;
            c.y_.x_ = (a.y_.y_ * b.y_.x_ + a.y_.x_ * b.x_.x_) % p;
            c.x_.x_ = (a.x_.y_ * b.y_.x_ + a.x_.x_ * b.x_.x_) % p;
            return c;
        }

        std::pair<int, int> Fib::mult(fibnum::Fib::matrix a, std::pair<int, int> b){
            std::pair<int, int> c;
            c.y_ = (a.y_.y_ * b.y_ + a.y_.x_ * b.x_) % p;
            c.x_ = (a.x_.y_ * b.y_ + a.x_.x_ * b.x_) % p;
            return c;
        }

        fibnum::Fib::matrix Fib::copyOf(fibnum::Fib::matrix a){
            fibnum::Fib::matrix b;
            b.x_.x_ = a.x_.x_;
            b.x_.y_ = a.x_.y_;
            b.y_.x_ = a.y_.x_;
            b.y_.y_ = a.y_.y_;
            return b;
        }

        fibnum::Fib::matrix Fib::pow(fibnum::Fib::matrix x, int e){
            fibnum::Fib::matrix res = {{1, 0}, {0, 1}};

            while(e > 0){
                if(e % 2){
                    res = mult(res, x);
                }
                e = e >> 1;
                x = mult(x, x);
            }
            return res;
        }


        std::pair<int, int> Fib::sum(std::pair<int, int> a, std::pair<int, int> b){
            std::pair<int, int> c;
            c.y_ = (a.y_ + b.y_) % p;
            c.x_ = (a.x_ + b.x_) % p;
            return c;
        }


        std::pair<int, int> Fib::fib(int n){
            fibnum::Fib::matrix a = pow({{1, 1}, {1, 0}}, n - 1);
            return mult(a, {1, 1});
        }


        int Fib::result(int n){
            fibnum::Fib::matrix a = pow({{1, 1}, {1, 0}}, n - 1);
            return mult(a, {1, 1}).y_;
        }


};

int FibComposition::FibMatrixCalc() {
    return fib_obj->result(n);
}

int FibComposition::FibRecCalc() {
    return fibfunction(n);
}
