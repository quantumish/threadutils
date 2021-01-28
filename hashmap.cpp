#define TABLE_DEFAULT_SZ

template <class K, class V> 
class hashmap {
  V arr[TABLE_DEFAULT_SZ];
  void resize(size_t min);
public:
  hashmap();
  void insert(K key, V val);
  void get(K key);
};

template <class K, class V> 
hashmap<K,V>::hashmap() {}

template <class K, class V> 
void hashmap<K,V>::insert(K key, V val)
{
  std::size_t index = std::hash<K>{}(key);
  if (index >= 1000) resize(index);
  arr[index] = val;
}

template <class K, class V> 
void hashmap<K,V>::get(K key)
{
  std::size_t index = std::hash<K>{}(key);
  // This may be naive to remove - assumes get() will only be called on existing keys
  // if (index >= 1000) resize(index);
  arr[index] = val;
}

template <class K, class V> 
void hashmap<K,V>::resize()
{
  // realloc() or something?
}
