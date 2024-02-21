#include<./include/Fib.h>

int main(){
    long long n;
    std::cin >> n;

    Fib f;
    srd::cout << f.result(n) << '\n';

    return 0;
}
