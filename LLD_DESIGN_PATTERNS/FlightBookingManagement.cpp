#include<bits/stdc++.h>
using namespace std;

// <--------------Passenger Class------------------> 

class Passenger{
    string Name;
    int passengerId;
    string contact;

    public:

    Passenger(int passengerID,string Name,string contact):passengerId(passengerID),Name(Name),contact(contact){};

    string getName(){
        return Name;
    }

    int getID(){
        return passengerId;
    }

    string getContact(){
        return contact;
    }

    void Display(){
        cout<<"PassengerID : "<<passengerId<<endl;
        cout<<"Name : "<<Name<<endl;
        cout<<"Contact : "<<contact<<endl;
        return;
    }

};

// <----------------Booking----------------------->

class Booking{
    int bookingId;
    shared_ptr<Passenger> passenger;
    int seatNumber;
    bool isActive;

    public:

    Booking(int id,int seat,shared_ptr<Passenger> passenger):
    bookingId(id),seatNumber(seat),passenger(passenger) ,isActive(true){}

    int getBookingId(){
        return bookingId;
    }
    int getSeatNumber() const { return seatNumber; }
    shared_ptr<Passenger> getPassenger() const { return passenger; }
    bool getStatus() const { return isActive; }

    void cancel(){
        isActive=false;
    }

    void display() const {
        cout << "Booking ID: " << bookingId
                  << ", Seat: " << seatNumber 
                  << ", Passenger: " << passenger->getName() 
                  << ", Status: " << (isActive ? "Confirmed" : "Canceled") 
                  << std::endl;
    }
};



// <----------------Flights------------------------->

class Flight{
    int flightNumber;
    string source;
    string destination;
    string departureTime;
    string arrivalTime;
    int totalSeats;
    int availableSeats;
    int bookingId;
    map<int,shared_ptr<Booking>>bookings;// it keeps record of booking for every seat

    public:

    Flight(int fn,string src,string dest,string departure,string arrv,int seats):
    flightNumber(fn),source(src),destination(dest),departureTime(departure),arrivalTime(arrv),totalSeats(seats),availableSeats(seats),bookingId(1){};


    int getFlightNumber(){ return flightNumber; }


    int bookseat(int seatNumber,shared_ptr<Passenger>passenger){
        if (seatNumber < 1 || seatNumber > totalSeats) {
            cout << "Invalid seat number!" << std::endl;
            return -1;
        }

        if (bookings.find(seatNumber) != bookings.end()) {
            cout << "Seat already booked!" << std::endl;
            return -1;
        }

        shared_ptr<Booking> booking =make_shared<Booking>(bookingId,seatNumber,passenger);
        bookings[seatNumber] = booking;
        availableSeats--;
        cout << "Seat " << seatNumber << " booked successfully on flight " << flightNumber << std::endl;
        return booking->getBookingId();
    }

    bool cancelBooking(int seatNumber) {
        auto it = bookings.find(seatNumber);
        if (it != bookings.end() && it->second->getStatus()) {
            it->second->cancel();
            availableSeats++;
            cout << "Booking for seat " << seatNumber << " is canceled." << std::endl;
            return true;
        }
        cout << "No active booking found for seat " << seatNumber << std::endl;
        return false;
    }

    void displayFlightInfo() const {
        cout << "Flight Number: " << flightNumber << std::endl;
        cout << "Route: " << source << " to " << destination << std::endl;
        cout << "Departure: " << departureTime << ", Arrival: " << arrivalTime << std::endl;
        cout << "Total Seats: " << totalSeats << ", Available Seats: " << availableSeats << std::endl;
    }

    void displayBookings() const {
        cout << "Bookings for flight " << flightNumber << ":" << std::endl;
        for (const auto &entry : bookings) {
            entry.second->display();
        }
    }

};



// <--------------------Airline Class--------------------> 

class Airline{
    string airlineName;
    vector<shared_ptr<Flight>>flights;

    public:
    Airline(string name):airlineName(name){};

    void addFlight(shared_ptr<Flight> &flight) {
        flights.push_back(flight);
    }

    shared_ptr<Flight> findFlight(int &flightNumber) const {
        for ( auto &flight : flights) {
            if (flight->getFlightNumber() == flightNumber) {
                return flight;
            }
        }
        cout << "Flight not found" << std::endl;
        return nullptr;
    }

    void displayFlights() const {
        cout << "Airline: " << airlineName << " Flights:" << std::endl;
        for (const auto &flight : flights) {
            flight->displayFlightInfo();
            std::cout << "---------------------------" << std::endl;
        }
    }

};

int main(){

    Airline airline("AirAyush");
    shared_ptr<Flight> flight1 =make_shared<Flight>(1,"Delhi","Japan","10:05 A.M","5:00 P.M",100);
    shared_ptr<Flight> flight2 =make_shared<Flight>(1,"Delhi","Dubai","8:05 A.M","10:00 A.M",120);

    airline.addFlight(flight1);
    airline.addFlight(flight2);


    // Display flights
    airline.displayFlights();

    // Create passengers
    shared_ptr<Passenger> p1 = std::make_shared<Passenger>(1, "Alice", "alice@example.com");
    shared_ptr<Passenger> p2 = std::make_shared<Passenger>(2, "Bob", "bob@example.com");
 

    int bookingId1 = flight1->bookseat(3, p1);
    int bookingId2 = flight1->bookseat(5, p2);

    // Display current bookings
    flight1->displayBookings();
    


    return 0;
}