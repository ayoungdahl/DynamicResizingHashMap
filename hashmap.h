#ifndef AKY_HASH_HASHMAP_H
#define AKY_HASH_HASHMAP_H

#include <functional>
#include <vector>
#include <stdio.h>
#include "hashbucket.h"

#define NUM_BUCKETS 16

namespace akyhash {
  template<typename K, typename V, typename Hash = std::hash<K>, typename KeyEQ = std::equal_to<K>>
  class HashMap {
  public:

  HashMap() : numBuckets(NUM_BUCKETS) {}
  //HashMap(const Hash &hash, const KeyEQ &keyEQ) : Hash(hash), KeyEQ(keyEQ), numBuckets(NUM_BUCKETS) {}

    std::pair<V, bool> insert(const K &key, const V &value) {
      size_t hash = hashFunc(key);
      return buckets[hash % numBuckets].insert(key, value, hash, keyEQFunc);
    }

    V get(const K &key) { return buckets[hashFunc(key) % numBuckets].get(key, keyEQFunc); }
    
  private:
    int numBuckets;
    Hash hashFunc;
    KeyEQ keyEQFunc;
    std::vector<HashBucket<K, V>> buckets = std::vector<HashBucket<K, V>>(NUM_BUCKETS, HashBucket<K, V>(NUM_BUCKETS));
  };
}
#endif
  
