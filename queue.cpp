#include "stack.hpp"

template <class T>
class queue {
    struct Node {
        T val;
        Node* next;
    };
    struct tagptr {
        uint64_t tag;
        Node* ptr;
    };
    std::atomic<tagptr> head;
    std::atomic<tagptr> tail;
public:
    queue();
    void push(T value);
    void pop();
    T front();
    T back();
};

template <class T>
queue<T>::queue() :head{{0,nullptr}}, tail{{0, nullptr}} {}

template <class T>
void queue<T>::push(T value)
{
    tagptr update;
    tagptr original = tail.load(std::memory_order_acquire);
    Node* addition = new Node;
    addition->val = value;
    addition->next = nullptr;
    do {
        update = original;
        update.ptr->next = addition;
        update.tag += 1;
    } while(!tail.compare_exchange_weak(original, update));
}


template <class T>
void queue<T>::pop()
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
T queue<T>::front()
{
    tagptr front = head.load(std::memory_order_acquire);
    return top.ptr->val;
}

template <class T>
T queue<T>::back()
{
    tagptr back = tail.load(std::memory_order_acquire);
    return top.ptr->val;
}

int main()
{
    queue<int> a;
    a.push(3);
    a.push(2);
    a.push(1);
    a.pop();
    std::cout << a.front() << "\n";
    std::cout << a.back() << "\n";
}
