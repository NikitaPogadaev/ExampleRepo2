#include "include/Fib.h"
#include "include/Fibfunction.h"
// #include <spdlog/spdlog.h>

int main(){
    long long n;
    std::cin >> n;

    fibnum::Fib f;
    std::cout << f.result(n) << '\n';
    std::cout << fibfunction(n) << '\n';
    // spdlog::info("Status code");
    // spdlog::info(f.result(n) == fibfunction(n));

    return 0;
}
