#ifndef LRU_CACHE_H_
#define LRU_CACHE_H_
#include<list>
#include<unordered_map>
#include<utility>
#include<cstddef>

template<class K, class V>
class LRUCache {
public:
  LRUCache(size_t capacity);
  ~LRUCache();
  void insert(K key, V value);
  typename std::list<std::pair<K, V>>::iterator find(K key);
  typename std::list<std::pair<K, V>>::iterator end();
  bool contains(K key);
private:
  void evict();
  std::list<std::pair<K, V>> data_{};
  size_t capacity_{};
  std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> hash_map_{};
};

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
  if (hash_map_.find(key) != hash_map_.end()) {
  data_.splice(data_.begin(), data_, hash_map_.find(key)->second);
  return hash_map_.find(key)->second;
  }
  return data_.end();
}
template<class K, class V>
typename std::list<std::pair<K, V>>::iterator LRUCache<K, V>::end() {
  return data_.end();
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

#endif // !LRU_CACHE_H_