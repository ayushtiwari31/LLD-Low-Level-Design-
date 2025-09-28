#include <iostream>
#include <memory>
using namespace std;

// Step 1: Implementation Interface (the "bridge")
class DrawingAPI {
public:
    virtual void drawCircle(double x, double y, double radius) = 0;
    virtual ~DrawingAPI() = default;
};

// Step 2: Concrete Implementations
class OpenGLAPI : public DrawingAPI {
public:
    void drawCircle(double x, double y, double radius) override {
        cout << "OpenGL: Drawing circle at (" << x << "," << y << ") radius " << radius << endl;
    }
};

class DirectXAPI : public DrawingAPI {
public:
    void drawCircle(double x, double y, double radius) override {
        cout << "DirectX: Drawing circle at (" << x << "," << y << ") radius " << radius << endl;
    }
};

// Step 3: Abstraction (Shape)
class Shape {
protected:
    DrawingAPI* drawingAPI;  // Bridge to implementation
public:
    Shape(DrawingAPI* api) : drawingAPI(api) {}
    virtual void draw() = 0;
    virtual ~Shape() = default;
};

// Step 4: Refined Abstraction
class Circle : public Shape {
    double x, y, radius;
public:
    Circle(double x, double y, double r, DrawingAPI* api)
        : Shape(api), x(x), y(y), radius(r) {}
    void draw() override {
        drawingAPI->drawCircle(x, y, radius);
    }
};

// Step 5: Client
int main() {
    OpenGLAPI opengl;
    DirectXAPI directx;

    Circle circle1(1, 2, 3, &opengl);
    Circle circle2(4, 5, 6, &directx);

    circle1.draw();
    circle2.draw();

    return 0;
}
