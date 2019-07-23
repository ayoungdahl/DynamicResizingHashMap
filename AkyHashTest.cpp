#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/ui/text/TestRunner.h>
#include <stdio.h>
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

  struct intStringHash {
    size_t operator()(const intString &key) const {
      return (std::hash<int>()(key.num) ^ (std::hash<std::string>()(key.word) << 1)) >> 1;
    }
  };
  
  akyhash::HashMap<int, int> *hm1;
  akyhash::HashMap<std::string, std::string> *hm2;
  akyhash::HashMap<intString, intString, intStringHash> *hm3;
  
  CPPUNIT_TEST_SUITE(AkyHashTest);

  CPPUNIT_TEST(testInsertAndGetString);
  CPPUNIT_TEST(testInsertAndGetIntString);
  //  CPPUNIT_TEST_EXCEPTION(testDivideByZeroThrows);

  CPPUNIT_TEST_SUITE_END();

public:
  
  void setUp() {
    
    hm1 = new akyhash::HashMap<int, int>();
    hm2 = new akyhash::HashMap<std::string, std::string>();
    hm3 = new akyhash::HashMap<intString, intString, intStringHash>();
  }
 
  void tearDown() {
    delete hm1;
    delete hm2;
    delete hm3;
  }

  void testInsertAndGetString() {

    std::string A = "hello!";
    std::string B = "A great value!";
    std::string C = "Happy Chocolate:)";
    std::pair<std::string, bool> rc;

    rc = hm2->insert(A, B);
    
    CPPUNIT_ASSERT(rc.second);
    CPPUNIT_ASSERT(rc.first == B);

    rc = hm2->insert(B, C);

    CPPUNIT_ASSERT(rc.second);
    CPPUNIT_ASSERT(rc.first == C);
   
    rc = hm2->insert(A, C);
    CPPUNIT_ASSERT(!rc.second);
    CPPUNIT_ASSERT(rc.first == B);

    CPPUNIT_ASSERT(hm2->get(A) == B);
    CPPUNIT_ASSERT(hm2->get(B) == C);
    CPPUNIT_ASSERT_THROW(hm2->get(C), std::out_of_range);
    
 }


  void testInsertAndGetIntString() {

    intString A = {7, "hello!"};
    intString B = {11, "A great value!"};
    intString C = {42, "Happy Chocolate:)"};
    std::pair<intString, bool> rc;

    rc = hm3->insert(A, B);
    
    CPPUNIT_ASSERT(rc.second);
    CPPUNIT_ASSERT(rc.first == B);

    rc = hm3->insert(B, C);

    CPPUNIT_ASSERT(rc.second);
    CPPUNIT_ASSERT(rc.first == C);
   
    rc = hm3->insert(A, C);
    CPPUNIT_ASSERT(!rc.second);
    CPPUNIT_ASSERT(rc.first == B);

    CPPUNIT_ASSERT(hm3->get(A) == B);
    CPPUNIT_ASSERT(hm3->get(B) == C);
    CPPUNIT_ASSERT_THROW(hm3->get(C), std::out_of_range);
 }

};

int main(int argc, char **argv) {
  CppUnit::TextUi::TestRunner runner;
  //  CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest(AkyHashTest::suite());
  runner.run();
  return 0;
}
