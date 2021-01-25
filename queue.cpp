#include "stack.hpp"

template <class T>
class queue : public stack<T> {
    std::atomic<stack<T>::tagptr> tail;
public:
    queue();
    void push(T value);
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
    } while(!head.compare_exchange_weak(original, update));
}
