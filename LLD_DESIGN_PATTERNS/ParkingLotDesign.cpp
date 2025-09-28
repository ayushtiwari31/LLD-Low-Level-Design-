#include<bits/stdc++.h>
using namespace std;


class ParkingSpot; // forward declaration

// <------------Vehicle------------->

// abstract vehicle class 
class Vehicle{
    public:
    virtual string getLicensePlate()=0;
    virtual bool fitsInSpot(ParkingSpot* spot)=0;
    virtual ~Vehicle(){}
};

// concrete vehicle:bike
class Bike:public Vehicle{
    private:string licencePlate;
    public:

    Bike(string licencePlate):licencePlate(licencePlate){}

    bool fitsInSpot(ParkingSpot* spot)override;

    string getLicensePlate()override{
        return licencePlate;
    }
};

// concrete vehicle:Car
class Car:public Vehicle{
    private:string licencePlate;
    public:

    Car(string licencePlate):licencePlate(licencePlate){}

    bool fitsInSpot(ParkingSpot* spot)override;

    string getLicensePlate()override{
        return licencePlate;
    }
};

// concrete vehicle:Truck
class Truck:public Vehicle{
    private:string licencePlate;
    public:

    Truck(string licencePlate):licencePlate(licencePlate){}

    bool fitsInSpot(ParkingSpot* spot)override;

    string getLicensePlate()override{
        return licencePlate;
    }
};



// <-----------------Parking Spots---------------------->


// abstract parking spot 
class ParkingSpot{
    protected:
    int id;
    bool occupied;

    public:
    ParkingSpot(int id):id(id),occupied(false){};
    
    virtual bool canFitVehicle(Vehicle* vehicle)=0;
    virtual ~ParkingSpot(){}

    bool park(Vehicle* vehicle){
        if(!occupied && canFitVehicle(vehicle)){
            occupied=true;
            return true;
        }else 
        return false;
    }

    bool removeVehicle(){
        occupied=false;
        return true;
    }

    bool isAvailable(){
        return !occupied;
    }

    int getId(){
        return id;
    }
};

class MiniSpot:public ParkingSpot{
    public:
    MiniSpot(int id):ParkingSpot(id){};

    bool canFitVehicle(Vehicle* vehicle)override{
        // only bike can park here 
        return dynamic_cast<Bike*>(vehicle)!=nullptr;
    }
};

class CompactSpot:public ParkingSpot{
    public:
    CompactSpot(int id):ParkingSpot(id){};

    bool canFitVehicle(Vehicle* vehicle)override{
        // bike and car both can park here 
        return dynamic_cast<Bike*>(vehicle)!=nullptr || dynamic_cast<Car*>(vehicle)!=nullptr;
    }
};

class LargeSpot:public ParkingSpot{
    public:
    LargeSpot(int id):ParkingSpot(id){};
    bool canFitVehicle(Vehicle* vehicle)override{
        //any vehicle can park here 
        return true;
    }
};


// <------------Vehicle fits in spot implementations------------>

bool Bike::fitsInSpot(ParkingSpot* spot){
    return spot->canFitVehicle(this);
}

bool Car::fitsInSpot(ParkingSpot* spot){
    return spot->canFitVehicle(this);
}

bool Truck::fitsInSpot(ParkingSpot* spot){
    return spot->canFitVehicle(this);
}


// <--------------Parking Floor-------------------------------->

class ParkingFloor{
    private:
    int floorNumber;
    vector<unique_ptr<ParkingSpot>>spots;

    public:
    ParkingFloor(int floorNumber,int totspots):floorNumber(floorNumber){
        int miniSpots=totspots/4;
        int compactSpots=totspots/2;
        int largeSpots=totspots-miniSpots-compactSpots;
        int id=1;
        for (int i = 0; i < miniSpots; i++) {
            spots.push_back(make_unique<MiniSpot>(id++));
        }
        for (int i = 0; i < compactSpots; i++) {
            spots.push_back(make_unique<CompactSpot>(id++));
        }
        for (int i = 0; i < largeSpots; i++) {
            spots.push_back(make_unique<LargeSpot>(id++));
        }
    }

    // park your vehicle 
    bool parkVehicle(Vehicle* vehicle){
        for(auto &spot:spots){
            if(spot->isAvailable()&& spot->park(vehicle)){
                cout << "Parked vehicle " << vehicle->getLicensePlate()
                << " on floor " << floorNumber
                << " at spot " << spot->getId() << endl;
                return true;
            }
        }
        return false;
    }

    // remove your Vehicle
    bool removeVehicle(Vehicle *vehicle){
        for(auto &spot:spots){
            if(!spot->isAvailable() && vehicle->fitsInSpot(spot.get())){
                spot->removeVehicle();
                cout << "Removed vehicle " << vehicle->getLicensePlate()
                     << " from floor " << floorNumber
                     << " at spot " << spot->getId() << endl;
                return true;
            }
        }
        return false;
    }

    int countFreeSpots(string spotType){
        int cnt=0;
        for(auto &spot:spots){
            if (spot->isAvailable()) {
                if (spotType == "Mini" && dynamic_cast<MiniSpot*>(spot.get())) 
                    cnt++;
                else if (spotType == "Compact" && dynamic_cast<CompactSpot*>(spot.get()))
                    cnt++;
                else if (spotType == "Large" && dynamic_cast<LargeSpot*>(spot.get()))
                    cnt++;
            }
        }
        return cnt;
    }

};


// <---------------Entrance, Exit & DisplayBoard--------------------->

class Entrance {
    int id;
    string location;
public:
    Entrance(int id, const string &loc) : id(id), location(loc) {}
    int getId() const { return id; }
};

class Exit {
    int id;
    string location;
public:
    Exit(int id, const string &loc) : id(id), location(loc) {}
    int getId() const { return id; }
};

class DisplayBoard {
public:
    void update(int freeMini, int freeCompact, int freeLarge) {
        cout << "Display Board: Free Spots - Mini: " << freeMini
             << ", Compact: " << freeCompact
             << ", Large: " << freeLarge << endl;
    }
};



// <-----------Ticket & Payment------------------->


class ParkingTicket{
    private:
    int ticketId;
    time_t  entryTime;
    int floorNumber;
    int spotId;
    string vehicleLicense;

    public:
    ParkingTicket(int ticketId,int floorNumber,int spotId, string vehicleLicense):
    ticketId(ticketId),floorNumber(floorNumber),spotId(spotId),vehicleLicense(vehicleLicense){
        entryTime=time(nullptr);
    }

    void printTicket(){
        cout << "Ticket ID: " << ticketId
        << ", Floor: " << floorNumber
        << ", Spot: " << spotId
        << ", License: " << vehicleLicense
        << ", Entry Time: " << ctime(&entryTime);
    }
};

class PaymentStrategy{
    public:
    virtual void pay(double)=0;
    virtual ~PaymentStrategy(){}
};

class CashPayment:public PaymentStrategy{
    public:
    void pay(double amount)override{
        cout << "Paid Rs" << amount << " in Cash" << endl;
    }
};

class CreditCardPayment : public PaymentStrategy {
    public:
        void pay(double amount) override {
            cout << "Paid $" << amount << " via Credit Card" << endl;
        }
};



// <------------------Adming & Parking Attendent--------------->

class Admin{
    private:
    string name;

    public:
    Admin(string name):name(name){};

    void addEntrance(vector<Entrance>&entrances,Entrance entrance){
        entrances.push_back(entrance);
    }

    void removeEntrance(vector<Entrance>&entrances,int id){
        for (auto it = entrances.begin(); it != entrances.end(); ++it) {
            if (it->getId() == id) {
                entrances.erase(it);
                break; 
            }
        }
    }

    // void addExits
    // void removeExit

};

class ParkingAttendent{
    int ticketCounter;

    public:ParkingAttendent():ticketCounter(0){};


    unique_ptr<ParkingTicket> createTicket(int floor, int spot, const string &license) {
        return make_unique<ParkingTicket>(++ticketCounter, floor, spot, license);
    }
};



// <--------------------ParkingLot--------------------------------->

class ParkingLot{
    vector<ParkingFloor>floors;
    vector<Entrance>entrance;
    vector<Exit>exit;
    DisplayBoard displayboard;

    public:

    ParkingLot(int numfloors,int spotsperfloor){
        for(int i=1;i<=numfloors;i++){
            floors.push_back(ParkingFloor(i,spotsperfloor));
        }
    }

    void addEntrance(Entrance entrance){
        this->entrance.push_back(entrance);
    }

    void removeEntrance(int id){
        for(auto it=entrance.begin();it!=entrance.end();it++){
            if(it->getId()==id){
                entrance.erase(it);
                break;
            }
        }
    }


    void updateDisplay(){
        int freeMini = 0, freeCompact = 0, freeLarge = 0;
        for(auto &floor:floors){
            freeMini+=floor.countFreeSpots("Mini");
            freeCompact += floor.countFreeSpots("Compact");
            freeLarge += floor.countFreeSpots("Large");
        }
        displayboard.update(freeMini,freeCompact,freeLarge);
    }

    bool parkVehicle(Vehicle* vehicle) {
        for (auto &floor : floors) {
            if (floor.parkVehicle(vehicle)) {
                updateDisplay();
                return true;
            }
        }
        cout << "Parking Lot is full for vehicle " << vehicle->getLicensePlate() << endl;
        return false;
    }

    bool removeVehicle(Vehicle* vehicle) {
        for (auto &floor : floors) {
            if (floor.removeVehicle(vehicle)) {
                updateDisplay();
                return true;
            }
        }
        cout << "Vehicle " << vehicle->getLicensePlate() << " not found." << endl;
        return false;
    }


};

int main(){

    ParkingLot lot(3,12);
    Admin admin("Ayush");

    vector<Entrance> entrances;
    admin.addEntrance(entrances, Entrance(1, "North Gate"));
    admin.addEntrance(entrances, Entrance(2, "East Gate"));

    // Exits can be added similarly.

    // Parking attendant creates tickets.
    ParkingAttendent attendant;


    // Create vehicles.
    Car car("CAR-101");
    Bike bike("BIKE-202");
    Truck truck("TRUCK-303");

    // Park vehicles.
    lot.parkVehicle(&car);
    lot.parkVehicle(&bike);
    lot.parkVehicle(&truck);

    // Generate and print a ticket for the car.
    auto ticket = attendant.createTicket(1, 5, car.getLicensePlate());
    ticket->printTicket();

     // Process a payment using a payment strategy.
    unique_ptr<PaymentStrategy> payment = make_unique<CreditCardPayment>();
    payment->pay(20.0);
 
    // Remove a vehicle.
    lot.removeVehicle(&bike);


    return 0;
}