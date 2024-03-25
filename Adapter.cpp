#include <iostream>
#include <string>

// Интерфейс, ожидаемый клиентом
class Target {
public:
    virtual void request() const = 0;
    virtual ~Target() = default;
};

// Класс, чей интерфейс нужно адаптировать
class Adaptee {
public:
    void specificRequest() const {
        std::cout << "Adaptee's specific request\n";
    }
};

// Адаптер
class Adapter : public Target {
private:
    Adaptee *adaptee;

public:
    Adapter(Adaptee *adaptee) : adaptee(adaptee) {}

    void request() const override {
        adaptee->specificRequest();
    }
};

// Пример использования
int main() {
    Adaptee adaptee;
    Adapter adapter(&adaptee);
    adapter.request();

    return 0;
}
