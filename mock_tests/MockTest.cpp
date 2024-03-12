#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using ::testing::Return;

// Создаем интерфейс для DataService
class DataService {
public:
    virtual ~DataService() {}
    virtual int getValue() const = 0;
};

// Реализуем класс MockDataService с использованием gmock
class MockDataService : public DataService {
public:
    MOCK_CONST_METHOD0(getValue, int());
};

// Класс, который зависит от DataService
class DataProcessor {
public:
    DataProcessor(DataService* dataService) : dataService(dataService) {}
    int processData() {
        return dataService->getValue() * 2;
    }
private:
    DataService* dataService;
};

// Тестирование DataProcessor с использованием мок-объекта
TEST(DataProcessorTest, ProcessDataTest) {
    MockDataService mockDataService;
    // EXPECT_CALL(mockDataService, getValue())
    //     .WillOnce(Return(5)); // Ожидаем, что метод getValue будет вызван один раз и вернет 5
    //
    // DataProcessor processor(&mockDataService);
    // EXPECT_EQ(processor.processData(), 10); // Проверяем, что метод processData вернет ожидаемый результат
}

// Главная функция для запуска тестов
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
