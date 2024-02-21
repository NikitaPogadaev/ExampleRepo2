#include <iostream>

class Singleton {
public:
    // Статический метод, который возвращает единственный экземпляр объекта
    static Singleton& getInstance() {
        // Ленивая инициализация: объект создается только при первом вызове
        static Singleton instance;
        return instance;
    }

    // Этот метод можно использовать для чего-то, что делает этот Singleton
    void someMethod() {
        std::cout << "Hello from Singleton!\n";
    }

    // Удалите конструктор копирования и оператор присваивания, чтобы предотвратить копирование объектов
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    // Приватный конструктор, чтобы предотвратить создание объектов извне
    Singleton() {}

    // Приватный деструктор, чтобы предотвратить удаление объекта извне
    ~Singleton() {}
};

int main() {
    // Получаем единственный экземпляр Singleton
    Singleton& instance = Singleton::getInstance();
    // Вызываем метод
    instance.someMethod();

    return 0;
}
