#ifndef AKY_HASH_HASHBUCKET_H
#define AKY_HASH_HASHBUCKET_H

#include <functional>
#include <vector>

#include "hashnode.h"

#define NUM_NODES_IN_BUCKET 10

namespace akyhash {

  enum class InsertRC {OK, DUPLICATE_KEY, BUCKET_FULL};

  template<typename K, typename V>
  class HashBucket {
  public:
    explicit HashBucket(const int num_buckets) : num_buckets_at_last_update(num_buckets) {}
    HashBucket(const HashBucket &other) {
      this->num_buckets_at_last_update = other.num_buckets_at_last_update;
      this->nodeChain = other.nodeChain;
    }
	
  private:
    template<typename k, typename v, typename Hash, typename KeyEQ>
    friend class HashMap;

    std::vector<HashNode<K, V>> nodeChain;

    int num_buckets_at_last_update;

    std::vector<HashNode<K, V>> acceptNodesFromOldReturnRejects(const std::vector<HashNode<K, V>> &oldChain, const int matchNum) {

      std::vector<HashNode<K, V>> reject;
      for (auto &node : oldChain) {

	if (node.hash % num_buckets_at_last_update == matchNum)
	  nodeChain.push_back(node);
	else 
	  reject.push_back(node);
      }

      return reject;
    }

    void swapChain(std::vector<HashNode<K, V>> &swapIn) { std::swap(nodeChain, swapIn); }
    
    std::pair<std::reference_wrapper<V>, InsertRC> insert(const K &key, const V &value, const size_t hash,
				   std::function<bool(const K &lhs, const K &rhs)> keyEQFunc) {

      for (auto &node : nodeChain) {
	if (keyEQFunc(node.key, key))
	  return {node.giveValRef(), InsertRC::DUPLICATE_KEY};
      }

      if (nodeChain.size() < NUM_NODES_IN_BUCKET) {
	nodeChain.push_back(HashNode<K, V>(key, value, hash));
	return {std::ref(nodeChain.back().value), InsertRC::OK};
      }
      
      return {(V&) value, InsertRC::BUCKET_FULL};
    }

    std::pair<std::reference_wrapper<V>, bool> giveValRef(const K &key, std::function<bool(const K &lhs, const K &rhs)> KeyEQFunc) {
      for (auto &node : nodeChain) {
	if (KeyEQFunc(node.key, key))
	  return {node.giveValRef, true}; 
      }
      return {0, false};
    }
      
    V& get(const K &key, std::function<bool(const K &lhs, const K &rhs)> keyEQFunc) {

      for (auto &node : nodeChain) {
	if (keyEQFunc(node.key, key))
	  return node.value;
      }

      throw std::out_of_range("akyHashMap get");
    }

    size_t erase(const K &key, std::function<bool(const K &lhs, const K &rhs)> keyEQFunc) {
      for (auto it = nodeChain.begin(); it != nodeChain.end(); it++) {
	if (keyEQFunc(it->key, key)) {
	  nodeChain.erase(it);
	  return 1;
	}
      }
      return 0;
    }

    size_t count (const K &key, std::function<bool(const K &lhs, const K &rhs)> keyEQFunc) const {
      for (auto &node : nodeChain) {
	if (keyEQFunc(node.key, key))
	  return 1;
      }
      return 0;
    }
  };
}
#endif
