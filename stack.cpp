#include <iostream>
#include <thread>
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
    void pop();
    T peek();
    T& operator[](int index);
};

template <class T>
stack<T>::stack() :head{nullptr} {}

template <class T>
void stack<T>::push(T value)
{
    Node* update;
    Node* original = head.load(std::memory_order_acquire);
    Node* addition = new Node;
    addition->val = value;
    update = addition;
    do {
        addition->next = original;
    } while(!head.compare_exchange_weak(original, update));
}

template <class T>
void stack<T>::pop()
{
    Node* update;
    Node* original = head.load(std::memory_order_acquire);
    do {
        if (original->next == nullptr) return;
        update = original->next;
    } while(!head.compare_exchange_weak(original, update));
    delete original;
}

template <class T>
T stack<T>::peek()
{
    Node* top = head.load(std::memory_order_acquire);
    return top->val;
}

// template <class T>
// T& stack<T>::operator[](int index)
// {
//     Node* r = head.load(std::memory_order_acquire);
//     for (int i = 0; i < index; i++) {
//         std::cout << r << "\n" << r->next << "\n";
//         r = r->next;
//     }
//     return r->val;
// }

int main()
{
    stack<uint64_t> a;
    std::thread t1(&stack<uint64_t>::push, &a, 1);
    std::thread t2(&stack<uint64_t>::push, &a, 2);
    std::thread t3(&stack<uint64_t>::push, &a, 3);
    t1.join(); t2.join(); t3.join();
    std::cout << a.head.load(std::memory_order_acquire)->val << "\n";
    a.pop();
    std::cout << a.head.load(std::memory_order_acquire)->val << "\n";
    a.pop();
    std::cout << a.head.load(std::memory_order_acquire)->val << "\n";
    //std::cout << a[0] << " " << a[1] << " " << a[3] << "\n";
}
