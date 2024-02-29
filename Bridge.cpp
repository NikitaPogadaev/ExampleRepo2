#include <iostream>

// Абстракция
class Shape {
protected:
    class Renderer {
    public:
        virtual void render() const = 0;
    };
    Renderer *renderer;

public:
    Shape(Renderer *renderer) : renderer(renderer) {}

    virtual void draw() const = 0;
};

// Реализация - пример
class RasterRenderer : public Shape::Renderer {
public:
    void render() const override {
        std::cout << "Drawing shapes with pixels\n";
    }
};

class VectorRenderer : public Shape::Renderer {
public:
    void render() const override {
        std::cout << "Drawing shapes with lines\n";
    }
};

// Конкретная абстракция - пример
class Circle : public Shape {
private:
    float radius;

public:
    Circle(Renderer *renderer, float radius) : Shape(renderer), radius(radius) {}

    void draw() const override {
        std::cout << "Drawing a circle with radius " << radius << ". ";
        renderer->render();
    }
};

// Конкретная абстракция - пример
class Square : public Shape {
private:
    float side;

public:
    Square(Renderer *renderer, float side) : Shape(renderer), side(side) {}

    void draw() const override {
        std::cout << "Drawing a square with side " << side << ". ";
        renderer->render();
    }
};

int main() {
    RasterRenderer rasterRenderer;
    VectorRenderer vectorRenderer;

    Circle circle1(&rasterRenderer, 5);
    Square square1(&rasterRenderer, 10);

    Circle circle2(&vectorRenderer, 3);
    Square square2(&vectorRenderer, 7);

    circle1.draw(); // Drawing a circle with radius 5. Drawing shapes with pixels
    square1.draw(); // Drawing a square with side 10. Drawing shapes with pixels

    circle2.draw(); // Drawing a circle with radius 3. Drawing shapes with lines
    square2.draw(); // Drawing a square with side 7. Drawing shapes with lines

    return 0;
}
