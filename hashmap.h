#ifndef AKY_HASH_HASHMAP_H
#define AKY_HASH_HASHMAP_H

#include <functional>
#include <vector>
#include <iostream>
#include <stdio.h>
#include "hashbucket.h"

#define NUM_BUCKETS 16

namespace akyhash {
  template<typename K, typename V> class HMiterator;

  template<typename K, typename V, typename Hash = std::hash<K>, typename KeyEQ = std::equal_to<K>>
  class HashMap {
  public:

    HashMap() : numBuckets(NUM_BUCKETS) {}
    explicit HashMap(int nb) : numBuckets(nb) {}

    bool operator==(const HashMap<K, V> &rhs) { return buckets == rhs.buckets; }
    bool operator!=(const HashMap<K, V> &rhs) { return buckets != rhs.buckets; }
  
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
    HMiterator<K, V> begin() { return HMiterator<K, V>(&this, buckets.begin(), buckets[0].nodeChain.begin()); }
    HMiterator<K, V> end() { return HMiterator<K, V>(&this, buckets.end(),buckets[numBuckets - 1].nodeChain.end()); }

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

    inline void splitBucket(int oldNum, int newNum) {
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

  template<typename K, typename V>
  class HMiterator {

  public:
 
    HMiterator(HashMap<K, V> &hm, typename std::vector<HashBucket<K, V>>::iterator bit, typename std::vector<HashNode<K, V>> nit)
    : hm(hm), bucket_it(bit), node_it(nit) {}

    HMiterator(const HMiterator<K, V> &orig) : hm(orig.hm), bucket_it(orig.bucket_it), node_it(orig.node_it) {}

    inline void increment() {
      if (++node_it == bucket_it->nodeChain.end()) {
	while (bucket_it != hm.buckets.end() && node_it == bucket_it->nodeChain.end()) {
	  node_it = ++bucket_it->nodeChain.begin();
	}
      }
    }

    HMiterator& operator=(const HMiterator<K, V> &rhs) {
      hm = rhs.hm;
      bucket_it = rhs.bucket_it;
      node_it = rhs.node_it;
      return *this;
    }

    HMiterator& operator++() {
      increment();
      return &this;
    }

    HMiterator operator++(int) {
      HMiterator orig(this);
      increment();
      return orig;
    }

    bool operator==(const HMiterator<K, V> &rhs) { return hm == rhs.hm && bucket_it == rhs.bucket_it && node_it == rhs.node_it; }
    bool operator!=(const HMiterator<K, V> &rhs) { return hm != rhs.hm || bucket_it != rhs.bucket_it || node_it != rhs.node_it; }
    std::pair<K, std::reference_wrapper<V>> operator*() { return {node_it->key, std::ref(node_it->value)}; }
    std::pair<K, V*> operator->() { return {node_it->key; &node_it->value}; }
    
  private:
    HashMap<V, V> &hm;
    typename std::vector<HashBucket<K, V>>::iterator bucket_it;
    typename std::vector<HashNode<K, V>>::iterator node_it;
  };

}
#endif
  
