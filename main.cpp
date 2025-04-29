#include "include/Fib.h"
#include "include/Fibfunction.h"
#include <fstream>

int main(int argc, char* argv[]){
    if(argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <number>" << std::endl;
        return 1;
    }

    long long n = (long long)(std::atoi(argv[1]));
    n = (long long)(std::atoi(argv[1]));
    std::ofstream fout("./bd/fib_nums.txt", std::ios_base::app);

    fibnum::Fib f;
    fout << f.result(n) << ' ';
    fout << fibfunction(n) << ' ';

    FibComposition ff(n, &f);

    fout << ff.FibNonTrivialCalc() << ' ';
    fout << ff.FibRecCalc() << '\n';
    fout.close();

    return 0;
}
