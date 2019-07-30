#ifndef AKY_HASH_HASHNODE_H
#define AKY_HASH_HASHNODE_H

namespace akyhash {

  template<typename K, typename V>
  class HashNode {
  public:
    HashNode(const std::pair<K, V> &kv, size_t hash) : kv(kv), hash(hash) {}
    HashNode& operator=(const HashNode &rhs) {
      kv = {rhs.kv.first, rhs.kv.second};
      hash = rhs.hash;
      return *this;
    }

    bool operator==(const HashNode &rhs) const { return kv == rhs.kv && hash == rhs.hash; }
    
  private:
    template<typename k, typename v>
    friend class HashBucket;
    template<typename k, typename v, typename hash, typename keq>
    friend class HMiterator;
    std::pair<K, V> kv;
    size_t hash;

    V& giveValRef() { return kv.second; }
    std::pair<K, V>* giveKV() {
      printf("in giveKV\n");
      printf("k %d\n", (int) kv.first);
      printf("k %d\n v %d\n", (int) kv.first, (int) kv.second);
      return &kv;
    }
  };
}
#endif
