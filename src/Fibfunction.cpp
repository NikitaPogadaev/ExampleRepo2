// #include"../include/Fibfunction.h"
#include "Fibfunction.h"
#include <spdlog/spdlog.h>
#include <string>

int fibfunction(int n){
    if(n == 0){
        return 0;
    }
    if(n==1){
        return 1;
    }
    spdlog::info("Status code " + std::to_string(n));
    return fibfunction(n - 1) + fibfunction(n - 2);
}
