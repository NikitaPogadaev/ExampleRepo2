#include <iostream>
#include <string>
#include <memory>

// Product - сложный объект, который мы хотим создать
class Pizza {
public:
    void setDough(const std::string& dough) {
        dough_ = dough;
    }

    void setSauce(const std::string& sauce) {
        sauce_ = sauce;
    }

    void setTopping(const std::string& topping) {
        topping_ = topping;
    }

    void display() const {
        std::cout << "Pizza with " << dough_ << " dough, " << sauce_ << " sauce and " << topping_ << " topping\n";
    }

private:
    std::string dough_;
    std::string sauce_;
    std::string topping_;
};

// Builder - абстрактный интерфейс для создания продукта
class PizzaBuilder {
public:
    virtual ~PizzaBuilder() {}
    virtual void buildDough() = 0;
    virtual void buildSauce() = 0;
    virtual void buildTopping() = 0;
    virtual std::shared_ptr<Pizza> getPizza() = 0;
};

// ConcreteBuilder - конкретная реализация строителя
class HawaiianPizzaBuilder : public PizzaBuilder {
public:
    void buildDough() override {
        pizza_->setDough("thin");
    }

    void buildSauce() override {
        pizza_->setSauce("tomato");
    }

    void buildTopping() override {
        pizza_->setTopping("ham and pineapple");
    }

    std::shared_ptr<Pizza> getPizza() override {
        return pizza_;
    }

private:
    std::shared_ptr<Pizza> pizza_ = std::make_shared<Pizza>();
};

// Director - использует строителя для пошагового создания продукта
class Cook {
public:
    void setPizzaBuilder(std::shared_ptr<PizzaBuilder> builder) {
        pizzaBuilder_ = builder;
    }

    void constructPizza() {
        pizzaBuilder_->buildDough();
        pizzaBuilder_->buildSauce();
        pizzaBuilder_->buildTopping();
    }

    std::shared_ptr<Pizza> getPizza() {
        return pizzaBuilder_->getPizza();
    }

private:
    std::shared_ptr<PizzaBuilder> pizzaBuilder_;
};

int main() {
    Cook cook;
    std::shared_ptr<PizzaBuilder> hawaiianPizzaBuilder = std::make_shared<HawaiianPizzaBuilder>();
    cook.setPizzaBuilder(hawaiianPizzaBuilder);

    cook.constructPizza();
    std::shared_ptr<Pizza> pizza = cook.getPizza();
    pizza->display();

    return 0;
}
