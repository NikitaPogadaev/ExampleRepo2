#include"../include/Fibfunction.h"

int fibfunction(int n){
    if(n == 0){
        return 0;
    }
    if(n==1){
        return 1;
    }
    return fibfunction(n - 1) + fibfunction(n - 2);
}
