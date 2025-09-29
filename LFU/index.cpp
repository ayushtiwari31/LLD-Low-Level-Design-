#include <bits/stdc++.h>
using namespace std;

struct Node {
    int key, val, freq;
    Node *prev, *next;
    Node(int k, int v): key(k), val(v), freq(1), prev(nullptr), next(nullptr) {}
};

class DLinkedList {
    private: 
    Node *head, *tail;
    int size;

    public : 
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

class LFUCache {
    int capacity;
    int curSize;
    int minFreq;
    unordered_map<int, Node*> keyNode;
    unordered_map<int, DLinkedList*> freqList;

    void increaseFreq(Node* node) {
        int f = node->freq;
        DLinkedList* list = freqList[f];
        list->remove(node);
        if (list->empty()) {
            delete list;
            freqList.erase(f);
            if (minFreq == f) ++minFreq;
        }
        node->freq = f + 1;
        if (freqList.find(node->freq) == freqList.end()) freqList[node->freq] = new DLinkedList();
        freqList[node->freq]->addFront(node);
    }

public:
    LFUCache(int capacity): capacity(capacity), curSize(0), minFreq(0) {}
    ~LFUCache() {
        for (auto &p : keyNode) delete p.second;
        for (auto &p : freqList) delete p.second;
    }

    int get(int key) {
        if (capacity == 0) return -1;
        auto it = keyNode.find(key);
        if (it == keyNode.end()) return -1;
        Node* node = it->second;
        increaseFreq(node);
        return node->val;
    }

    void put(int key, int value) {
        if (capacity == 0) return;
        if (keyNode.find(key) != keyNode.end()) {
            Node* node = keyNode[key];
            node->val = value;
            increaseFreq(node);
            return;
        }
        if (curSize == capacity) {
            DLinkedList* list = freqList[minFreq];
            Node* evict = list->removeLast();
            keyNode.erase(evict->key);
            delete evict;
            if (list->empty()) {
                delete list;
                freqList.erase(minFreq);
            }
            --curSize;
        }
        Node* node = new Node(key, value);
        if (freqList.find(1) == freqList.end()) freqList[1] = new DLinkedList();
        freqList[1]->addFront(node);
        keyNode[key] = node;
        minFreq = 1;
        ++curSize;
    }
};


int main() {
    LFUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    cout << cache.get(1) << "\n"; // 1
    cache.put(3, 3); // evicts key 2
    cout << cache.get(2) << "\n"; // -1
    cout << cache.get(3) << "\n"; // 3
    cache.put(4, 4); // evicts key 1
    cout << cache.get(1) << "\n"; // -1
    cout << cache.get(3) << "\n"; // 3
    cout << cache.get(4) << "\n"; // 4
}

