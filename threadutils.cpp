#include <cstdint>
#include <thread>

#include "list.hpp"
#include "locks.hpp"

int main()
{
    thd::list<uint64_t> list{1};
    list.append(12);
    list.append(14);
    std::thread t1(&thd::list<uint64_t>::write, &list, 1, 23);
    std::thread t2(&thd::list<uint64_t>::write, &list, 1, 24);
    std::thread t3(&thd::list<uint64_t>::read, &list, 1);
    t1.join();
    t2.join();
    t3.join();
}
