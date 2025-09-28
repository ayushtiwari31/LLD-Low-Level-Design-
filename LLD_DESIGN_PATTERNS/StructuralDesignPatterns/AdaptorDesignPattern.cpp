#include <iostream>
using namespace std;

// Step 1: Target Interface (what client expects)
class Printer {
public:
    virtual void print(string text) = 0;
    virtual ~Printer() = default;
};

// Step 2: Adaptee (existing class with different interface)
class OldPrinter {
public:
    void oldPrint(string msg) {
        cout << "Old Printer prints: " << msg << endl;
    }
};

// Step 3: Adapter (makes OldPrinter compatible with Printer)
class PrinterAdapter : public Printer {
    OldPrinter* oldPrinter;
public:
    PrinterAdapter(OldPrinter* op) : oldPrinter(op) {}
    void print(string text) override {
        oldPrinter->oldPrint(text); // delegate call
    }
};

// Step 4: Client
int main() {
    OldPrinter* legacyPrinter = new OldPrinter();
    Printer* printer = new PrinterAdapter(legacyPrinter);

    printer->print("Hello Adapter Pattern!");

    delete legacyPrinter;
    delete printer;
    return 0;
}
