// Now that we have the requirements, let's identify the key components of our Movie Ticket Booking system:

// 1. Theatre: Represents the physical theatre with screens. 
// 2. Screen: Represents a movie screen within the theatre. 
// 3. Movie: Represents a movie that can be shown. 
// 4. Show: Represents a movie screening at a specific time and screen. 
// 5. Seat: Represents an individual seat in a screen.
// 6. User: Represents a registered user of the system. 
// 7. SeatLockProvider : Common Interface for Locking Mechanisms during Ticket Booking

// The key challenges for the Movie Ticket Booking system include:
// • Concurrency Management: Handling multiple users trying to book the same seats simultaneously.
// • Session Management: Tracking and timing out booking sessions to prevent seat hoarding.
// • Payment Integration: Managing the payment process with retry capabilities.
// • Scalability: Supporting multiple theatres, screens, and concurrent users.

#include<bits/stdc++.h>;
using namespace std;

class Movie{
   private : 
   const int movieId;
   const string movieName;
   const int movieDurationInMinutes;

   public : 
   Movie(int id, const string &name, int time):movieId(id),movieName(name),movieDurationInMinutes(time){};

   int getMovieId()const{
    return movieId;
   }

   string getMovieName() const{
    return movieName;
   }

   int getMovieDuration()const{
    return movieDurationInMinutes;
   }
};

enum class SeatCategory{
SILVER, GOLD,PLATINUM
};

enum class BookingStatus {
  CREATED,// Booking has been created but not yet confirmed
  CONFIRMED, // Booking has been successfully confirmed
  EXPIRED // Booking has expired due to timeout or other factors
};

class Seat{
    private:
    const int seatId;
    int seatRow;
    SeatCategory seatCategory;

    public:

    Seat(int id, int row, SeatCategory category):seatId(id),seatRow(row),seatCategory(category){}

    int getSeatId()const{
        return seatId;
    }

    int getSeatRow()const{
        return seatRow;
    }

    SeatCategory getSeatCategory()const{
        return seatCategory;
    }

};

class Theatre;

class Screen{
    private:
    const int id;
    const string name;
    shared_ptr<Theatre> theatre;
    vector<shared_ptr<Seat>> seats;

    public:
    Screen(int id, const string &name, shared_ptr<Theatre> theatre):id(id),name(name),theatre(move(theatre)){}

    void addSeat(shared_ptr<Seat> seat){
        seats.push_back(move(seat));
    }

    int getScreenId(){
        return id;
    }

    const string& getName() const {
        return name;
    }

    shared_ptr<Theatre> getTheatre() const{
        return theatre;
    }
};


class Show{
    private:
    const int id;
    shared_ptr<Movie>movie;
    shared_ptr<Screen>screen;
    time_t startTime;
    const int durationInMinutes;

    public:
    Show(int id,shared_ptr<Movie>movie,shared_ptr<Screen>screen,time_t startTime, int duration):
    id(id),
    movie(movie),
    screen(screen),
    startTime(startTime),
    durationInMinutes(duration){}

    int getId(){
        return id;
    }

    shared_ptr<Movie>getMovie(){
        return movie;
    }
    shared_ptr<Screen>getScreen(){
        return screen;
    }

    int getDurationInMinutes()const{
        return durationInMinutes;
    }

    // Utility: get human-readable start time
    string getFormattedStartTime() const {
        return string(ctime(&startTime));  // ctime adds newline at the end
    }

};

// ---------------- Theatre Class ----------------
class Theatre {
private:
    const int id;                     
    const string name;                 
    vector<shared_ptr<Screen>> screens; 

public:
    Theatre(int id, const string& name)
        : id(id), name(name) {}

 
    void addScreen(shared_ptr<Screen> screen) {
        screens.push_back(move(screen));
    }

 
    int getTheatreId() const { return id; }

    const vector<shared_ptr<Screen>>& getScreens() const {
     return screens;
    }

    const string& getName() const { 
     return name;
    }
};


// ---------------- User Class ----------------
class User {
private:
    const string name;          
    const string emailAddress; 

public:
    
    User(const string& name, const string& emailAddress)
        : name(name), emailAddress(emailAddress) {}

    
    const string& getUserName() const { return name; }

    const string& getUserEmail() const { return emailAddress; }
};


class Booking {
private:
    const string id;                                   // Unique identifier for this booking
    shared_ptr<Show> show;                             // The specific show for which the booking is made
    shared_ptr<User> user;                             // The user who made this booking
    const vector<shared_ptr<Seat>> seatsBooked;        // The list of seats that are part of this booking
    BookingStatus bookingStatus;                       // The current status of the booking

public:
    // Constructor
    Booking(const string& id,
            shared_ptr<Show> show,
            shared_ptr<User> user,
            const vector<shared_ptr<Seat>>& seatsBooked)
        : id(id), show(move(show)), user(move(user)),
          seatsBooked(seatsBooked), bookingStatus(BookingStatus::CREATED) {}

    // Check if booking is confirmed
    bool isConfirmed() const {
        return bookingStatus == BookingStatus::CONFIRMED;
    }

    // Confirm booking
    void confirmBooking() {
        if (bookingStatus != BookingStatus::CREATED) {
            throw runtime_error("Cannot confirm a booking that is not in the Created state.");
        }
        bookingStatus = BookingStatus::CONFIRMED;
    }

    // Expire booking
    void expireBooking() {
        if (bookingStatus != BookingStatus::CREATED) {
            throw runtime_error("Cannot expire a booking that is not in the Created state.");
        }
        bookingStatus = BookingStatus::EXPIRED;
    }

    // Getters
    const string& getId() const {
        return id;
    }

    shared_ptr<Show> getShow() const {
        return show;
    }

    const vector<shared_ptr<Seat>>& getSeatsBooked() const {
        return seatsBooked;
    }

    shared_ptr<User> getUser() const {
        return user;
    }

    BookingStatus getBookingStatus() const {
        return bookingStatus;
    }
};

class SeatLock {
    Seat seat;
    Show show;
    int timeout;   // in seconds
    time_t lockTime;
    User user;

public:
    SeatLock(Seat s, Show sh, int t, time_t lt, User u)
        : seat(s), show(sh), timeout(t), lockTime(lt), user(u) {}

    bool isLockExpired() {
        time_t now = time(0);
        return difftime(now, lockTime) > timeout;
    }

    User getLockedBy() {
        return user;
    }

    Seat getSeat() {
        return seat;
    }
};

class SeatLockProvider {
    int lockTimeout; // seconds
    map<Show, map<Seat, SeatLock>> locks;
    mutex mtx;

public:
    SeatLockProvider(int timeout) {
        lockTimeout = timeout;
    }

    void lockSeats(Show show, vector<Seat> seats, User user) {
        mtx.lock();
        map<Seat, SeatLock> &seatLocks = locks[show];

        // check if already locked
        for (int i = 0; i < seats.size(); i++) {
            Seat seat = seats[i];
            if (seatLocks.find(seat) != seatLocks.end()) {
                SeatLock existing = seatLocks[seat];
                if (!existing.isLockExpired()) {
                    mtx.unlock();
                    throw runtime_error("Seat " + to_string(seat.getSeatId()) + " is already locked");
                }
            }
        }

        // lock seats
        time_t now = time(0);
        for (int i = 0; i < seats.size(); i++) {
            Seat seat = seats[i];
            seatLocks[seat] = SeatLock(seat, show, lockTimeout, now, user);
        }

        mtx.unlock();
    }

    void unlockSeats(Show show, vector<Seat> seats, User user) {
        mtx.lock();
        if (locks.find(show) == locks.end()) {
            mtx.unlock();
            return;
        }

        map<Seat, SeatLock> &seatLocks = locks[show];
        for (int i = 0; i < seats.size(); i++) {
            Seat seat = seats[i];
            if (seatLocks.find(seat) != seatLocks.end()) {
                SeatLock lock = seatLocks[seat];
                if (lock.getLockedBy().getName() == user.getName()) {
                    seatLocks.erase(seat);
                }
            }
        }

        mtx.unlock();
    }

    bool validateLock(Show show, Seat seat, User user) {
        mtx.lock();
        if (locks.find(show) == locks.end()) {
            mtx.unlock();
            return false;
        }

        map<Seat, SeatLock> &seatLocks = locks[show];
        if (seatLocks.find(seat) == seatLocks.end()) {
            mtx.unlock();
            return false;
        }

        SeatLock lock = seatLocks[seat];
        bool valid = !lock.isLockExpired() && lock.getLockedBy().getName() == user.getName();
        mtx.unlock();
        return valid;
    }

    vector<Seat> getLockedSeats(Show show) {
        mtx.lock();
        vector<Seat> result;

        if (locks.find(show) != locks.end()) {
            map<Seat, SeatLock> &seatLocks = locks[show];
            for (auto it = seatLocks.begin(); it != seatLocks.end(); it++) {
                if (!it->second.isLockExpired()) {
                    result.push_back(it->first);
                }
            }
        }

        mtx.unlock();
        return result;
    }
};


class BookingService {
    map<string, Booking> bookings;
    mutex mtx;
    SeatLockProvider &seatLockProvider;

public:
    BookingService(SeatLockProvider &provider) : seatLockProvider(provider) {}

    Booking* createBooking(string id, User user, Show show, vector<Seat> seats) {
        bool locked = seatLockProvider.lockSeats(show, seats, user);
        if (!locked) {
            cout << "[BookingService] Seats not available for booking " << id << endl;
            return nullptr;
        }

        Booking booking(id, user, show, seats);

        mtx.lock();
        bookings[id] = booking;
        mtx.unlock();

        cout << "[BookingService] Booking created with ID " << id
             << " for user " << user.getName() << endl;

        return &bookings[id];
    }

    bool confirmBooking(string id) {
        mtx.lock();
        if (bookings.find(id) == bookings.end()) {
            mtx.unlock();
            return false;
        }
        Booking &booking = bookings[id];
        bool ok = booking.confirm();
        mtx.unlock();

        if (ok) {
            cout << "[BookingService] Booking " << id << " confirmed." << endl;
        } else {
            cout << "[BookingService] Booking " << id << " was already confirmed." << endl;
        }
        return ok;
    }
};
int main(){
    return 0;
}