#include <bits/stdc++.h>
using namespace std;

// Step 1: Vehicle interface
class Vehicle {
public:
    virtual void drive() = 0;
    virtual ~Vehicle() = default;
};

// Step 2: Concrete classes
class Car : public Vehicle {
public:
    void drive() override {
        cout << "Driving a Car 🚗" << endl;
    }
};

class Bike : public Vehicle {
public:
    void drive() override {
        cout << "Riding a Bike 🏍️" << endl;
    }
};

// Step 3: Factory class
// class VehicleFactory {
// public:
//     static unique_ptr<Vehicle> createVehicle(const string& type) {
//         if (type == "car") return make_unique<Car>();
//         if (type == "bike") return make_unique<Bike>();
//         return nullptr;
//     }
// };

class VehicleFactory{
    public :
    static unique_ptr<Vehicle> createVehicle(const string &type){
        if(type=="car"){
            return make_unique<Car>();
        }else if(type=="bike"){
            return make_unique<Bike>();
        }else{
            return nullptr;
        }
    }
};

// Step 4: Client code
int main() {
    auto v1 = VehicleFactory::createVehicle("car");
    auto v2 = VehicleFactory::createVehicle("bike");

    v1->drive();
    v2->drive();

    return 0;
}
