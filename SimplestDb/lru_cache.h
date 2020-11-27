#pragma once
#include<list>
#include<unordered_map>
#include<utility>

template<typename K, typename V>
class LRUCache {
public:
  LRUCache(size_t capacity) : capacity(capacity) {

  };
  ~LRUCache() {
    while (!data.empty()) {
      evict();
    }
  };
  void insert(K key, V value) {
    if (hash_map.find(key) == hash_map.end()) {
      auto iter = data.begin();
      data.insert(iter, { key, value });
      --iter;
      hash_map.insert({ key, iter });
      if (data.size() > capacity) {
        evict();
      }
    }
  };
  typename std::list<std::pair<K, V>>::iterator find(K key) {
    data.splice(data.begin(), data, hash_map.find(key)->second);
    return hash_map.find(key)->second;
  };
  bool contains(K key) {
    if (hash_map.find(key) != hash_map.end())
      return true;
    return false;
  }
private:
  void evict() {
    auto iter = data.end();
    --iter;
    hash_map.erase(iter->first);
    data.erase(iter);
  };

  std::list<std::pair<K, V>> data{};
  size_t capacity{};
  std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> hash_map{};
};