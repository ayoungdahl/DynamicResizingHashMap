#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/ui/text/TestRunner.h>

#include <iostream>
#include <stdio.h>
#include <unordered_map>

#include "hashmap.h"

class AkyHashTest : public CppUnit::TestFixture {
private:

  struct intString {
    int num;
    std::string word;
    intString() {}
    intString(const int n, const std::string &w) : num(n), word(w) {}
    bool operator==(const intString &other) const {
      return num == other.num && word == other.word;
    }
  };

  struct intStringKeyEQ {
    bool operator()(const intString &lhs, const intString &rhs) const {
      return lhs.num == rhs.num && lhs.word == rhs.word; 
    }
  };
  struct intStringHash {
    size_t operator()(const intString &key) const {
      return (std::hash<int>()(key.num) ^ (std::hash<std::string>()(key.word) << 1)) >> 1;
    }
  };
  
  akyhash::HashMap<int, int> *hm1;
  akyhash::HashMap<std::string, std::string> *hm2;
  akyhash::HashMap<intString, intString, intStringHash, intStringKeyEQ> *hm3;

  std::string s1 = "hello!";
  std::string s2 = "Cabbage!";
  std::string s3 = "Sweetroll!";
  
  CPPUNIT_TEST_SUITE(AkyHashTest);
  CPPUNIT_TEST(testInt);
  //CPPUNIT_TEST(testString);
  //CPPUNIT_TEST(testIntString);
  //CPPUNIT_TEST(test1MInt);
  CPPUNIT_TEST_SUITE_END();

public:
  
  void setUp() {
    
    hm1 = new akyhash::HashMap<int, int>();
    hm2 = new akyhash::HashMap<std::string, std::string>();
    hm3 = new akyhash::HashMap<intString, intString, intStringHash, intStringKeyEQ>();
  }
 
  void tearDown() {
    delete hm1;
    delete hm2;
    delete hm3;
  }

  template<typename K, typename V, typename H = std::hash<K>, typename KEQ = std::equal_to<K>>
  void testIt(akyhash::HashMap<K, V, H, KEQ> *hm, const K &A, const K &B, const K &C) {

    std::pair<V, bool> rc;
    rc = hm->insert(std::make_pair(A, B));

    CPPUNIT_ASSERT(rc.second);
    CPPUNIT_ASSERT(rc.first == B);

    rc = hm->insert(std::pair<K, K>(B, C));

    CPPUNIT_ASSERT(rc.second);
    CPPUNIT_ASSERT(rc.first == C);
   
    rc = hm->insert(std::pair<K, K>(A, C));
    CPPUNIT_ASSERT(!rc.second);
    CPPUNIT_ASSERT(rc.first == B);

    CPPUNIT_ASSERT(hm->get(A) == B);
    CPPUNIT_ASSERT(hm->get(B) == C);
    CPPUNIT_ASSERT_THROW(hm->get(C), std::out_of_range);

    CPPUNIT_ASSERT(hm->count(A) == 1);
    CPPUNIT_ASSERT(hm->count(B) == 1);
    CPPUNIT_ASSERT(hm->count(C) == 0);

    CPPUNIT_ASSERT(hm->erase(B) == 1);
    CPPUNIT_ASSERT(hm->erase(B) == 0);
    CPPUNIT_ASSERT_THROW(hm->get(B), std::out_of_range);
    CPPUNIT_ASSERT(hm->get(A) == B);

    akyhash::HMiterator<K, K> it = hm->begin();
    K key = it->first;
    /*        
    //    std::unordered_map<K, V, H, KEQ> tm;
    int loops = 0;
    for (akyhash::HMiterator<K, K, H, KEQ> it = hm->begin(); it != hm->end(); ++it) {
      //K key = it->first;
      //tm[it->first] = it->second;
      loops++;
    }
    */
    //CPPUNIT_ASSERT(loops == 2);
    //CPPUNIT_ASSERT(tm[A] == B);
    //CPPUNIT_ASSERT(tm[B] == C);
    
    CPPUNIT_ASSERT((*hm)[(K&)A] == B);
    (*hm)[(K&)A] = C;
    CPPUNIT_ASSERT(!(hm->get(A) == B));
    CPPUNIT_ASSERT(hm->get(A) == C);
  }
  void testInt() { testIt(hm1, 7, 11, 42); }
  //void testString() { testIt(hm2, s1, s2, s3); }
  //void testIntString() { testIt(hm3, intString(7, s1), intString(11, s2), intString(42, s3)); }
  
  void test1MInt() {
    const int num = 1e6;
    std::pair<int, bool> rc;

    for (int i = 0; i < num; i++) {
      rc = hm1->insert(std::make_pair(i, i * 10));
      CPPUNIT_ASSERT(rc.second);
      CPPUNIT_ASSERT(rc.first == i * 10);
    }
    
    for (int i = 0; i < num; i++) {
      CPPUNIT_ASSERT(hm1->get(i) == i * 10);
    }
    
  }
  
};

int main(int argc, char **argv) {
  CppUnit::TextUi::TestRunner runner;
  //  CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest(AkyHashTest::suite());
  runner.run();
  return 0;
}
