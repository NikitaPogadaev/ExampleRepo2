#include <iostream>

// Подсистема
class SubsystemA {
public:
    void operationA() {
        std::cout << "Subsystem A operation\n";
    }
};

// Подсистема
class SubsystemB {
public:
    void operationB() {
        std::cout << "Subsystem B operation\n";
    }
};

// Фасад
class Facade {
private:
    SubsystemA subsystemA;
    SubsystemB subsystemB;

public:
    void operation() {
        subsystemA.operationA();
        subsystemB.operationB();
    }
};

// Пример использования
int main() {
    Facade facade;
    facade.operation();

    return 0;
}
