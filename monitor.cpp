#include <concepts>
#include "locks.hpp"
#include <eigen3/Eigen/Dense>

template <class T>
concept operatable = requires(T a, T b) {
    a+b;
    a-b;
    a/b;
    a*b;
    a&b;
    a|b;    
    a=b;
    
};

template <operatable T> 
class monitor {
    spinlock s;
public:
    T val;
    
    monitor();
    monitor(T v);
    
};

template <operatable T> 
monitor<T>::monitor() :val() {}
template <operatable T> 
monitor<T>::monitor(T v) :val(v) {}

class Nothing
{    
    Nothing();
};
    
Nothing::Nothing() {}

int main()
{
    monitor<void*> a;
    char b = 'b';
    char c = 'c';
    std::cout << b/c << "\n";
}
