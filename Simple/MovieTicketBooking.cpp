#include<bits/stdc++.h>
using namespace std;


//class representing movie
class Movie{
    private:
    string title;
    int duration;

    public:
    Movie(string title,int dur):title(title),duration(dur){};

    string getTitle(){ return title; }
    int getDuration(){ return duration;}
};


//class representing seat
class Seat{
    int seatNumber;
    bool isBooked;

    public:

    Seat(int num):seatNumber(num),isBooked(false){}

    bool bookSeat() {
        if (isBooked)
            return false; 
        isBooked = true;
        return true;
    }

    bool isAvailable(){ return !isBooked; }
    int getSeatNumber(){ return seatNumber; }
};


// class representing theater 
class Theater{
    string screenName;
    vector<Seat>seats;

    public:
    Theater(string name, int seatCount) : screenName(name) {
        for (int i = 1; i <= seatCount; i++) {
            seats.push_back(Seat(i));
        }
    }

    bool bookSeat(int seatNumber) {
        if (seatNumber < 1 || seatNumber > seats.size()) {
            std::cout << "Invalid seat number." << std::endl;
            return false;
        }
        return seats[seatNumber - 1].bookSeat();
    }

    // Display all seat availability.
    void displaySeatStatus(){
       cout << "Seat status for " << screenName << ":\n";
        for ( auto &seat : seats) {
            cout << "Seat " << seat.getSeatNumber() << ": " 
                      << (seat.isAvailable() ? "Available" : "Booked") << std::endl;
        }
    }

};


// class representing show 
class Show{
    Movie movie;
    string time;
    Theater *theater;

    public:
    Show(Movie movie, string time,  Theater* theater): 
    movie(movie), time(time), theater(theater) {}

    Show()
    : movie("", 0), time(""), theater(nullptr)
    {}

    string getTime()  { return time; }
    Movie getMovie() { return movie; }

// Interface to book seat in the show. Returns true if seat booked.
bool bookSeat(int seatNumber) {
    // Delegate the booking to the theater.
    return theater->bookSeat(seatNumber);
}

// Display seating status.
void displaySeating()  {
    theater->displaySeatStatus();
}
};

// class representing ticket 

class Ticket{
    Show show;
    int seatNumber;
    string customerName;

public:
    Ticket(Show show, int seatNumber,string customerName)
        : show(show), seatNumber(seatNumber), customerName(customerName) {}

    void printTicket() {
        std::cout << "\n--- Ticket Details ---\n";
        std::cout << "Customer: " << customerName << std::endl;
        std::cout << "Movie: " << show.getMovie().getTitle() << std::endl;
        std::cout << "Time: " << show.getTime() << std::endl;
        std::cout << "Seat Number: " << seatNumber << std::endl;
        std::cout << "----------------------\n";
    }
};


// class representing Booking system 
class BookingSystem{
    unordered_map<int,Show>shows;
    int nextShowId;
    public:

    BookingSystem():nextShowId(1){};

    int addShow(Movie movie,string time,Theater& theater){
        int showId = nextShowId++;
        shows.emplace(showId, Show(movie, time, &theater));

        return showId;
    }

    // Book a seat for a given show.
    Ticket* bookTicket(int showId, int seatNumber, string customerName) {
        if (shows.find(showId) == shows.end()) {
            cout << "Show not found!" << endl;
            return nullptr;
        }
        // Try to book the seat.
        if (!shows[showId].bookSeat(seatNumber)) {
            cout << "Seat is already booked or invalid." << endl;
            return nullptr;
        }
        // Create a ticket if booking is successful.
        return new Ticket(shows[showId], seatNumber, customerName);
    }


    // Display seating for a given show.
    void showSeatingStatus(int showId) {
        if (shows.find(showId) != shows.end()) {
            shows[showId].displaySeating();
        } else {
            cout << "Show not found!" << endl;
        }
    }


};


int main(){

    Movie movie1("Inception", 148);
    Theater theater1("Screen 1", 10); // Theater with 10 seats
    
    BookingSystem bookingSystem;
    int showId = bookingSystem.addShow(movie1, "7:00 PM", theater1);

    // Display seating status before booking.
    bookingSystem.showSeatingStatus(showId);

    // Book a seat.
    Ticket* ticket = bookingSystem.bookTicket(showId, 5, "John Doe");
    if (ticket != nullptr) {
        ticket->printTicket();
        delete ticket; // Clean up allocated memory
    }

    // Display seating status after booking.
    bookingSystem.showSeatingStatus(showId);
    return 0;
}