#ifndef AKY_HASH_HASHNODE_H
#define AKY_HASH_HASHNODE_H

namespace akyhash {

  template<typename K, typename V>
  class HashNode {
  public:
    HashNode(const std::pair<K, V> &kv, size_t hash) : kv{kv}, hash{hash} {}
    
    bool operator==(const HashNode &rhs) const { return kv == rhs.kv && hash == rhs.hash; }

  private:  
    template<typename k, typename v>
    friend class HashBucket;
    template<typename k, typename v, typename hash, typename keq>
    friend class HMiterator;

    size_t hash;
    std::pair<K, V> kv;

    V& giveValRef() { return kv.second; }
    std::pair<K, V>* giveKV() { return &kv; }
  };
}
#endif
