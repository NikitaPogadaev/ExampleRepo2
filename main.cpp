#include "include/Fib.h"
#include "include/Fibfunction.h"

int main(){
    long long n;
    std::cin >> n;

    fibnum::Fib f;
    std::cout << f.result(n) << '\n';
    std::cout << fibfunction(n) << '\n';

    FibComposition ff(n, &f);

    std::cout << ff.FibNonTrivialCalc() << '\n';
    std::cout << ff.FibRecCalc() << '\n';

    return 0;
}
