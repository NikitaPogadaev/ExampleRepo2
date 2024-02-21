#include<iostream>



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

    };

};

int main() {
    fibnum::Fib f;
    std::cout << f.result(4);
}
