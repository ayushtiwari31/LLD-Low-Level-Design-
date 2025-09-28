#include <iostream>
#include <memory>
using namespace std;

// Step 1: Handler Interface
class Handler {
protected:
    Handler* next = nullptr;  // pointer to next handler
public:
    void setNext(Handler* n) { next = n; }
    virtual void handleRequest(int level) {
        if (next) next->handleRequest(level);
    }
    virtual ~Handler() = default;
};

// Step 2: Concrete Handlers
class Level1Support : public Handler {
public:
    void handleRequest(int level) override {
        if (level == 1)
            cout << "Level 1 Support: Handled request ✅\n";
        else if (next)
            next->handleRequest(level);
    }
};

class Level2Support : public Handler {
public:
    void handleRequest(int level) override {
        if (level == 2)
            cout << "Level 2 Support: Handled request ✅\n";
        else if (next)
            next->handleRequest(level);
    }
};

class Manager : public Handler {
public:
    void handleRequest(int level) override {
        if (level == 3)
            cout << "Manager: Handled request ✅\n";
        else
            cout << "Request could not be handled ❌\n";
    }
};

// Step 3: Client
int main() {
    Level1Support l1;
    Level2Support l2;
    Manager manager;

    // Build the chain: L1 → L2 → Manager
    l1.setNext(&l2);
    l2.setNext(&manager);

    // Requests
    l1.handleRequest(1);
    l1.handleRequest(2);
    l1.handleRequest(3);
    l1.handleRequest(4); // no one can handle

    return 0;
}
