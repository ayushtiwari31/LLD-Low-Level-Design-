#include <bits/stdc++.h>
using namespace std;

class MaxHeap {
    vector<int> a;

    void shiftUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (a[p] >= a[i]) break;
            swap(a[p], a[i]);
            i = p;
        }
    }

    void shiftDown(int i) {
        int n = (int)a.size();
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, mx = i;
            if (l < n && a[l] > a[mx]) mx = l;
            if (r < n && a[r] > a[mx]) mx = r;
            if (mx == i) break;
            swap(a[i], a[mx]);
            i = mx;
        }
    }

    void build() {
        for (int i = (int)a.size() / 2 - 1; i >= 0; --i) shiftDown(i);
    }

public:
    MaxHeap() {}
    MaxHeap(vector<int>& v) : a(v) { build(); }

    int size() { return (int)a.size(); }
    bool empty(){ return a.empty(); }

    int top() { return a.empty() ? INT_MIN : a[0]; }

    void push(int x) {
        a.push_back(x);
        shiftUp((int)a.size() - 1);
    }

    void pop() {
        if (a.empty()) return;
        a[0] = a.back();
        a.pop_back();
        if (!a.empty()) shiftDown(0);
    }

    vector<int> data() { return a; }
};

int main() {
    vector<int> v = {3, 1, 6, 5, 2, 4};
    MaxHeap h(v);
    cout << h.top() << "\n";
    h.push(10);
    cout << h.top() << "\n";
    h.pop();
    cout << h.top() << "\n";
    while (!h.empty()) {
        cout << h.top() << " ";
        h.pop();
    }
    cout << "\n";
    return 0;
}

