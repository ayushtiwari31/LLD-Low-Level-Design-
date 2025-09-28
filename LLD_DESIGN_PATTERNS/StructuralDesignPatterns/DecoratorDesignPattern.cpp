#include <iostream>
using namespace std;

// Step 1: Component Interface
class Coffee {
public:
    virtual string getDescription() = 0;
    virtual double getCost() = 0;
    virtual ~Coffee() = default;
};

// Step 2: Concrete Component
class SimpleCoffee : public Coffee {
public:
    string getDescription() override { return "Simple Coffee"; }
    double getCost() override { return 50.0; }
};

// Step 3: Decorator Base Class
class CoffeeDecorator : public Coffee {
protected:
    Coffee* coffee;  // raw pointer
public:
    CoffeeDecorator(Coffee* c) : coffee(c) {}
    ~CoffeeDecorator() override { delete coffee; }  // must free manually
};

// Step 4: Concrete Decorators
class MilkDecorator : public CoffeeDecorator {
public:
    MilkDecorator(Coffee* c) : CoffeeDecorator(c) {}
    string getDescription() override { return coffee->getDescription() + " & Milk"; }
    double getCost() override { return coffee->getCost() + 10.0; }
};

class SugarDecorator : public CoffeeDecorator {
public:
    SugarDecorator(Coffee* c) : CoffeeDecorator(c) {}
    string getDescription() override { return coffee->getDescription() + " & Sugar"; }
    double getCost() override { return coffee->getCost() + 5.0; }
};

// Step 5: Client
int main() {
    Coffee* coffee = new SimpleCoffee();
    cout << coffee->getDescription() << " => ₹" << coffee->getCost() << endl;

    coffee = new MilkDecorator(coffee);
    cout << coffee->getDescription() << " => ₹" << coffee->getCost() << endl;

    coffee = new SugarDecorator(coffee);
    cout << coffee->getDescription() << " => ₹" << coffee->getCost() << endl;

    delete coffee;  // clean up entire chain

    return 0;
}
