#ifndef LRU_CACHE_H_
#define LRU_CACHE_H_
#include<list>
#include<unordered_map>
#include<utility>
#include<cstddef>

template<typename K, typename V>
class LRUCache {
public:
  LRUCache(size_t capacity);
  ~LRUCache();
  void insert(K key, V value);
  typename std::list<std::pair<K, V>>::iterator find(K key);
  bool contains(K key);
private:
  void evict();
  std::list<std::pair<K, V>> data_{};
  size_t capacity_{};
  std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> hash_map_{};
};
#endif // !LRU_CACHE_H_