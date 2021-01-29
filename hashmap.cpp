#include <cstdlib>
#include <functional>
#include <iostream>

#define TABLE_DEFAULT_SZ 100

template <class K, class V>
class hashmap {
    V* arr;
    void resize(size_t min);
public:
    hashmap();
    void insert(K key, V val);
    V get(K key);
};

template <class K, class V>
hashmap<K,V>::hashmap() :arr{new V[TABLE_DEFAULT_SZ]} {}

template <class K, class V>
void hashmap<K,V>::insert(K key, V val)
{
    std::size_t index = (char)std::hash<K>{}(key);
    std::cout << index << "\n";
    if (index >= 1000) resize(index);
    arr[index] = val;
}

template <class K, class V>
V hashmap<K,V>::get(K key)
{
    std::size_t index = (char)std::hash<K>{}(key);
    // This may be naive to remove - assumes get() will only be called on existing keys
    // if (index >= 1000) resize(index);
    return arr[index];
}

template <class K, class V>
void hashmap<K,V>::resize(size_t min)
{
    V* old = arr;
    arr = new V[min+1];
    delete old;
}

int main()
{
    hashmap<std::string, int> h;
    h.insert("abc", 2);
    h.insert("abcd", 4);
    h.insert("abce", 5);
    std::cout << h.get("abc") << " " << h.get("abce") << " " << h.get("abcd") <<"\n";
}
