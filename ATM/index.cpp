#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

enum class CashType {
    BILL_100 = 100,
    BILL_50  = 50,
    BILL_20  = 20,
    BILL_10  = 10,
    BILL_5   = 5,
    BILL_1   = 1
};

class Card {
    string cardNumber_;
    int pin_;
    string accountNumber_;
public:
    Card(const string &cardNumber, int pin, const string &accountNumber)
        : cardNumber_(cardNumber), pin_(pin), accountNumber_(accountNumber) {}
    const string &getCardNumber() const { return cardNumber_; }
    bool validatePin(int enteredPin) const { return pin_ == enteredPin; }
    const string &getAccountNumber() const { return accountNumber_; }
};

class Account {
    string accountNumber_;
    double balance_;
public:
    Account(const string &accountNumber, double initialBalance)
        : accountNumber_(accountNumber), balance_(initialBalance) {}
    bool withdraw(double amount) {
        if (balance_ >= amount) { balance_ -= amount; return true; }
        return false;
    }
    void deposit(double amount) { balance_ += amount; }
    double getBalance() const { return balance_; }
    const string &getAccountNumber() const { return accountNumber_; }
};

class ATMMachineContext;
class ATMStateFactory;

class ATMState {
public:
    virtual ~ATMState() = default;
    virtual string getStateName() const = 0;
    // Return nullptr if no state change; otherwise return a NEW ATMState* created by Factory.
    virtual ATMState* next(ATMMachineContext &context) = 0;
};

class ATMMachineContext {
    Card *currentCard_;                 // owned
    Account *currentAccount_;           // not owned if it's from accountsDb_ (points into map values)
    unordered_map<string, Account*> accountsDb_; // ATMMachineContext owns these Account*
    ATMState *currentState_;            // owned
    ATMStateFactory *factory_;          // owned
public:
    ATMMachineContext();
    ~ATMMachineContext();

    void setState(ATMState *st) { currentState_ = st; }
    ATMState* getState() const { return currentState_; }

    void insertCard(Card *card) {        // context takes ownership
        // delete previous card if any
        if (currentCard_) delete currentCard_;
        currentCard_ = card;
    }
    void ejectCard() {
        if (currentCard_) { delete currentCard_; currentCard_ = nullptr; }
        currentAccount_ = nullptr;
    }
    Card* getCurrentCard() const { return currentCard_; }

    void setCurrentAccount(const string &accNum) {
        auto it = accountsDb_.find(accNum);
        if (it != accountsDb_.end()) currentAccount_ = it->second;
        else currentAccount_ = nullptr;
    }
    Account* getCurrentAccount() const { return currentAccount_; }

    void addAccount(Account *acc) { accountsDb_[acc->getAccountNumber()] = acc; } // context takes ownership

    void setFactory(ATMStateFactory *f) { factory_ = f; }
    ATMStateFactory* getFactory() const { return factory_; }

    // call to attempt state transition: asks current state for next(); if non-null, replace state
    void advance() {
        if (!currentState_) return;
        ATMState* candidate = currentState_->next(*this);
        if (candidate != nullptr && candidate != currentState_) {
            delete currentState_;
            currentState_ = candidate;
        }
        // if candidate == nullptr -> stay in current state
    }
};

// Forward state classes
class IdleState;
class HasCardState;
class SelectOperationState;

class ATMStateFactory {
public:
    ATMStateFactory() = default;
    ~ATMStateFactory() = default;
    ATMState* createIdleState() { return new IdleState(); }
    ATMState* createHasCardState() { return new HasCardState(); }
    ATMState* createSelectOperationState() { return new SelectOperationState(); }
};

class IdleState : public ATMState {
public:
    IdleState() { cout << "ATM is in Idle State - Please insert your card\n"; }
    string getStateName() const override { return "IdleState"; }
    ATMState* next(ATMMachineContext &context) override {
        if (context.getCurrentCard()) {
            // create next state via factory
            return context.getFactory()->createHasCardState();
        }
        return nullptr; // stay in Idle
    }
};

class HasCardState : public ATMState {
public:
    HasCardState() { cout << "ATM is in HasCard State - Please enter your PIN\n"; }
    string getStateName() const override { return "HasCardState"; }
    ATMState* next(ATMMachineContext &context) override {
        if (!context.getCurrentCard()) {
            return context.getFactory()->createIdleState();
        }
        if (context.getCurrentAccount()) {
            return context.getFactory()->createSelectOperationState();
        }
        return nullptr; // remain in HasCardState until PIN validated and account selected
    }
};

class SelectOperationState : public ATMState {
public:
    SelectOperationState() { cout << "ATM is in SelectOperation State - Choose operation\n"; }
    string getStateName() const override { return "SelectOperationState"; }
    ATMState* next(ATMMachineContext &context) override {
        if (!context.getCurrentCard()) {
            return context.getFactory()->createIdleState();
        }
        return nullptr; // stay in SelectOperation until operation finishes
    }
};

// ATMMachineContext ctor/dtor
ATMMachineContext::ATMMachineContext()
    : currentCard_(nullptr), currentAccount_(nullptr), currentState_(nullptr), factory_(nullptr)
{
    factory_ = new ATMStateFactory();
    currentState_ = factory_->createIdleState();
}

ATMMachineContext::~ATMMachineContext() {
    if (currentState_) { delete currentState_; currentState_ = nullptr; }
    if (factory_) { delete factory_; factory_ = nullptr; }
    if (currentCard_) { delete currentCard_; currentCard_ = nullptr; }
    // delete all accounts owned
    for (auto &p : accountsDb_) {
        if (p.second) delete p.second;
    }
    accountsDb_.clear();
    currentAccount_ = nullptr;
}

// Demo main
int main() {
    ATMMachineContext atm;

    // Add some accounts (context will own them)
    atm.addAccount(new Account("ACC123", 1000.0));
    atm.addAccount(new Account("ACC456", 250.0));

    // Create card and insert (context takes ownership)
    Card *card = new Card("4111-1111-1111-1111", 1234, "ACC123");
    atm.insertCard(card);

    atm.advance(); // Idle -> HasCard (since card is present)

    int enteredPin = 1234;
    if (atm.getCurrentCard() && atm.getCurrentCard()->validatePin(enteredPin)) {
        atm.setCurrentAccount(atm.getCurrentCard()->getAccountNumber());
        if (atm.getCurrentAccount())
            cout << "PIN correct. Account selected: " << atm.getCurrentAccount()->getAccountNumber() << "\n";
        else
            cout << "PIN correct but account not found.\n";
    } else {
        cout << "PIN incorrect or no card.\n";
        atm.ejectCard();
    }

    atm.advance(); // HasCard -> SelectOperation (if account set)
    ATMState *state = atm.getState();
    cout << "Current State: " << (state ? state->getStateName() : "None") << "\n";

    if (atm.getCurrentAccount()) {
        double toWithdraw = 200.0;
        if (atm.getCurrentAccount()->withdraw(toWithdraw)) {
            cout << "Dispensed " << toWithdraw << ". New balance: " << atm.getCurrentAccount()->getBalance() << "\n";
        } else {
            cout << "Insufficient funds.\n";
        }
    }

    // Eject card and return to Idle
    atm.ejectCard();
    atm.advance(); // SelectOperation -> Idle (since card removed)
    cout << "Final State: " << (atm.getState() ? atm.getState()->getStateName() : "None") << "\n";

    // atm destructor will clean up remaining objects
    return 0;
}
