#include <iostream>
#include <atomic>

template <class T>
class stack {
    struct Node {
        T val;
        Node* next;
    };
public:
    std::atomic<Node*> head;
    stack();
    void push(T value);
};

template <class T>
stack<T>::stack() :head{nullptr} {}

template <class T>
void stack<T>::push(T value)
{
    Node* update;
    Node* original = head.load(std::memory_order_acquire);
    Node* addition = new Node;
    do {
        addition->next = original;
        update = addition;
    } while(!head.compare_exchange_weak(original, update));
}

int main()
{
    stack<uint64_t> a;
    a.push(3);
    std::cout << (a.head.load(std::memory_order_acquire))->val << "\n";
}
