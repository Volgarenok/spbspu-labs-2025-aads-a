#include <boost/test/unit_test.hpp>
#include <sstream>
#include <hash_table/hashTable.hpp>

namespace
{
  template< class Key, class T, class HS1, class HS2, class EQ >
  void printHashTable(std::ostream & out, const sharifullina::HashTable< Key, T, HS1, HS2, EQ > & hashTable)
  {
    if (hashTable.empty())
    {
      return;
    }
    auto it = hashTable.begin();
    out << it->first << ' ' << it->second;
    for (++it; it != hashTable.end(); ++it)
    {
      out << ' ' << it->first << ' ' << it->second;
    }
  }
}

BOOST_AUTO_TEST_SUITE(constructors)

BOOST_AUTO_TEST_CASE(defaultConstructor)
{
  sharifullina::HashTable< int, std::string > hashTable;
  BOOST_TEST(hashTable.empty());
  BOOST_TEST(hashTable.size() == 0);
  BOOST_TEST(hashTable.loadFactor() == 0.0f);
}

BOOST_AUTO_TEST_CASE(copyConstructor)
{
  sharifullina::HashTable< int, std::string > hashTable1;
  hashTable1.insert(1, "one");
  hashTable1.insert(2, "two");
  sharifullina::HashTable< int, std::string > hashTable2(hashTable1);
  std::ostringstream out;
  printHashTable(out, hashTable2);
  BOOST_TEST(out.str() == "1 one 2 two");
}

BOOST_AUTO_TEST_CASE(moveConstructor)
{
  sharifullina::HashTable< int, std::string > hashTable1;
  hashTable1.insert(1, "one");
  hashTable1.insert(2, "two");
  sharifullina::HashTable< int, std::string > hashTable2(std::move(hashTable1));
  std::ostringstream out;
  printHashTable(out, hashTable2);
  BOOST_TEST(out.str() == "1 one 2 two");
  BOOST_TEST(hashTable1.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(operators)

BOOST_AUTO_TEST_CASE(copyOperator)
{
  sharifullina::HashTable< int, std::string > hashTable1;
  hashTable1.insert(1, "one");
  hashTable1.insert(2, "two");
  sharifullina::HashTable< int, std::string > hashTable2;
  hashTable2 = hashTable1;
  std::ostringstream out;
  printHashTable(out, hashTable2);
  BOOST_TEST(out.str() == "1 one 2 two");
}

BOOST_AUTO_TEST_CASE(moveOperator)
{
  sharifullina::HashTable< int, std::string > hashTable1;
  hashTable1.insert(1, "one");
  hashTable1.insert(2, "two");
  sharifullina::HashTable< int, std::string > hashTable2;
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
  sharifullina::HashTable< int, std::string > hashTable;
  hashTable.insert(1, "one");
  hashTable.insert(2, "two");
  hashTable.rehash(10);
  BOOST_TEST(hashTable.size() == 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(modifiers)

BOOST_AUTO_TEST_CASE(insert)
{
  sharifullina::HashTable< int, std::string > hashTable;
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
  sharifullina::HashTable< int, std::string > hashTable;
  hashTable.insert(1, "one");
  hashTable.insert(2, "two");
  hashTable.insert(3, "three");
  BOOST_TEST(hashTable.size() == 3);
}

BOOST_AUTO_TEST_CASE(erase)
{
  sharifullina::HashTable< int, std::string > hashTable;
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
}

BOOST_AUTO_TEST_CASE(swap)
{
  sharifullina::HashTable< int, std::string > hashTable1;
  hashTable1.insert(1, "one");
  sharifullina::HashTable< int, std::string > hashTable2;
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
  sharifullina::HashTable< int, std::string > hashTable;
  hashTable.insert(1, "one");
  hashTable.insert(2, "two");
  BOOST_TEST(hashTable.at(1) == "one");
  BOOST_TEST(hashTable.at(2) == "two");
}

BOOST_AUTO_TEST_CASE(find)
{
  sharifullina::HashTable< int, std::string > hashTable;
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
