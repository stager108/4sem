# pragma once

#include <algorithm>

struct Node {
    Node* Left = nullptr;
    Node* Right = nullptr;
    unsigned char Symbol = 0;
    int Frequency = 0;

    ~Node() {
        delete Left;
        delete Right;
    }

    bool operator<(const Node& Second) {
        if (Frequency != Second.Frequency)
            return Frequency < Second.Frequency;
        return Symbol < Second.Symbol;
    }

};

class Heap {
public:

    explicit Heap() {
        n = 0;
    }

    Node* min() const {
        return a[0];
    }

    void extractMin(){
        n--;
        if (n > 0) {
            std::swap(a[0], a[n]);
            siftDown(0);
            a[n] = nullptr;
        }
    }

    void push(Node* value){
        n++;
        a[n - 1] = value;
        siftUp(n - 1);
    }

    ~Heap() { }

private:

    bool more(Node* First, Node* Second){
        return ((Second->Frequency < First->Frequency) ||
                ((Second->Frequency == First->Frequency) && (Second->Symbol < First->Symbol)));
    }

    Node* a[300];//elements
    int n;

    void swap(Node*& i, Node*& j){
        Node* c;
        c = i;
        i = j;
        j = c;
    }

    int parent(int value) const {
        return (value - 1) / 2;
    }

    int leftSon(int value) const {
        return 2 * value + 1;
    }

    int rightSon(int value) const {
        return 2 * value + 2;
    }

    void siftDown(int value) {
        int z = value;
        if (rightSon(value) < n) {
            if ((more(a[value], a[leftSon(value)])) || (more(a[value], a[rightSon(value)]))) {
                if ((more(a[value], a[leftSon(value)])) && (!more(a[leftSon(value)], a[rightSon(value)]))) {
                    swap(a[value], a[leftSon(value)]);
                    value = leftSon(value);
                } else if ((more(a[value], a[rightSon(value)])) && (!more(a[rightSon(value)], a[leftSon(value)]))) {
                    swap(a[value], a[rightSon(value)]);
                    value = rightSon(value);
                }
            }
        } else {
            if (leftSon(value) < n)
                if (more(a[value], a[leftSon(value)])) {
                    swap(a[value], a[leftSon(value)]);
                    value = leftSon(value);
                }

        }
        if (z < value)
            siftDown(value);
    }

    void siftUp(int value) {
        while ((value > 0) && (more(a[parent(value)], a[value]))) {
            swap(a[value], a[parent(value)]);
            value = parent(value);
        }
    }
};
