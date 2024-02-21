#include"../include/Fib.h"

namespace fibnum{
    matrix Fib::mult(matrix a, matrix b){
            matrix c;
            c.y_.y_ = (a.y_.y_ * b.y_.y_ + a.y_.x_ * b.x_.y_) % p;
            c.x_.y_ = (a.x_.y_ * b.y_.y_ + a.x_.x_ * b.x_.y_) % p;
            c.y_.x_ = (a.y_.y_ * b.y_.x_ + a.y_.x_ * b.x_.x_) % p;
            c.x_.x_ = (a.x_.y_ * b.y_.x_ + a.x_.x_ * b.x_.x_) % p;
            return c;
        }

        std::pair<long long, long long> Fib::mult(matrix a, std::pair<long long, long long> b){
            std::pair<long long, long long> c;
            c.y_ = (a.y_.y_ * b.y_ + a.y_.x_ * b.x_) % p;
            c.x_ = (a.x_.y_ * b.y_ + a.x_.x_ * b.x_) % p;
            return c;
        }

        matrix Fib::copyOf(matrix a){
            matrix b;
            b.x_.x_ = a.x_.x_;
            b.x_.y_ = a.x_.y_;
            b.y_.x_ = a.y_.x_;
            b.y_.y_ = a.y_.y_;
            return b;
        }

        matrix Fib::pow(matrix x, long long e){
            matrix res = {{1, 0}, {0, 1}};

            while(e > 0){
                if(e % 2){
                    res = mult(res, x);
                }
                e = e >> 1;
                x = mult(x, x);
            }
            return res;
        }


        std::pair<long long, long long> Fib::sum(std::pair<long long, long long> a, std::pair<long long, long long> b){
            std::pair<long long, long long> c;
            c.y_ = (a.y_ + b.y_) % p;
            c.x_ = (a.x_ + b.x_) % p;
            return c;
        }


        std::pair<long long, long long> Fib::fib(int n){
            matrix a = pow({{1, 1}, {1, 0}}, n - 1);
            return mult(a, {1, 1});
        }


        long long Fib::result(int n){
            matrix a = pow({{1, 1}, {1, 0}}, n - 1);
            return mult(a, {1, 1}).y_;
        }


};
