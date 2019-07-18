#ifndef AKY_HASH_HASHMAP_H
#define AKY_HASH_HASHMAP_H

#include <functional>
#include <vector>

#include "hashbucket.h"

#define NUM_BUCKETS 16
#define NUM_NODES_IN_BUCKET 10

namespace akyhash {
  template<typename K, typename V>
  class HashMap {
  public:

  HashMap(std::function<size_t(const K &key)> hashFunc(), std::function<bool(const K &lhs, const K &rhs)> keyEQFunc = std::equal_to<K>()) : 
    hashFunc(hashFunc),
      keyEQFunc(keyEQFunc),
      numBuckets(NUM_BUCKETS),
      numNodesInBucket(NUM_NODES_IN_BUCKET)
      {
	buckets = std::vector<HashBucket<K, V>>(NUM_BUCKETS, HashBucket<K, V>(NUM_BUCKETS));
      }
 
  private:
    int numBuckets;
    const int numNodesInBucket;
    size_t (*hashFunc)(const K &key);
    bool (*keyEQFunc)(const K &lhs, const K &rhs);
    std::vector<HashBucket<K, V>> buckets;
  };
}
#endif
  
