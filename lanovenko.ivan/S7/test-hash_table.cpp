#include <boost/test/unit_test.hpp>
#include <sstream>
#include "hash_table.hpp"

namespace
{
  template< class Key, class T, class HS1, class HS2, class EQ >
  void printHashTable(std::ostream & out, const lanovenko::HashTable< Key, T, HS1, HS2, EQ > & hashTable)
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
  lanovenko::HashTable< int, std::string > hashTable;
  BOOST_TEST(hashTable.empty());
  BOOST_TEST(hashTable.size() == 0);
  BOOST_TEST(hashTable.loadFactor() == 0.0f);
}

BOOST_AUTO_TEST_CASE(copyConstructor)
{
  lanovenko::HashTable< int, std::string > hashTable1;
  hashTable1.insert(1, "one");
  hashTable1.insert(2, "two");
  lanovenko::HashTable< int, std::string > hashTable2(hashTable1);
  std::ostringstream out;
  printHashTable(out, hashTable2);
  BOOST_TEST(out.str() == "1 one 2 two");
}

BOOST_AUTO_TEST_CASE(moveConstructor)
{
  lanovenko::HashTable< int, std::string > hashTable1;
  hashTable1.insert(1, "one");
  hashTable1.insert(2, "two");
  lanovenko::HashTable< int, std::string > hashTable2(std::move(hashTable1));
  std::ostringstream out;
  printHashTable(out, hashTable2);
  BOOST_TEST(out.str() == "1 one 2 two");
  BOOST_TEST(hashTable1.empty());
}

BOOST_AUTO_TEST_CASE(rangeConstructor)
{
  lanovenko::HashTable< int, std::string > hashTable1;
  hashTable1.insert(1, "one");
  hashTable1.insert(2, "two");
  auto first = hashTable1.begin();
  auto last = hashTable1.end();
  lanovenko::HashTable< int, std::string > hashTable2(first, last);
  std::ostringstream out1;
  printHashTable(out1, hashTable1);
  std::ostringstream out2;
  printHashTable(out2, hashTable2);
  BOOST_TEST(out1.str() == out2.str());
}

BOOST_AUTO_TEST_CASE(initializerListconstructor)
{
  std::initializer_list< std::pair< int, std::string > > il{{1, "one"}, {2, "two"}};
  lanovenko::HashTable< int, std::string > hashTable1(il);
  lanovenko::HashTable< int, std::string > hashTable2;
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
  lanovenko::HashTable< int, std::string > hashTable1;
  hashTable1.insert(1, "one");
  hashTable1.insert(2, "two");
  lanovenko::HashTable< int, std::string > hashTable2;
  hashTable2 = hashTable1;
  std::ostringstream out;
  printHashTable(out, hashTable2);
  BOOST_TEST(out.str() == "1 one 2 two");
}

BOOST_AUTO_TEST_CASE(moveOperator)
{
  lanovenko::HashTable< int, std::string > hashTable1;
  hashTable1.insert(1, "one");
  hashTable1.insert(2, "two");
  lanovenko::HashTable< int, std::string > hashTable2;
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
  lanovenko::HashTable< int, std::string > hashTable(6);
  hashTable.insert(1, "one");
  hashTable.insert(2, "two");
  hashTable.rehash(10);
  BOOST_TEST(hashTable.size() == 2);
}

BOOST_AUTO_TEST_CASE(maxLoadFactor)
{
  lanovenko::HashTable< int, std::string > hashTable;
  BOOST_TEST(hashTable.max_load_factor() == 0.7f);
  hashTable.max_load_factor(0.5f);
  BOOST_TEST(hashTable.max_load_factor() == 0.5f);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(modifiers)

BOOST_AUTO_TEST_CASE(insert)
{
  lanovenko::HashTable< int, std::string > hashTable;
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
  lanovenko::HashTable< int, std::string > hashTable(2);
  hashTable.insert(1, "one");
  hashTable.insert(2, "two");
  hashTable.insert(3, "three");
  BOOST_TEST(hashTable.size() == 3);
}

BOOST_AUTO_TEST_CASE(insertRange)
{
  lanovenko::HashTable< int, std::string > hashTable1;
  lanovenko::HashTable< int, std::string > hashTable2;
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
  lanovenko::HashTable< int, std::string > hashTable;
  hashTable.insert(1, "one");
  hashTable.insert(2, "two");
  auto hint = hashTable.cbegin();
  auto it1 = hashTable.emplace_hint(hint, 1, "first");
  BOOST_TEST(it1->first == 1);
  BOOST_TEST(it1->second == "one");

  auto it2 = hashTable.emplace_hint(hint, 3, "three");
  BOOST_TEST(it2->first == 3);
  BOOST_TEST(it2->second == "three");
}

BOOST_AUTO_TEST_CASE(clear)
{
  lanovenko::HashTable< int, std::string > hashTable(2);
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

BOOST_AUTO_TEST_SUITE_END()
