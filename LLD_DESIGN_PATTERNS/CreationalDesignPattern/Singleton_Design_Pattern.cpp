#include <iostream>
#include <mutex>
using namespace std;

class Singleton {
private:
    static Singleton* instance;   // single instance pointer
    static mutex mtx;             // for thread safety

    // private constructor
    Singleton() {
        cout << "Singleton instance created\n";
    }

    // delete copy & assignment
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

public:
    static Singleton* getInstance() {
        // Double-checked locking for thread safety
        if (instance == nullptr) {
            lock_guard<mutex> lock(mtx);
            if (instance == nullptr) {
                instance = new Singleton();
            }
        }
        return instance;
    }

    void showMessage() {
        cout << "Hello from Singleton (pointer version)!\n";
    }
};

// initialize static members
Singleton* Singleton::instance = nullptr;
mutex Singleton::mtx;

// Client
int main() {
    Singleton* s1 = Singleton::getInstance();
    Singleton* s2 = Singleton::getInstance();

    s1->showMessage();

    if (s1 == s2) {
        cout << "Both are the same instance\n";
    }
}
