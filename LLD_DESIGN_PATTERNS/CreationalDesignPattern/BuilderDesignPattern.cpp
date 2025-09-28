#include <iostream>
#include <string>
using namespace std;

// Step 1: Product
class Pizza {
    bool cheese;
    bool pepperoni;
    bool mushrooms;
    bool olives;

public:
    Pizza(bool ch, bool pep, bool mush, bool oli)
        : cheese(ch), pepperoni(pep), mushrooms(mush), olives(oli) {}

    void show() {
        cout << "Pizza with: "
             << (cheese ? "Cheese " : "")
             << (pepperoni ? "Pepperoni " : "")
             << (mushrooms ? "Mushrooms " : "")
             << (olives ? "Olives " : "")
             << endl;
    }
};

// Step 2: Builder
class PizzaBuilder {
    bool cheese = false;
    bool pepperoni = false;
    bool mushrooms = false;
    bool olives = false;

public:
    PizzaBuilder& addCheese(){
        cheese=true;
        return *this;
    }
    PizzaBuilder& addPepperoni() { pepperoni = true; return *this; }
    PizzaBuilder& addMushrooms() { mushrooms = true; return *this; }
    PizzaBuilder& addOlives() { olives = true; return *this; }

    Pizza build(){
         return Pizza(cheese, pepperoni, mushrooms, olives);
    }
};

// Step 3: Client
int main() {
    Pizza pizza1 = PizzaBuilder()
                    .addCheese()
                    .addOlives()
                    .build();

    Pizza pizza2 = PizzaBuilder()
                    .addPepperoni()
                    .addMushrooms()
                    .build();

    pizza1.show();
    pizza2.show();

    return 0;
}
