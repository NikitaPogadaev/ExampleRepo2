#include <iostream>
#include <string>
#include <memory>

class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

class TurnOnCommand : public Command {
private:
    std::string device;

public:
    TurnOnCommand(const std::string& device) : device(device) {}

    void execute() override {
        std::cout << "Turning on " << device << std::endl;
    }
};

class TurnOffCommand : public Command {
private:
    std::string device;

public:
    TurnOffCommand(const std::string& device) : device(device) {}

    void execute() override {
        std::cout << "Turning off " << device << std::endl;
    }
};

// Отправитель (Invoker)
class RemoteControl {
private:
    std::shared_ptr<Command> commandOn;
    std::shared_ptr<Command> commandOff;

public:
    void setCommandOn(const std::shared_ptr<Command>& cmd) {
        commandOn = cmd;
    }
    void setCommandOff(const std::shared_ptr<Command>& cmd) {
        commandOff = cmd;
    }

    void pressButton(bool on = true) {
        if (commandOn && on) {
            commandOn->execute();
            return;
        }
        if (commandOff && !on) {
            commandOff->execute();
            return;
        }
        std::cout << "No command assigned" << std::endl;
    }
};

int main() {
    // Создаем устройства
    std::string light = "Living Room Light";
    std::string fan = "Ceiling Fan";

    // Создаем команды
    std::shared_ptr<Command> turnOnLight = std::make_shared<TurnOnCommand>(light);
    std::shared_ptr<Command> turnOffFan = std::make_shared<TurnOffCommand>(fan);

    // Создаем пульт дистанционного управления
    RemoteControl remote;

    // Назначаем команды на пульт
    remote.setCommandOn(turnOnLight);
    remote.pressButton();

    remote.setCommandOff(turnOffFan);
    remote.pressButton(false);

    return 0;
}
