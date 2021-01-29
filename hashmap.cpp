#include <cstdlib>
#include <functional>
#include <iostream>
#include <atomic>
#include <cstdint>
#include <string>

#define TABLE_DEFAULT_SZ 15

template <class K, class V>
class hashmap {
    V* arr;
    size_t size;
    void resize(size_t min);
    int keys;
public:
    float load_factor();
    hashmap();
    void insert(K key, V val);
    V get(K key);
};

template <class K, class V>
hashmap<K,V>::hashmap()
    :arr{new V[TABLE_DEFAULT_SZ]}, size{TABLE_DEFAULT_SZ}, keys{0} {}

template <class K, class V>
void hashmap<K,V>::insert(K key, V val)
{
    std::size_t index = std::hash<K>{}(key) & size;
    if (load_factor() >= 0.75) resize(size*2 + 1);
    keys++;
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
    //std::cout << "Resizing to " << min << " bytes" << "\n";
    V* old = arr;
    arr = new V[min];
    for (int i = 0; i < size; i++) {
        arr[i] = old[i];
    }
    delete old;
    size = min;
}

template <class K, class V>
float hashmap<K,V>::load_factor() {return (float)keys/size;}



int main(int argc, char** argv)
{
    hashmap<int, int> h;
    #define MAXNUM 14000
    uint64_t BIGNUM = std::stoll(argv[1], NULL, 10);
    for (int i = 0; i < BIGNUM; i++) {
        h.insert(i, rand() % MAXNUM + 1);
    }
    for (int i = 0; i < BIGNUM; i++) {
        h.get(i);
    }
}
