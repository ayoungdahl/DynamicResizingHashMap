#ifndef AKY_HASH_HASHMAP_H
#define AKY_HASH_HASHMAP_H

#include <functional>
#include <vector>
#include <iostream>
#include <stdio.h>
#include "hashbucket.h"

#define NUM_BUCKETS 16

namespace akyhash {
  template<typename K, typename V, typename Hash = std::hash<K>, typename KeyEQ = std::equal_to<K>>
  class HashMap {
  public:

    HashMap() : numBuckets(NUM_BUCKETS) {}
    explicit HashMap(int nb) : numBuckets(nb) {}

    V& operator[](K &key) {
      std::pair<std::reference_wrapper<V>, bool> ref = buckets[findBucket(hashFunc(key))].giveValRef(key, keyEQFunc);

      if (ref.second)
	return ref.first.get();
      
      return insertAux(key, V()).first.get();      
    }

    std::pair<V, bool> insert(const K &key, const V &value) {
      std::pair<V, bool> ref = insertAux(key, value);
      return {ref.first, ref.second};
    }

    V get(const K &key) { return buckets[findBucket(hashFunc(key))].get(key, keyEQFunc); }
    size_t erase(const K &key) { return buckets[findBucket(hashFunc(key))].erase(key, keyEQFunc); }
    size_t count(const K &key) const { return buckets[findBucket(hashFunc(key))].count(key, keyEQFunc); } 
    
  private:
    int numBuckets;
    Hash hashFunc;
    KeyEQ keyEQFunc;
    std::vector<HashBucket<K, V>> buckets = std::vector<HashBucket<K, V>>(NUM_BUCKETS, HashBucket<K, V>(NUM_BUCKETS));

    int findBucket(size_t hash) const {

      int bucket = hash % numBuckets;
      if (buckets[bucket].num_buckets_at_last_update != numBuckets)
	bucket = hash % buckets[bucket].num_buckets_at_last_update;

      return bucket;
    }

    void splitBucket(int oldNum, int newNum) {
      std::vector<HashNode<K, V>> nodesForOld = buckets[newNum].acceptNodesFromOldReturnRejects(buckets[oldNum].nodeChain, newNum);
      buckets[oldNum].swapChain(nodesForOld);
    }

    std::pair<std::reference_wrapper<V>, bool> insertAux(const K &key, const V &value) {
      size_t hash = hashFunc(key);
      int insertBucket = findBucket(hash);

      std::pair<std::reference_wrapper<V>, InsertRC> rc = buckets[insertBucket].insert(key, value, hash, keyEQFunc);
      if (rc.second == InsertRC::OK)
	return {rc.first, true};
      else if (rc.second == InsertRC::DUPLICATE_KEY)
	return {rc.first, false};

      while (rc.second == InsertRC::BUCKET_FULL) {
	insertBucket = findBucket(hash);

	int newBucketNum;
	if (buckets[insertBucket].num_buckets_at_last_update == numBuckets) {
	  int oldNumBuckets = numBuckets;
	  numBuckets *= 2;
	  buckets.resize(numBuckets, HashBucket<K, V>(oldNumBuckets));
	  newBucketNum = insertBucket + oldNumBuckets;
	}
	else
	  newBucketNum = insertBucket + buckets[insertBucket].num_buckets_at_last_update;
	
	buckets[insertBucket].num_buckets_at_last_update = numBuckets;
	buckets[newBucketNum].num_buckets_at_last_update = numBuckets;
	splitBucket(insertBucket, newBucketNum);
	rc = buckets[findBucket(hash)].insert(key, value, hash, keyEQFunc);

	if (rc.second == InsertRC::OK)
	  return {rc.first, true};
	else
	  return {rc.first, false};
      }
    }
  };
}
#endif
  
