#include"lru_cache.h"

#include<cstddef>
#include<list>
#include<unordered_map>
#include<utility>
template<class K, class V>
LRUCache<K, V>::LRUCache(size_t capacity) : capacity_(capacity) {

}
template<class K, class V>
LRUCache<K, V>::~LRUCache() {
  while (!data_.empty()) {
    evict();
  }
}
template<class K, class V>
void LRUCache<K, V>::insert(K key, V value) {
  if (hash_map_.find(key) == hash_map_.end()) {
    auto iter = data_.begin();
    data_.insert(iter, { key, value });
    --iter;
    hash_map_.insert({ key, iter });
    if (data_.size() > capacity_) {
      evict();
    }
  }
}
template<class K, class V>
typename std::list<std::pair<K, V>>::iterator LRUCache<K, V>::find(K key) {
  data_.splice(data_.begin(), data_, hash_map_.find(key)->second);
  return hash_map_.find(key)->second;
}
template<class K, class V>
bool LRUCache<K, V>::contains(K key) {
  if (hash_map_.find(key) != hash_map_.end())
    return true;
  return false;
}
template<class K, class V>
void LRUCache<K, V>::evict() {
  auto iter = data_.end();
  --iter;
  hash_map_.erase(iter->first);
  data_.erase(iter);
}