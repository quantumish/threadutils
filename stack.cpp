#include <iostream>
#include <thread>
#include <atomic>

template <class T>
class stack {
    struct Node {
        T val;
        Node* next;
    };
    struct tagptr {
        uint64_t tag;
        Node* ptr;
    };
    std::atomic<tagptr> head;
public:
    stack();
    void push(T value);
    void pop();
    T peek();
    T top();
    T& operator[](int index);
};

template <class T>
stack<T>::stack() :head{{0, nullptr}} {}

template <class T>
void stack<T>::push(T value)
{
    tagptr update;
    tagptr original = head.load(std::memory_order_acquire);
    Node* addition = new Node;
    addition->val = value;
    update.ptr = addition;
    do {
        addition->next = original.ptr;
        update.tag += 1;
    } while(!head.compare_exchange_weak(original, update));
}

template <class T>
void stack<T>::pop()
{
    tagptr update;
    tagptr original = head.load(std::memory_order_acquire);
    do {
        if (original.ptr->next == nullptr) return;
        update.ptr = original.ptr->next;
        update.tag += 1;
    } while(!head.compare_exchange_weak(original, update));
    delete original.ptr;
}

template <class T>
T stack<T>::peek()
{
    tagptr top = head.load(std::memory_order_acquire);
    return top.ptr->val;
}

template <class T>
T stack<T>::top() {return peek();}

int main()
{
    stack<uint64_t> a;
    std::thread t1(&stack<uint64_t>::push, &a, 1);
    std::thread t2(&stack<uint64_t>::push, &a, 2);
    std::thread t3(&stack<uint64_t>::push, &a, 3);
    t1.join(); t2.join(); t3.join();
    std::cout << a.peek() << "\n";
    a.pop();
    std::cout << a.peek() << "\n";
    a.pop();
    std::cout << a.peek() << "\n";
}
