#ifndef AKY_HASH_HASHBUCKET_H
#define AKY_HASH_HASHBUCKET_H

#include <vector>

#include "hashnode.h"


namespace akyhash {

  template<typename K, typename V>
  class HashBucket {
  public:
    HashBucket(const int num_buckets) : num_buckets_at_last_update(num_buckets) {}
    HashBucket(const HashBucket &other) { this->num_buckets_at_last_update = other.num_buckets_at_last_update; }

    HashBucket& operator=(const HashBucket&) = delete;


  private:
    template<typename k, typename v>
    friend class HashMap;

    std::vector<Hashode<K, V>> *pNodeChain;
    int num_buckets_at_last_update;

    int insert(const K &key, const V &value, const size_t hash) {

      for (auto &node : nodes) {
	if (!node.key)
	  node.insertKeyValue(key, value, hash);
	else if (node.key == key)
	  node.insertValue(value);
      }

      return 0;
    }

    V& get(const K &key) {

      for (auto &node : nodes) {

	if (!node.occupied)
	  throw std::out_of_range("akyHashMap get");

	if (node.key == key)
	  return node.value;
      }

      throw std::out_of_range("akyHashMap get");
    }

  };
}
#endif
