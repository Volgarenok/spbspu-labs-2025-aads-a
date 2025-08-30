#include <initializer_list>
#include <string>
#include <utility>
#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "hashTable.hpp"
BOOST_AUTO_TEST_CASE(hashTable)
{
  using HTable = brevnov::HashTable< int, std::string >;
  HTable table;
  BOOST_TEST(table.empty());
  table.insert({ 1, "one" });
  BOOST_TEST(table.size() == 1);
  BOOST_TEST(table.at(1) == "one");
  table[2] = "two";
  BOOST_TEST(table.size() == 2);
  HTable table2{ { 1, "one" }, { 5, "five" }, { 9, "nine" } };
  BOOST_TEST(table2.size() == 3);
  table = std::move(table2);
  BOOST_TEST(table[5] == "five");
  HTable table3(table);
  BOOST_TEST(table3.size() == 3);
  table3.clear();
  BOOST_TEST(table3.empty());
  table.erase(table.begin());
  BOOST_TEST(table.begin()->second == "five");
  table.emplace(std::make_pair(6, "six"));
  BOOST_TEST(table.size() == 3);
  table.erase(6);
  BOOST_TEST(table.size() == 2);
  HTable::Iterator it = table.find(7);
  BOOST_CHECK(it == table.end());
  it = table.find(5);
  BOOST_TEST(it->second == "five");
  BOOST_TEST(table.loadFactor() > 0.0);
  BOOST_TEST(table.loadFactor() <= table.maxLoadFactor());
  float maxLoadFactor = table.maxLoadFactor();
  table.maxLoadFactor(0.9);
  BOOST_TEST(maxLoadFactor < table.maxLoadFactor());
}
