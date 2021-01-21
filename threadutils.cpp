
#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <stdexcept>
#include <thread>

template <class T>
struct Node {
    T val;
    Node* next;
};

template <class T>
class List {    
    Node<T> root;
    pthread_mutex_t lock;
    pthread_mutex_t alock;
public:
    List();
    List(T init);

    void write(int index, T newval);
    T read(int index);
    void append(T newval);    
};

template <class T>
List<T>::List(T init) :root{init, nullptr} {}

template <class T>
void List<T>::write(int index, T newval)
{
    pthread_mutex_lock(&lock);
    Node<T>* r = &root;
    for (int i = 0; i < index; i++) {
        if (r == nullptr) throw std::out_of_range("List index is out of range");
        r = r->next;
    }
    r->val=newval;
    pthread_mutex_unlock(&lock);
    printf("Wrote index %d\n", index);
}

template <class T>
T List<T>::read(int index)
{
    pthread_mutex_lock(&lock);
    Node<T>* r = &root;
    for (int i = 0; i < index; i++) {
        if (r == nullptr) throw std::out_of_range("List index is out of range");
        r = r->next;
    }
    pthread_mutex_unlock(&lock);
    printf("Read index %d of %p and got %llu\n", index, &root, r->val);
    return r->val;
}

template <class T>
void List<T>::append(T newval)
{
    pthread_mutex_lock(&alock);
    Node<T>* r = &root;
    for (int i = 0; r->next != NULL; i++) r = r->next;
    Node<T>* next = new Node<T>;
    next->val = newval;
    next->next = nullptr;
    r->next = next;
    pthread_mutex_unlock(&alock);
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
