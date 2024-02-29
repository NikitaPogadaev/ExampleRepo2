#include <iostream>

// Абстрактные классы продуктов
class AbstractProductA {
public:
    virtual void use() const = 0;
};

class AbstractProductB {
public:
    virtual void consume() const = 0;
};

// Конкретные классы продуктов
class ConcreteProductA1 : public AbstractProductA {
public:
    void use() const override {
        std::cout << "Using ConcreteProductA1\n";
    }
};

class ConcreteProductA2 : public AbstractProductA {
public:
    void use() const override {
        std::cout << "Using ConcreteProductA2\n";
    }
};

class ConcreteProductB1 : public AbstractProductB {
public:
    void consume() const override {
        std::cout << "Consuming ConcreteProductB1\n";
    }
};

class ConcreteProductB2 : public AbstractProductB {
public:
    void consume() const override {
        std::cout << "Consuming ConcreteProductB2\n";
    }
};

// Абстрактная фабрика
class AbstractFactory {
public:
    virtual AbstractProductA* createProductA() const = 0;
    virtual AbstractProductB* createProductB() const = 0;
};

// Конкретные фабрики
class ConcreteFactory1 : public AbstractFactory {
public:
    AbstractProductA* createProductA() const override {
        return new ConcreteProductA1();
    }

    AbstractProductB* createProductB() const override {
        return new ConcreteProductB1();
    }
};

class ConcreteFactory2 : public AbstractFactory {
public:
    AbstractProductA* createProductA() const override {
        return new ConcreteProductA2();
    }

    AbstractProductB* createProductB() const override {
        return new ConcreteProductB2();
    }
};

// Пример использования
int main() {
    AbstractFactory* factory1 = new ConcreteFactory1();
    AbstractProductA* productA1 = factory1->createProductA();
    AbstractProductB* productB1 = factory1->createProductB();
    productA1->use();
    productB1->consume();

    AbstractFactory* factory2 = new ConcreteFactory2();
    AbstractProductA* productA2 = factory2->createProductA();
    AbstractProductB* productB2 = factory2->createProductB();
    productA2->use();
    productB2->consume();

    delete factory1;
    delete productA1;
    delete productB1;
    delete factory2;
    delete productA2;
    delete productB2;

    return 0;
}
