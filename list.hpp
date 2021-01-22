#pragma once

#include <stdexcept>
#include <mutex>
#include <iostream>

namespace thd {

template <class T>
class list {
    struct Node {
        T val;
        Node* next;
    };
    Node root;
    mutable std::mutex lock;
    mutable std::mutex alock;
public:
    list();
    list(T init);
    ~list();
    T& operator[](int index);

    int length() const noexcept;
    void write(int index, T newval);
    T read(int index);
    void append(T newval);
};

template <class T>
list<T>::list(T init) :root{init, nullptr} {}

template <class T>
list<T>::~list()
{
    Node* r = root.next;
    for (int i = 0; r->next != NULL; i++) {
        Node* old = r;
        r = r->next;
        delete old;
    }
}

template <class T>
int list<T>::length() const noexcept
{
    lock.lock();
    if (&root == nullptr) return 0;
    int length = 0;
    for (Node* r = const_cast<Node*>(&root); r->next != NULL; r=r->next) length++;
    std::cout << "length is " << length << "\n";
    lock.unlock();
    return length;
}

template <class T>
T& list<T>::operator[](int index)
{
    lock.lock();
    Node* r = &root;
    for (int i = 0; i < index; i++) {
        if (r == nullptr) throw std::out_of_range("list index is out of range");
        r = r->next;
    }
    std::cout << "Read index " << index << " and got " << r->val << "\n";
    lock.unlock();
    return r->val;
}

template <class T>
T list<T>::read(int index)
{
    return operator[](index);
}

template <class T>
void list<T>::write(int index, T newval)
{
    lock.lock();
    Node* r = &root;
    for (int i = 0; i < index; i++) {
        if (r == nullptr) throw std::out_of_range("list index is out of range");
        r = r->next;
    }
    r->val=newval;
    printf("Wrote index %d\n", index);
    lock.unlock();
}

template <class T>
void list<T>::append(T newval)
{
    alock.lock();
    Node* r = &root;
    for (int i = 0; r->next != NULL; i++) r = r->next;
    Node* next = new Node;
    next->val = newval;
    next->next = nullptr;
    r->next = next;
    alock.unlock();
}
}
