#include <gtest/gtest.h>

#include <Fib.h>
#include <Fibfunction.h>


TEST(Fib, SimpleEqual)
{
    fibnum::Fib f;
    int n;
    std::cin >> n;
    EXPECT_EQ(n < 30, true);
    std::cout << f.result(n) << '\n';/*
    std::cout << fibfunction(n) << '\n';*/
    EXPECT_EQ(f.result(n), fibfunction(n));
}

TEST(Fib, Stress)
{
    fibnum::Fib f;
    for(int i = 1; i < 30; ++i){
        EXPECT_EQ(f.result(i), fibfunction(i));
    }
}

