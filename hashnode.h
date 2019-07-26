#ifndef AKY_HASH_HASHNODE_H
#define AKY_HASH_HASHNODE_H

namespace akyhash {

  template<typename K, typename V>
  class HashNode {
  public:
  HashNode(const K &key, const V &value, size_t hash) : key(key), value(value), hash(hash) {}
  private:
    template<typename k, typename v>
    friend class HashBucket;
    K key;
    V value;
    size_t hash;

    V& giveValRef() { return value; }
  };
}
#endif
