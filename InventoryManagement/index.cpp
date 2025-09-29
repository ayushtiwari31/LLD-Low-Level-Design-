#include <bits/stdc++.h>
using namespace std;

// -------------------- Product Hierarchy --------------------
// Abstract base class for Product
class Product {
protected:
    string id;
    string name;
public:
    Product(const string &id, const string &name): id(id), name(name) {}
    virtual ~Product() = default;
    string getId() const { return id; }
    string getName() const { return name; }
    // Each product has a category (abstract)
    virtual string category() const = 0;
};

// Concrete product: Electronics
class Electronics : public Product {
public:
    Electronics(const string &id, const string &name): Product(id,name) {}
    string category() const override { return "Electronics"; }
};

// Concrete product: Grocery
class Grocery : public Product {
public:
    Grocery(const string &id, const string &name): Product(id,name) {}
    string category() const override { return "Grocery"; }
};

// Concrete product: Apparel
class Apparel : public Product {
public:
    Apparel(const string &id, const string &name): Product(id,name) {}
    string category() const override { return "Apparel"; }
};

// -------------------- Factory Pattern --------------------
// ProductFactory is responsible for creating products of different categories
class ProductFactory {
public:
    enum class Type { ELECTRONICS, GROCERY, APPAREL };
    static shared_ptr<Product> createProduct(Type t, const string &id, const string &name) {
        switch(t) {
            case Type::ELECTRONICS: return make_shared<Electronics>(id,name);
            case Type::GROCERY: return make_shared<Grocery>(id,name);
            case Type::APPAREL: return make_shared<Apparel>(id,name);
        }
        return nullptr;
    }
};

class InventoryManager; // Forward declaration

// -------------------- Strategy Pattern --------------------
// Base Strategy for replenishment
class ReplenishmentStrategy {
public:
    virtual ~ReplenishmentStrategy() = default;
    virtual void replenish(InventoryManager &mgr) = 0;
};

// Strategy: Reorder Point (if stock goes below threshold, order up to a level)
class ReorderPointStrategy : public ReplenishmentStrategy {
    unordered_map<string,int> reorderPoint; // reorder threshold
    unordered_map<string,int> orderUpTo;    // maximum stock to fill up to
public:
    void setRule(const string &productId, int reorder, int upTo) {
        reorderPoint[productId] = reorder;
        orderUpTo[productId] = upTo;
    }
    void replenish(InventoryManager &mgr) override;
};

// Strategy: Fixed Interval (order fixed amount after fixed number of days)
class FixedIntervalStrategy : public ReplenishmentStrategy {
    int intervalDays;
    int lastRunDay;
    int today;
    unordered_map<string,int> fixedOrderAmount;
public:
    FixedIntervalStrategy(int interval): intervalDays(interval), lastRunDay(0), today(0) {}
    void advanceDay(int d=1) { today += d; }
    void setFixedAmount(const string &productId, int amount) { fixedOrderAmount[productId] = amount; }
    void replenish(InventoryManager &mgr) override;
};

// -------------------- Singleton Pattern --------------------
// InventoryManager (only one instance)
class InventoryManager {
    // productId -> (Product object, quantity) 
    unordered_map<string, pair<shared_ptr<Product>, int>> items;
    unique_ptr<ReplenishmentStrategy> strategy; // current strategy

    // Private constructor (Singleton)
    InventoryManager() {}
public:
    InventoryManager(const InventoryManager &) = delete;
    InventoryManager& operator=(const InventoryManager &) = delete;

    // Singleton instance accessor
    static InventoryManager& instance() {
        static InventoryManager inst;
        return inst;
    }

    // Set replenishment strategy
    void setStrategy(unique_ptr<ReplenishmentStrategy> s) { strategy = move(s); }

    // Add new product or increase existing quantity
    void addProduct(shared_ptr<Product> p, int qty) {
        auto it = items.find(p->getId());
        if(it == items.end()) items[p->getId()] = {p, qty};
        else it->second.second += qty;
    }

    // Consume (reduce stock)
    bool consume(const string &productId, int qty) {
        auto it = items.find(productId);
        if(it == items.end()) return false;
        if(it->second.second < qty) return false;
        it->second.second -= qty;
        return true;
    }

    // Get available quantity of product
    int getQuantity(const string &productId) const {
        auto it = items.find(productId);
        if(it == items.end()) return 0;
        return it->second.second;
    }

    // Get list of all product IDs
    vector<string> listProductIds() const {
        vector<string> out;
        out.reserve(items.size());
        for(auto &p: items) out.push_back(p.first);
        return out;
    }

    // Get product by ID
    shared_ptr<Product> getProduct(const string &productId) const {
        auto it = items.find(productId);
        if(it == items.end()) return nullptr;
        return it->second.first;
    }

    // Print full inventory
    void printInventory() const {
        cout << "Inventory:\n";
        for(auto &kv: items) {
            cout << kv.first << " | " << kv.second.first->getName() << " | " << kv.second.first->category() << " | qty=" << kv.second.second << "\n";
        }
    }

    // Execute replenishment strategy
    void runReplenish() {
        if(strategy) strategy->replenish(*this);
    }

    // Place order (increase stock)
    void order(const string &productId, int qty) {
        auto it = items.find(productId);
        if(it == items.end()) return;
        it->second.second += qty;
        cout << "Ordered " << qty << " of " << productId << "\n";
    }
};

// -------------------- Strategy Implementations --------------------

// Reorder Point strategy implementation
void ReorderPointStrategy::replenish(InventoryManager &mgr) {
    for(auto &kv: reorderPoint) {
        const string &pid = kv.first;
        int rp = kv.second;
        int upTo = orderUpTo[pid];
        int current = mgr.getQuantity(pid);
        if(current <= rp) {
            int orderQty = max(0, upTo - current);
            if(orderQty > 0) mgr.order(pid, orderQty);
        }
    }
}

// Fixed Interval strategy implementation
void FixedIntervalStrategy::replenish(InventoryManager &mgr) {
    if((today - lastRunDay) < intervalDays) return; // not yet time to order
    lastRunDay = today;
    for(auto &kv: fixedOrderAmount) {
        const string &pid = kv.first;
        int amt = kv.second;
        mgr.order(pid, amt);
    }
}

// -------------------- Demo --------------------
int main() {
    auto &inv = InventoryManager::instance();

    // Create products using factory
    auto p1 = ProductFactory::createProduct(ProductFactory::Type::ELECTRONICS, "E100", "Smartphone");
    auto p2 = ProductFactory::createProduct(ProductFactory::Type::GROCERY, "G200", "Rice 5kg");
    auto p3 = ProductFactory::createProduct(ProductFactory::Type::APPAREL, "A300", "T-Shirt");

    // Add products to inventory
    inv.addProduct(p1, 10);
    inv.addProduct(p2, 50);
    inv.addProduct(p3, 5);
    inv.printInventory();

    // Set strategy: ReorderPointStrategy
    auto rpStrategy = make_unique<ReorderPointStrategy>();
    rpStrategy->setRule("E100", 5, 20);
    rpStrategy->setRule("G200", 20, 100);
    inv.setStrategy(move(rpStrategy));

    // Consume products
    cout << "Consuming some items...\n";
    inv.consume("E100", 7);
    inv.consume("G200", 35);
    inv.consume("A300", 2);
    inv.printInventory();

    // Run replenishment (ReorderPointStrategy)
    cout << "Running replenish (ReorderPointStrategy)\n";
    inv.runReplenish();
    inv.printInventory();

    // Change strategy to FixedIntervalStrategy
    auto fixedStrat = make_unique<FixedIntervalStrategy>(7);
    fixedStrat->setFixedAmount("A300", 20);
    inv.setStrategy(move(fixedStrat));

    // Advance days and run fixed interval replenish
    cout << "Advancing days and running fixed interval replenish\n";
    inv.runReplenish();
    inv.printInventory();

    return 0;
}
