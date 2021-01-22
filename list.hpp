#pragma once

#include <stdexcept>
#include <mutex>
#include <iostream>

template <class T>
class List {
    struct Node {
        T val;
        Node* next;        
    };
    Node root;
    std::mutex lock;
    std::mutex alock;
public:
    List();
    List(T init);
    ~List();
    T& operator[](int index);
    
    void write(int index, T newval);
    T read(int index);
    void append(T newval);
};

template <class T>
List<T>::List(T init) :root{init, nullptr} {}

template <class T>
List<T>::~List()
{
    Node* r = root.next;
    for (int i = 0; r->next != NULL; i++) {
        Node* old = r;
        r = r->next;
        delete old;
    }
}

template <class T>
T& List<T>::operator[](int index)
{
    lock.lock();
    Node* r = &root;
    for (int i = 0; i < index; i++) {
        if (r == nullptr) throw std::out_of_range("List index is out of range");
        r = r->next;
    }
    std::cout << "Read index " << index << " and got " << r->val << "\n";
    lock.unlock();
    return r->val;
}

template <class T>
T List<T>::read(int index)
{
    return operator[](index);
}
    
template <class T>
void List<T>::write(int index, T newval)
{
    lock.lock();
    Node* r = &root;
    for (int i = 0; i < index; i++) {
        if (r == nullptr) throw std::out_of_range("List index is out of range");
        r = r->next;
    }
    r->val=newval;
    printf("Wrote index %d\n", index);
    lock.unlock();
}

template <class T>
void List<T>::append(T newval)
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
