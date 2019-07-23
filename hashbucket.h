#ifndef AKY_HASH_HASHBUCKET_H
#define AKY_HASH_HASHBUCKET_H

#include <functional>
#include <vector>

#include "hashnode.h"

#define NUM_NODES_IN_BUCKET 10

namespace akyhash {
  
  template<typename K, typename V>
  class HashBucket {
  public:
    HashBucket(const int num_buckets) : num_buckets_at_last_update(num_buckets) {}
    HashBucket(const HashBucket &other) { this->num_buckets_at_last_update = other.num_buckets_at_last_update; }

    HashBucket& operator=(const HashBucket&) = delete;


  private:
    template<typename k, typename v, typename Hash, typename KeyEQ>
    friend class HashMap;

    std::vector<HashNode<K, V>> nodeChain = std::vector<HashNode<K, V>>(NUM_NODES_IN_BUCKET, HashNode<K,V>());
    int num_buckets_at_last_update;

    std::pair<V, bool> insert(const K &key, const V &value, const size_t hash, std::function<bool(const K &lhs, const K &rhs)> keyEQFunc) {

      for (auto &node : nodeChain) {
	if (!node.occupied) {
	  node.key = key;
	  node.value = value;
	  node.hash = hash;
	  node.occupied = true;
	  return {node.value, true};
	}
	else if (keyEQFunc(node.key, key))
	  return {node.value, false};
      }

      return {value, false};
    }

    V& get(const K &key, std::function<bool(const K &lhs, const K &rhs)> keyEQFunc) {

      for (auto &node : nodeChain) {
	if (node.occupied && keyEQFunc(node.key, key))
	  return node.value;
      }

      throw std::out_of_range("akyHashMap get");
    }

  };
}
#endif
