#include <boost/test/unit_test.hpp>
#include <sstream>
#include "hashTable.hpp"

namespace
{
  template< class Key, class T, class HS1, class HS2, class EQ >
  void printHashTable(std::ostream & out, const maslov::HashTable< Key, T, HS1, HS2, EQ > & hashTable)
  {
    if (hashTable.empty())
    {
      return;
    }
    auto it = hashTable.cbegin();
    out << it->first << ' ' << it->second;
    for (++it; it != hashTable.cend(); ++it)
    {
      out << ' ' << it->first << ' ' << it->second;
    }
  }
}

BOOST_AUTO_TEST_SUITE(constructors)

BOOST_AUTO_TEST_CASE(defaultConstructor)
{
  maslov::HashTable< int, std::string > hashTable;
  BOOST_TEST(hashTable.empty());
  BOOST_TEST(hashTable.size() == 0);
  BOOST_TEST(hashTable.loadFactor() == 0.0f);
}

BOOST_AUTO_TEST_CASE(copyConstructor)
{
  maslov::HashTable< int, std::string > hashTable1;
  hashTable1.insert(1, "one");
  hashTable1.insert(2, "two");
  maslov::HashTable< int, std::string > hashTable2(hashTable1);
  std::ostringstream out;
  printHashTable(out, hashTable2);
  BOOST_TEST(out.str() == "1 one 2 two");
}

BOOST_AUTO_TEST_CASE(moveConstructor)
{
  maslov::HashTable< int, std::string > hashTable1;
  hashTable1.insert(1, "one");
  hashTable1.insert(2, "two");
  maslov::HashTable< int, std::string > hashTable2(std::move(hashTable1));
  std::ostringstream out;
  printHashTable(out, hashTable2);
  BOOST_TEST(out.str() == "1 one 2 two");
  BOOST_TEST(hashTable1.empty());
}

BOOST_AUTO_TEST_CASE(rangeConstructor)
{
  maslov::HashTable< int, std::string > hashTable1;
  hashTable1.insert(1, "one");
  hashTable1.insert(2, "two");
  auto first = hashTable1.begin();
  auto last = hashTable1.end();
  maslov::HashTable< int, std::string > hashTable2(first, last);
  std::ostringstream out1;
  printHashTable(out1, hashTable1);
  std::ostringstream out2;
  printHashTable(out2, hashTable2);
  BOOST_TEST(out1.str() == out2.str());
}

BOOST_AUTO_TEST_CASE(initializerListconstructor)
{
  std::initializer_list< std::pair< int, std::string > > il{{1, "one"}, {2, "two"}};
  maslov::HashTable< int, std::string > hashTable1(il);
  maslov::HashTable< int, std::string > hashTable2;
  hashTable2.insert(1, "one");
  hashTable2.insert(2, "two");
  std::ostringstream out1;
  printHashTable(out1, hashTable1);
  std::ostringstream out2;
  printHashTable(out2, hashTable2);
  BOOST_TEST(out1.str() == out2.str());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(operators)

BOOST_AUTO_TEST_CASE(copyOperator)
{
  maslov::HashTable< int, std::string > hashTable1;
  hashTable1.insert(1, "one");
  hashTable1.insert(2, "two");
  maslov::HashTable< int, std::string > hashTable2;
  hashTable2 = hashTable1;
  std::ostringstream out;
  printHashTable(out, hashTable2);
  BOOST_TEST(out.str() == "1 one 2 two");
}

BOOST_AUTO_TEST_CASE(moveOperator)
{
  maslov::HashTable< int, std::string > hashTable1;
  hashTable1.insert(1, "one");
  hashTable1.insert(2, "two");
  maslov::HashTable< int, std::string > hashTable2;
  hashTable2 = std::move(hashTable1);
  std::ostringstream out;
  printHashTable(out, hashTable2);
  BOOST_TEST(out.str() == "1 one 2 two");
  BOOST_TEST(hashTable1.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(hashPolicy)

BOOST_AUTO_TEST_CASE(rehash)
{
  maslov::HashTable< int, std::string > hashTable(6);
  hashTable.insert(1, "one");
  hashTable.insert(2, "two");
  hashTable.rehash(10);
  BOOST_TEST(hashTable.size() == 2);
}

BOOST_AUTO_TEST_CASE(maxLoadFactor)
{
  maslov::HashTable< int, std::string > hashTable;
  BOOST_TEST(hashTable.maxLoadFactor() == 0.7f);
  hashTable.maxLoadFactor(0.5f);
  BOOST_TEST(hashTable.maxLoadFactor() == 0.5f);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(modifiers)

BOOST_AUTO_TEST_CASE(insert)
{
  maslov::HashTable< int, std::string > hashTable;
  auto it1 = hashTable.insert(1, "one");
  BOOST_TEST(it1.second == true);
  BOOST_TEST(it1.first->second == "one");
  BOOST_TEST(hashTable.size() == 1);

  auto it2 = hashTable.insert(2, "two");
  BOOST_TEST(it2.second == true);
  BOOST_TEST(hashTable.size() == 2);

  auto it3 = hashTable.insert(1, "first");
  BOOST_TEST(it3.second == false);
  BOOST_TEST(it3.first->second == "one");
  BOOST_TEST(hashTable.size() == 2);
}

BOOST_AUTO_TEST_CASE(insertRehash)
{
  maslov::HashTable< int, std::string > hashTable(2);
  hashTable.insert(1, "one");
  hashTable.insert(2, "two");
  hashTable.insert(3, "three");
  BOOST_TEST(hashTable.size() == 3);
}

BOOST_AUTO_TEST_CASE(insertRange)
{
  maslov::HashTable< int, std::string > hashTable1;
  maslov::HashTable< int, std::string > hashTable2;
  hashTable1.insert(1, "one");
  hashTable1.insert(2, "two");
  hashTable1.insert(3, "three");
  auto first = hashTable1.begin();
  auto last = hashTable1.find(3);
  hashTable2.insert(first, last);
  std::ostringstream out;
  printHashTable(out, hashTable2);
  BOOST_TEST(out.str() == "1 one 2 two");
}

BOOST_AUTO_TEST_CASE(emplaceHint)
{
  maslov::HashTable< int, std::string > hashTable;
  hashTable.insert(1, "one");
  hashTable.insert(2, "two");
  auto hint = hashTable.cbegin();
  auto it1 = hashTable.emplaceHint(hint, 1, "first");
  BOOST_TEST(it1->first == 1);
  BOOST_TEST(it1->second == "one");

  auto it2 = hashTable.emplaceHint(hint, 3, "three");
  BOOST_TEST(it2->first == 3);
  BOOST_TEST(it2->second == "three");
}

BOOST_AUTO_TEST_CASE(clear)
{
  maslov::HashTable< int, std::string > hashTable(2);
  hashTable.insert(1, "one");
  hashTable.insert(2, "two");
  hashTable.insert(3, "three");
  hashTable.clear();
  BOOST_TEST(hashTable.size() == 0);
  hashTable.insert(1, "first");
  std::ostringstream out;
  printHashTable(out, hashTable);
  BOOST_TEST(out.str() == "1 first");
}

BOOST_AUTO_TEST_CASE(erase)
{
  maslov::HashTable< int, std::string > hashTable;
  hashTable.insert(1, "one");
  hashTable.insert(2, "two");
  hashTable.insert(3, "three");

  auto it = hashTable.find(2);
  auto nextIt = hashTable.erase(it);
  BOOST_TEST(nextIt->first == 3);
  std::ostringstream out1;
  printHashTable(out1, hashTable);
  BOOST_TEST(out1.str() == "1 one 3 three");

  size_t num = hashTable.erase(3);
  BOOST_TEST(num == 1);
  std::ostringstream out2;
  printHashTable(out2, hashTable);
  BOOST_TEST(out2.str() == "1 one");

  hashTable.insert(4, "four");
  auto first = hashTable.find(1);
  auto last = hashTable.find(4);
  hashTable.erase(first, last);
  std::ostringstream out3;
  printHashTable(out3, hashTable);
  BOOST_TEST(out3.str() == "4 four");
}

BOOST_AUTO_TEST_CASE(swap)
{
  maslov::HashTable< int, std::string > hashTable1;
  hashTable1.insert(1, "one");
  maslov::HashTable< int, std::string > hashTable2;
  hashTable2.insert(2, "two");
  hashTable1.swap(hashTable2);
  std::ostringstream out1;
  printHashTable(out1, hashTable2);
  BOOST_TEST(out1.str() == "1 one");
  std::ostringstream out2;
  printHashTable(out2, hashTable1);
  BOOST_TEST(out2.str() == "2 two");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(lookup)

BOOST_AUTO_TEST_CASE(at)
{
  maslov::HashTable< int, std::string > hashTable;
  hashTable.insert(1, "one");
  hashTable.insert(2, "two");
  BOOST_TEST(hashTable.at(1) == "one");
  BOOST_TEST(hashTable.at(2) == "two");
}

BOOST_AUTO_TEST_CASE(operatorSquareBrackets)
{
  maslov::HashTable< int, std::string > hashTable;
  hashTable[1] = "one";
  hashTable[2] = "two";
  BOOST_TEST(hashTable[1] == "one");
  BOOST_TEST(hashTable[2] == "two");
  BOOST_TEST(hashTable.size() == 2);
}

BOOST_AUTO_TEST_CASE(find)
{
  maslov::HashTable< int, std::string > hashTable;
  bool check1 = hashTable.find(1) == hashTable.end();
  BOOST_TEST(check1);
  hashTable.insert(1, "one");
  hashTable.insert(2, "two");
  auto it1 = hashTable.find(1);
  BOOST_TEST(it1->first == 1);
  BOOST_TEST(it1->second == "one");
  auto it2 = hashTable.find(3);
  bool check2 = it2 == hashTable.end();
  BOOST_TEST(check2);
}

BOOST_AUTO_TEST_SUITE_END()
