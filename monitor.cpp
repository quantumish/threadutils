#include <concepts>
#include <functional>
#include "locks.hpp"

template <class T> 
class monitor {
    spinlock s;
    T val;
public:
    // HACK No clue how this works
    // Based off of https://stackoverflow.com/questions/12647217/making-a-c-class-a-monitor-in-the-concurrent-sense
    template <class R, class ...Args>
    R execute_function(std::function<R(T, Args...)> f, Args... args);
    monitor();
    monitor(T v);
    
};

template <class T> 
monitor<T>::monitor() :val() {}
template <class T> 
monitor<T>::monitor(T v) :val(v) {}

template <class T>
template <class R, class ...Args>
R monitor<T>::execute_function(std::function<R(T, Args...)> f, Args... args)
{
    return f(val, args...);
}

void arbitary_function(char suffix, const char* w1, int w2, void* w3)
{
    std::cout << " " << w1 << " " << w2 << " " << w3 << " " << suffix << "\n";
}

int main()
{
    monitor<char> a {'!'};
    const char* c1 = "wtf this is magic";
    int c2 = 3432;
    void* c3 = (void*) 0x432f34;
    std::function<void(char, const char*, int, void*)> f = arbitrary_function;
    a.execute_function(f, c1, c2, c3);
}
