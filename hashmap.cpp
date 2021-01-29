#include <cstdlib>
#include <functional>
#include <iostream>
#include <atomic>

#define TABLE_DEFAULT_SZ 127

template <class K, class V>
class hashmap {
    std::atomic<V>* arr;
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
    :arr{new std::atomic<V>[TABLE_DEFAULT_SZ]}, size{TABLE_DEFAULT_SZ}, keys{0} {}

template <class K, class V>
void hashmap<K,V>::insert(K key, V val)
{
    std::size_t index = std::hash<K>{}(key) & size;
    if (load_factor() >= 0.75) resize(size*2 + 1);
    keys++;
    arr[index].store(val);
}

template <class K, class V>
V hashmap<K,V>::get(K key)
{
    std::size_t index = std::hash<K>{}(key) & 0xFFFF;
    // This may be naive to remove - assumes get() will only be called on existing keys
    // if (index >= 1000) resize(index);
    return arr[index].load();
}

template <class K, class V>
void hashmap<K,V>::resize(size_t min)
{
    //std::cout << "Resizing to " << min << " bytes" << "\n";
    std::atomic<V>* old = arr;
    arr = new std::atomic<V>[min+1];
    for (int i = 0; i < size; i++) {
        arr[i].store(old[i].load());
    }
    delete old;
    size = min+1;
}

template <class K, class V>
float hashmap<K,V>::load_factor() {return (float)keys/size;}

int main()
{
    hashmap<int, int> h;
    #define MAXNUM 14000
    #define BIGNUM 10000000
    for (int i = 0; i < BIGNUM; i++) {
        h.insert(i, rand() % MAXNUM);
    }
    for (int i = 0; i < BIGNUM; i++) {
        h.get(rand() % BIGNUM);
    }
}
