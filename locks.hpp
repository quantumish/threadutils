#pragma once
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
protected:
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

class shared_lock : spinlock {
    std::atomic<int> threads;
    std::atomic_flag shared_latch;
public:
    shared_lock();
    void s_lock();
    void s_unlock();
};

shared_lock::shared_lock() :shared_latch(false), threads(0), spinlock() {}

void shared_lock::s_lock()
{
    std::thread::id o{};
    while(latch.test_and_set(std::memory_order_acquire) && owner != o);    
    owner=o;
    threads++;
}

void shared_lock::s_unlock()
{
    threads--;
    if (threads==0) latch.clear();
}


