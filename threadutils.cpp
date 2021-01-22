#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <stdexcept>
#include <thread>
#include <iostream>
#include <atomic>
#include <queue>
#include <mutex>

class semaphore {
    int capacity;
    std::atomic<int> value;
    std::queue<std::thread::id> wlist;
public:
    semaphore(int);
    void acquire();
    void release();    
};

semaphore::semaphore(int c) :capacity(c), value(0) {}

void semaphore::acquire()
{
    if (value >= capacity) {
        wlist.push(std::this_thread::get_id());
        while (wlist.front() != std::this_thread::get_id() || value>=capacity);
    }
    wlist.pop();
    value++;
}

void semaphore::release()
{
    value--;
}

class spinlock {
    std::atomic_flag latch;
    std::thread::id owner;
public:
    spinlock();
    void lock();
    void unlock();
};

spinlock::spinlock() :latch(false) {}

void spinlock::lock() {
    owner = std::this_thread::get_id();
    while(latch.test_and_set(std::memory_order_acquire));
}

void spinlock::unlock() {
    if (owner != std::this_thread::get_id()) return;
    latch.clear(std::memory_order_release);
}

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
    lock.unlock();    
    return r->val;
}

template <class T>
T List<T>::read(int index)
{
    std::cout << "Read index " << index << " and got " << operator[](index) << "\n";
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
    lock.unlock();
    printf("Wrote index %d\n", index);
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

int main()
{
    List<uint64_t> list{1};
    list.append(12);
    list.append(14);
    std::thread t1(&List<uint64_t>::write, &list, 1, 23);
    std::thread t2(&List<uint64_t>::write, &list, 1, 24);
    std::thread t3(&List<uint64_t>::read, &list, 1);
    t1.join();
    t2.join();
    t3.join();
}
