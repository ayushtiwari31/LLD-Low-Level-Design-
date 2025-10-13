#include <bits/stdc++.h>
using namespace std;

struct Node {
    int key, val;
    Node *prev, *next;
    Node(int k, int v): key(k), val(v), prev(nullptr), next(nullptr) {}
};

class DLinkedList {
    Node *head, *tail;
    int size;

public:
    DLinkedList(): head(nullptr), tail(nullptr), size(0) {}

    void addFront(Node* node) {
        node->prev = nullptr;
        node->next = head;
        if (head) head->prev = node;
        head = node;
        if (!tail) tail = head;
        ++size;
    }

    void remove(Node* node) {
        if (!node) return;
        if (node->prev) node->prev->next = node->next;
        else head = node->next;
        if (node->next) node->next->prev = node->prev;
        else tail = node->prev;
        node->prev = node->next = nullptr;
        --size;
    }

    Node* removeLast() {
        if (!tail) return nullptr;
        Node* node = tail;
        remove(node);
        return node;
    }

    bool empty() const { return size == 0; }
};

class LRUCache {
    int capacity;
    int curSize;
    unordered_map<int, Node*> keyNode;
    DLinkedList *list;

public:
    LRUCache(int capacity): capacity(capacity), curSize(0) {
        list = new DLinkedList();
    }

    ~LRUCache() {
        for (auto &p : keyNode) delete p.second;
        delete list;
    }

    int get(int key) {
        if (capacity == 0) return -1;
        auto it = keyNode.find(key);
        if (it == keyNode.end()) return -1;
        Node* node = it->second;
        list->remove(node);
        list->addFront(node);
        return node->val;
    }

    void put(int key, int value) {
        if (capacity == 0) return;
        auto it = keyNode.find(key);
        if (it != keyNode.end()) {
            Node* node = it->second;
            node->val = value;
            list->remove(node);
            list->addFront(node);
            return;
        }

        if (curSize == capacity) {
            Node* evict = list->removeLast();
            if (evict) {
                keyNode.erase(evict->key);
                delete evict;
                --curSize;
            }
        }

        Node* node = new Node(key, value);
        list->addFront(node);
        keyNode[key] = node;
        ++curSize;
    }
};

int main() {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    cout << cache.get(1) << "\n"; // 1
    cache.put(3, 3);              // evicts key 2
    cout << cache.get(2) << "\n"; // -1
    cache.put(4, 4);              // evicts key 1
    cout << cache.get(1) << "\n"; // -1
    cout << cache.get(3) << "\n"; // 3
    cout << cache.get(4) << "\n"; // 4

    return 0;
}
