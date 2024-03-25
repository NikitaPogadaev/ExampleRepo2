#include <iostream>
#include <vector>

// Интерфейс для посетителя
class Visitor;

// Интерфейс посещаемых элементов
class Shape {
public:
    virtual void accept(Visitor &visitor) = 0;
};

// Конкретные элементы
class Circle : public Shape {
public:
    void accept(Visitor &visitor) override;
};

class Rectangle : public Shape {
public:
    void accept(Visitor &visitor) override;
};

// Интерфейс посетителя
class Visitor {
public:
    virtual void visit(Circle &circle) = 0;
    virtual void visit(Rectangle &rectangle) = 0;
};

// Конкретный посетитель для вычисления площади
class AreaVisitor : public Visitor {
public:
    void visit(Circle &circle) override {
        std::cout << "Calculating area of circle\n";
        // Здесь можно вычислить площадь круга
    }

    void visit(Rectangle &rectangle) override {
        std::cout << "Calculating area of rectangle\n";
        // Здесь можно вычислить площадь прямоугольника
    }
};

// Клиентский код
int main() {
    std::vector<Shape*> shapes = {new Circle(), new Rectangle()};
    AreaVisitor areaVisitor;

    for (Shape *shape : shapes) {
        shape->accept(areaVisitor);
    }

    // Очистка памяти
    for (Shape *shape : shapes) {
        delete shape;
    }

    return 0;
}

// Реализация accept для конкретных элементов
void Circle::accept(Visitor &visitor) {
    visitor.visit(*this);
}

void Rectangle::accept(Visitor &visitor) {
    visitor.visit(*this);
}
