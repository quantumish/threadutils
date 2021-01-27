#include <cstdint>
#include <thread>

#include "list.hpp"
#include "locks.hpp"

thd::mutex a{};

void foo()
{
    a.lock();
    sleep(1);
    a.unlock();
}

void bar()
{
    a.lock();
    std::cout << "Yes!" << "\n";
    a.unlock();
}


int main()
{
    std::thread a(&foo);
    std::thread b(&bar);
    a.join(); b.join();
    thd::list<uint64_t> list{1};
    list.append(12);
    list.append(14);
    std::thread t1(&thd::list<uint64_t>::write, &list, 1, 23);
    std::thread t2(&thd::list<uint64_t>::write, &list, 1, 24);
    std::thread t3(&thd::list<uint64_t>::read, &list, 1);
    std::thread t4(&thd::list<uint64_t>::append, &list, 1);
    std::thread t5(&thd::list<uint64_t>::append, &list, 2);
    std::thread t6(&thd::list<uint64_t>::length, &list);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
}
