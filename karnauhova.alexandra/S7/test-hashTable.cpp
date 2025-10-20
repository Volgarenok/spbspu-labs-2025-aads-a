#include <initializer_list>
#include <string>
#include <utility>
#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "hashTable.hpp"

BOOST_AUTO_TEST_CASE(test_insert_and_find)
{
  karnauhova::HashTable< int, std::string > table;
  BOOST_TEST(table.empty());
  table.insert({ 1, "one" });
  BOOST_TEST(table.size() == 1);
  BOOST_TEST(table.at(1) == "one");
  table[2] = "two";
  BOOST_TEST(table.size() == 2);
}

BOOST_AUTO_TEST_CASE(test_erase)
{
  karnauhova::HashTable< int, std::string > table;
  table.insert({1, "one"});
  table.insert({2, "two"});
  table.erase(table.begin());
  BOOST_TEST(table.size() == 1);
  auto it = table.find(1);
  bool not_found1 = (it == table.end());
  BOOST_TEST(not_found1);
  table.erase(2);
  BOOST_TEST(table.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_iterators)
{
  karnauhova::HashTable< int, std::string > table;
  table.insert({1, "one"});
  table.insert({2, "two"});
  table.insert({3, "three"});
  size_t count = 0;
  for (auto it = table.begin(); it != table.end(); ++it)
  {
    std::string val = it->second;
    bool is_one = (val == "one");
    bool is_two = (val == "two");
    bool is_three = (val == "three");
    bool is_valid = is_one || is_two || is_three;
    BOOST_TEST(is_valid);
    ++count;
  }
  BOOST_TEST(count == 3);
}

BOOST_AUTO_TEST_CASE(test_empty)
{
  karnauhova::HashTable< int, std::string > table;
  BOOST_TEST(table.empty());
  BOOST_TEST(table.size() == 0);
  auto it = table.begin();
  bool is_end = (it == table.end());
  BOOST_TEST(is_end);
}

BOOST_AUTO_TEST_CASE(hashTable)
{
  using Table = karnauhova::HashTable< int, std::string >;
  Table table;
  BOOST_TEST(table.empty());
  table.insert({ 1, "one" });
  BOOST_TEST(table.size() == 1);
  BOOST_TEST(table.at(1) == "one");
  table[2] = "two";
  BOOST_TEST(table.size() == 2);
  Table table2;
  table2.insert({1, "one"});
  table2.insert({3, "three"}); 
  table2.insert({5, "five"});
  BOOST_TEST(table2.size() == 3);
  table = std::move(table2);
  BOOST_TEST(table[5] == "five");
  Table table3(table);
  BOOST_TEST(table3.size() == 3);
  table.erase(table.begin());
  BOOST_TEST(table.begin()->second == "three");
  Table::Iterator it = table.find(7);
  BOOST_CHECK(it == table.end());
  it = table.find(5);
  BOOST_TEST(it->second == "five");
  BOOST_TEST(table.loadFactor() > 0.0);
}
