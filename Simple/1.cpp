#include <iostream>
using namespace std;

// Strategy Interface
class PaymentStrategy {
public:
    virtual void pay(int amount) = 0;  // Pure virtual function (Abstract Method)
    virtual ~PaymentStrategy() {}  // Virtual destructor
};

// Concrete Strategy 1: Credit Card Payment
class CreditCardPayment : public PaymentStrategy {
private:
    string cardNumber;
public:
    CreditCardPayment(string cardNum) : cardNumber(cardNum) {}

    void pay(int amount) override {
        cout << "Paid $" << amount << " using Credit Card: " << cardNumber << endl;
    }
};

// Concrete Strategy 2: PayPal Payment
class PayPalPayment : public PaymentStrategy {
private:
    string email;
public:
    PayPalPayment(string email) : email(email) {}

    void pay(int amount) override {
        cout << "Paid $" << amount << " using PayPal: " << email << endl;
    }
};

// Context Class: Shopping Cart
class ShoppingCart {
private:
    PaymentStrategy* paymentMethod;
public:
    void setPaymentMethod(PaymentStrategy* method) {
        paymentMethod = method;
    }

    void checkout(int amount){
        if (paymentMethod) {
            paymentMethod->pay(amount);
        } else {
            cout << "No payment method selected!" << endl;
        }
    }
};

int main() {
    ShoppingCart cart;

    // Paying with Credit Card
    CreditCardPayment creditCard("1234-5678-9012-3456");
    cart.setPaymentMethod(&creditCard);
    cart.checkout(100);

    // Paying with PayPal
    PayPalPayment paypal("user@example.com");
    cart.setPaymentMethod(&paypal);
    cart.checkout(50);

    return 0;
}
