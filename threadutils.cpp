
#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <stdexcept>

struct Node {
    uint64_t val;
    Node* next;
};

class List {    
    Node root;
    pthread_mutex_t lock;
    pthread_mutex_t alock;

    List();
    List(uint64_t init);

    void write(int index, uint64_t newval);
    uint64_t read(int index);
    void append(uint64_t newval);    
};

List::List() :root{0, nullptr} {}
List::List(uint64_t init) :root{init, nullptr} {}

void List::write(int index, uint64_t newval)
{
    pthread_mutex_lock(&lock);
    Node* r = &root;
    for (int i = 0; i < index; i++) {
        if (r == nullptr) throw std::out_of_range("List index is out of range");
        r = r->next;
    }
    r->val=newval;
    pthread_mutex_unlock(&lock);
    printf("Wrote index %d\n", index);
}

uint64_t List::read(int index)
{
    pthread_mutex_lock(&lock);
    Node* r = &root;
    for (int i = 0; i < index; i++) {
        if (r == nullptr) throw std::out_of_range("List index is out of range");
        r = r->next;
    }
    pthread_mutex_unlock(&lock);
    printf("Read index %d of %p and got %llu\n", index, &root, r->val);
    return r->val;
}

void List::append(uint64_t newval)
{
    pthread_mutex_lock(&alock);
    Node* r = &root;
    for (int i = 0; r->next != NULL; i++) r = r->next;
    Node* next = new Node;
    next->val = newval;
    next->next = nullptr;
    r->next = next;
    pthread_mutex_unlock(&alock);
}

int main()
{    
}
