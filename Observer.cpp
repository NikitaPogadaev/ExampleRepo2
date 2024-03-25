#include <iostream>
#include <string>
#include <vector>
#include <span>
#include <algorithm>

// Интерфейс для подписчиков
class Observer {
public:
    virtual void update(const std::string& magazine, const std::span<std::string> rec_magazines) = 0;
    virtual ~Observer() = default;
};

// Интерфейс для издателя
class Subject {
public:
    virtual void subscribe(Observer* observer) = 0;
    virtual void unsubscribe(Observer* observer) = 0;
    virtual void notify(const std::string& magazine, std::size_t first, std::size_t last) = 0;
    virtual ~Subject() = default;
};

// Издательский дом
class Publisher : public Subject {
private:
    std::vector<Observer*> subscribers;
    std::vector<std::string> magazines;

public:
    void subscribe(Observer* observer) override {
        subscribers.push_back(observer);
    }

    void unsubscribe(Observer* observer) override {
        // Удаляем подписчика из списка
        auto it = std::find(subscribers.begin(), subscribers.end(), observer);
        if (it != subscribers.end()) {
            subscribers.erase(it);
        }
    }

    void notify(const std::string& magazine, std::size_t first, std::size_t last) override {
        // Уведомляем всех подписчиков
        for (Observer* observer : subscribers) {
            std::span<std::string> rec_magazines(magazines);
            observer->update(magazine, rec_magazines.subspan(first, last));
        }
    }

    void newMagazineAvailable(const std::string& magazine, std::size_t first = 0, std::size_t last = 0) {
        magazines.push_back(magazine);
        notify(magazine, first, last);
    }
};

// Подписчик
class Subscriber : public Observer {
public:
    void update(const std::string& magazine, const std::span<std::string> rec_magazines) override {
        std::cout << "Received new magazine: " << magazine << '\n';
        std::cout << "Your recomendations:";
        for(auto& i: rec_magazines) {
            std::cout << '\n' << i;
        }

        std::cout << '\n' << '\n';
    }
};

int main() {
    // Создаем издателя
    Publisher publisher;

    // Создаем подписчиков
    Subscriber subscriber1, subscriber2;

    // Подписываем подписчиков на издателя
    publisher.subscribe(&subscriber1);
    publisher.subscribe(&subscriber2);

    // Издатель отправляет новый выпуск журнала
    publisher.newMagazineAvailable("Magazine #1");

    // Отписываем одного из подписчиков
    publisher.unsubscribe(&subscriber1);

    // Издатель отправляет еще один новый выпуск журнала
    publisher.newMagazineAvailable("Magazine #2");
    publisher.newMagazineAvailable("Magazine #3");
    publisher.newMagazineAvailable("Magazine #4", 1, 3);
    publisher.newMagazineAvailable("Magazine #5");

    return 0;
}
