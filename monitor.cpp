#include <concepts>
#include <functional>
#include <eigen3/Eigen/Dense>
#include "locks.hpp"

namespace thd {

template <class T> 
class monitor {
    spinlock s;
    T val;
public:    
    template <class R, class ...Args>
    R execute_function(std::function<R(T, Args...)> f, Args... args);
    template <class R, class ...Args>
    R execute_member(auto f, Args... args);
    monitor();
    monitor(T v);
    
};

template <class T> 
monitor<T>::monitor() :val() {}
template <class T> 
monitor<T>::monitor(T v) :val(v) {}

// C++ templates are my new religion. This is miraculous.
template <class T>
template <class R, class ...Args>
R monitor<T>::execute_function(std::function<R(T, Args...)> f, Args... args)
{
    s.lock();
    R out = f(val, args...);
    s.unlock();
    return out;
}

template <class T>
template <class R, class ...Args>
R monitor<T>::execute_member(auto f, Args... args)
{
    s.lock();
    R out = val.f(args...);
    s.unlock();
    return out;
}

}

int arbitrary_function(char suffix, const char* w1, int w2, void* w3)
{
    std::cout << w1 << " " << w2 << " " << w3 << " " << suffix << "\n";
    return 2;
}

int main()
{
    thd::monitor<Eigen::MatrixXf> a (Eigen::MatrixXf::Random(10,10));
    // const char* c1 = "wtf this is magic";
    // int c2 = 3432;
    // void* c3 = (void*) 0x432f34;
    // std::function<int(char, const char*, int, void*)> f = arbitrary_function;
    Eigen::MatrixXf b = Eigen::MatrixXf::Random(10,10);
    // std::function<const Eigen::Product(const Eigen::MatrixBase&))> f = Eigen::MatrixXf::operator*;
    std::cout << a.execute_member(Eigen::MatrixXf::operator*, b)  << "\n";
}
