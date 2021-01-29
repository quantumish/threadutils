#include <cstdlib>
#include <functional>
#include <iostream>
#include <atomic>
#include <cstdint>
#include <string>

#define TABLE_DEFAULT_SZ 16

template <class K, class V>
class hashmap {
    K* karr;
    V* arr;
    size_t size;
    void resize(size_t min);
    int keys;
    float load_max;
public:
    uint64_t collisions;
    float load_factor();
    hashmap(float l = 0.75);
    void insert(K key, V val);
    V get(K key);
};

template <class K, class V>
hashmap<K,V>::hashmap(float l)
    :arr{new V[TABLE_DEFAULT_SZ]()}, karr{new K[TABLE_DEFAULT_SZ]()}, size{TABLE_DEFAULT_SZ-1}, keys{0}, collisions{0}, load_max{l} {}

template <class K, class V>
void hashmap<K,V>::insert(K key, V val)
{
    std::size_t index = std::hash<K>{}(key) & size;
    // std::cout << std::hash<K>{}(key) << " " << size << " " << index << "\n";
    if (load_factor() >= load_max) resize(size*2 + 2);
    keys++;
    if (arr[index] != 0) collisions++;
    karr[index] = key;
    arr[index] = val;
}

template <class K, class V>
V hashmap<K,V>::get(K key)
{
    std::size_t index = std::hash<K>{}(key) & size;
    return arr[index];
}

template <class K, class V>
void hashmap<K,V>::resize(size_t min)
{
    std::cout << "Resizing to " << min << " bytes" << "\n";
    V* old = arr;
    K* kold = karr;
    arr = new V[min];
     karr = new K[min];
    for (int i = 0; i < size; i++) {
        size_t rehash = std::hash<K>{}(kold[i]) & min;
        arr[rehash] = old[i];
        karr[rehash] = kold[i];
    }
    delete old;
    delete kold;
    size = min - 1;
}

template <class K, class V>
float hashmap<K,V>::load_factor() {return (float)keys/size;}

std::string gen_random(const int len) {

    std::string tmp_s;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];

    return tmp_s;

}

int main(int argc, char** argv)
{
    hashmap<int, int> h(0.7);
    #define MAXNUM 14000
    uint64_t BIGNUM = std::stoll(argv[1], NULL, 10);
    for (int i = 0; i < BIGNUM; i++) {
        // std::string s = gen_random(10);
        // std::cout << s << "\n";
        h.insert(rand()%BIGNUM,rand()/(RAND_MAX/MAXNUM+1));
    }
    std::cout << h.collisions << " " << (double)h.collisions / BIGNUM << "\n";
    // for (int i = 0; i < BIGNUM; i++) {
    //     h.get(i);
    // }
}
