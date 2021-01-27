#pragma once
#include <thread>
#include <iostream>
#include <atomic>
#include <queue>
#include <mutex>
#include <unistd.h>
#include <linux/futex.h>
#include <sys/syscall.h>

namespace thd {

class mutex {
public:
    uint32_t futex;
    mutex();
    void lock();
    void unlock();
};

mutex::mutex() :futex{0} {};

void mutex::lock()
{
    syscall(__NR_futex, &futex, FUTEX_WAIT, 1, NULL);
    futex = 1;
}

void mutex::unlock()
{
    syscall(__NR_futex, &futex, FUTEX_WAKE, 1, NULL);
    futex = 0;
}

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

class mandatory_lock : public spinlock {
public:
    mandatory_lock();
    void lock();
};

mandatory_lock::mandatory_lock() :spinlock{} {}

void mandatory_lock::lock()
{
    if (latch.test_and_set(std::memory_order_acquire)) {
        throw std::runtime_error("Attempted to access code with mandatory lock");
    }
}

}
