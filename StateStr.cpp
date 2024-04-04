#include <iostream>
#include <memory>
#include <string>

// Интерфейс Состояния
class State {
public:
    virtual void insertCoin() = 0;
    virtual void pressButton() = 0;
    virtual void dispense() = 0;
    virtual ~State() = default;
};

// Конкретные состояния
class IdleState : public State {
public:
    void insertCoin() override {
        std::cout << "Coin inserted, please select a drink." << std::endl;
    }

    void pressButton() override {
        std::cout << "No drink selected." << std::endl;
    }

    void dispense() override {
        std::cout << "No drink dispensed.\n" << std::endl;
    }
};

class DrinkSelectedState : public State {
private:
    std::string drink;

public:
    DrinkSelectedState(const std::string& drink) : drink(drink) {}

    void insertCoin() override {
        std::cout << "Coin already inserted." << std::endl;
    }

    void pressButton() override {
        std::cout << "Dispensing " << drink << "." << std::endl;
    }

    void dispense() override {
        std::cout << "Please press the button to dispense " << drink << "." << std::endl;
    }
};

// Контекст
class VendingMachine {
private:
    std::shared_ptr<State> currentState;

public:
    VendingMachine() : currentState(std::make_shared<IdleState>()) {}

    void setState(const std::shared_ptr<State>& newState) {
        currentState = newState;
    }

    void insertCoin() {
        currentState->insertCoin();
    }

    void pressButton() {
        currentState->pressButton();
    }

    void dispense() {
        currentState->dispense();
    }
};

int main() {
    VendingMachine vendingMachine;

    vendingMachine.insertCoin();
    vendingMachine.pressButton();
    vendingMachine.dispense();

    vendingMachine.setState(std::make_shared<DrinkSelectedState>("Cola"));
    vendingMachine.insertCoin();
    vendingMachine.pressButton();
    vendingMachine.dispense();

    return 0;
}
