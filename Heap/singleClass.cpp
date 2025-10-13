#include <bits/stdc++.h>
using namespace std;

class Heap {
    vector<int> a;
    bool isMin;

    bool better(int i, int j) { return isMin ? a[i] < a[j] : a[i] > a[j]; }

    void shiftUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (!better(i, p)) break;
            swap(a[p], a[i]);
            i = p;
        }
    }

    void shiftDown(int i) {
        int n = (int)a.size();
        while (true) {
            int l = 2*i + 1, r = 2*i + 2, b = i;
            if (l < n && better(l, b)) b = l;
            if (r < n && better(r, b)) b = r;
            if (b == i) break;
            swap(a[i], a[b]);
            i = b;
        }
    }

    void build() {
        for (int i = (int)a.size()/2 - 1; i >= 0; --i) shiftDown(i);
    }

public:
    Heap(bool isMin=false): isMin(isMin) {}
    Heap(vector<int>& v, bool isMin=false): a(v), isMin(isMin) { build(); }

    int size() { return (int)a.size(); }
    bool empty() { return a.empty(); }

    int top() { return a.empty() ? (isMin ? INT_MAX : INT_MIN) : a[0]; }

    void push(int x) { a.push_back(x); shiftUp((int)a.size()-1); }

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

    Heap maxH(v, false);
    cout << "max: " << maxH.top() << "\n";
    maxH.push(10);
    while (!maxH.empty()) { cout << maxH.top() << " "; maxH.pop(); }
    cout << "\n";

    Heap minH(v, true);
    cout << "min: " << minH.top() << "\n";
    minH.push(0);
    while (!minH.empty()) { cout << minH.top() << " "; minH.pop(); }
    cout << "\n";
    return 0;
}
