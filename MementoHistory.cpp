#include <iostream>
#include <string>
#include <vector>
#include <memory> 

// Хранитель (Memento)
class Memento {
private:
    std::string state;

public:
    Memento(const std::string& state) : state(state) {}

    std::string getState() const {
        return state;
    }
};

// Создатель (Originator)
class Editor {
private:
    std::string text;

public:
    void setText(const std::string& newText) {
        text = newText;
    }

    std::string getText() const {
        return text;
    }

    // Создание момента для сохранения текущего состояния
    std::shared_ptr<Memento> createMemento() {
        return std::make_shared<Memento>(text);
    }

    // Восстановление состояния из момента
    void restoreFromMemento(const std::shared_ptr<Memento>& memento) {
        text = memento->getState();
    }
};

// Опекун (Caretaker)
class History {
private:
    std::vector<std::shared_ptr<Memento>> mementos;

public:
    void addMemento(const std::shared_ptr<Memento>& memento) {
        mementos.push_back(memento);
    }

    std::shared_ptr<Memento> getMemento(int index) const {
        return mementos[index];
    }
};

int main() {
    Editor editor;
    History history;

    // Редактирование текста и сохранение состояний
    editor.setText("Hello, world!");
    history.addMemento(editor.createMemento());

    editor.setText("Goodbye, world!");
    history.addMemento(editor.createMemento());

    // Восстановление состояния
    editor.restoreFromMemento(history.getMemento(0));
    std::cout << "Restored text: " << editor.getText() << '\n';

    editor.restoreFromMemento(history.getMemento(1));
    std::cout << "Restored text: " << editor.getText() << '\n';
    return 0;
}
