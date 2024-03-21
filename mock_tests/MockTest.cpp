#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Fib.h>
#include <Fibfunction.h>

using namespace std;
using ::testing::Return;

// Реализуем класс MockDataService с использованием gmock
class MockFib : public fibnum::FibObj {
public:
    MOCK_METHOD(int, result, (int n), (override));
};


// Тестирование DataProcessor с использованием мок-объекта
TEST(MockFibObj, FibObj) {
    MockFib mockFib;
    EXPECT_CALL(mockFib, result(12))
        .WillOnce(Return(144)); 

    FibComposition fib_culcul(12, &mockFib);
    EXPECT_EQ(fib_culcul.FibNonTrivialCalc(), fib_culcul.FibRecCalc()); // Проверяем, что метод processData вернет ожидаемый результат
    // EXPECT_EQ(fib_culcul.FibNonTrivialCalc(), fib_culcul.FibRecCalc()); // Проверяем, что метод processData вернет ожидаемый результат
}

// Главная функция для запуска тестов
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
