#ifndef AKY_HASH_HASHNODE_H
#define AKY_HASH_HASHNODE_H

namespace akyhash {

  template<typename K, typename V>
  class HashNode {
  public:
    HashNode(const std::pair<K, V> &kv, size_t hash) : kv(kv), hash(hash) {}
  private:
    template<typename k, typename v>
    friend class HashBucket;
    std::pair<K, V> kv;
    size_t hash;

    V& giveValRef() { return kv.value; }
  };
}
#endif
