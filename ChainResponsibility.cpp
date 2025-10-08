#include <iostream>
#include <string>
#include <memory>

class LoanRequest {
public:
    double amount;
    std::string purpose;

    LoanRequest(double amt, const std::string& purp)
        : amount(amt), purpose(purp) {}
};

class LoanHandler {
protected:
    std::shared_ptr<LoanHandler> nextHandler;
    std::string title;

public:
    LoanHandler(const std::string& t) : title(t) {}

    virtual ~LoanHandler() = default;

    void setNext(const std::shared_ptr<LoanHandler>& next) {
        nextHandler = next;
    }

    virtual void handleRequest(const LoanRequest& request) {
        if (canHandle(request)) {
            processRequest(request);
        } else if (nextHandler) {
            std::cout << title << " cannot process request. Passing to next...\n";
            nextHandler->handleRequest(request);
        } else {
            std::cout << "No one can process request for $" << request.amount << std::endl;
        }
    }

protected:
    virtual bool canHandle(const LoanRequest& request) = 0;
    virtual void processRequest(const LoanRequest& request) = 0;
};

class Cashier : public LoanHandler {
public:
    Cashier() : LoanHandler("Cashier") {}

protected:
    bool canHandle(const LoanRequest& request) override {
        return request.amount <= 1000;
    }

    void processRequest(const LoanRequest& request) override {
        std::cout << "Cashier approved request for $" << request.amount << std::endl;
    }
};

class Manager : public LoanHandler {
public:
    Manager() : LoanHandler("Manager") {}

protected:
    bool canHandle(const LoanRequest& request) override {
        return request.amount <= 10000;
    }

    void processRequest(const LoanRequest& request) override {
        std::cout << "Manager approved request for $" << request.amount << std::endl;
    }
};

class Director : public LoanHandler {
public:
    Director() : LoanHandler("Director") {}

protected:
    bool canHandle(const LoanRequest& request) override {
        return request.amount <= 100000;
    }

    void processRequest(const LoanRequest& request) override {
        std::cout << "Director approved request for $" << request.amount << std::endl;
    }
};

class President : public LoanHandler {
public:
    President() : LoanHandler("President") {}

protected:
    bool canHandle(const LoanRequest& request) override {
        return true;
    }

    void processRequest(const LoanRequest& request) override {
        std::cout << "President approved request for $" << request.amount << std::endl;
    }
};

int main() {
    auto cashier = std::make_shared<Cashier>();
    auto manager = std::make_shared<Manager>();
    auto director = std::make_shared<Director>();
    auto president = std::make_shared<President>();

    cashier->setNext(manager);
    manager->setNext(director);
    director->setNext(president);

    LoanRequest request1(500, "Phone");
    LoanRequest request2(5000, "Repair");
    LoanRequest request3(50000, "Car");
    LoanRequest request4(500000, "House");

    cashier->handleRequest(request1);
    cashier->handleRequest(request2);
    cashier->handleRequest(request3);
    cashier->handleRequest(request4);

    return 0;
}
