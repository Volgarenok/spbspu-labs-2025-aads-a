#include <boost/test/unit_test.hpp>
#include <dynamicArr.hpp>
#include <hashTable.hpp>

BOOST_AUTO_TEST_CASE(constr_and_insert)
{
  finaev::HashTable< int, std::string > table;
  BOOST_TEST(table.empty());
  table[1] = "one";
  table.insert(std::make_pair(2, "two"));
  BOOST_TEST(table.size() == 2);
  BOOST_TEST(table[1] == "one");
  BOOST_TEST(table.at(2) == "two");
}

BOOST_AUTO_TEST_CASE(erase_test)
{
  finaev::HashTable< int, std::string > table;
  table[1] = "one";
  table.insert(std::make_pair(2, "two"));
  BOOST_TEST(table.erase(2) == 1);
  BOOST_TEST(table.erase(3) == 0);
  table.erase(table.begin());
  BOOST_TEST(table.size() == 0);
}

BOOST_AUTO_TEST_CASE(find_test)
{
  finaev::HashTable< int, std::string > table;
  table[1] = "one";
  table.insert(std::make_pair(2, "two"));
  BOOST_TEST(table.find(2)->second == "two");
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  finaev::HashTable< int, std::string > table1;
  table1[1] = "one";
  finaev::HashTable< int, std::string > table2;
  table2.insert(std::make_pair(2, "two"));
  table1.swap(table2);
  BOOST_TEST(table1.find(2)->second == "two");
  BOOST_TEST(table2.find(1)->second == "one");
}

BOOST_AUTO_TEST_CASE(rehashToSmaller_test)
{
  finaev::HashTable< int, std::string > table;
  table[1] = "one";
  table.insert(std::make_pair(2, "two"));
  size_t original_size = table.size();
  table.rehash(original_size - 1);
  BOOST_TEST(table.size() == original_size);
}

BOOST_AUTO_TEST_CASE(rehash_test)
{
  finaev::HashTable< int, std::string > table;
  table[1] = "one";
  table.insert(std::make_pair(2, "two"));
  table[3] = "three";
  size_t old_size = table.size();
  table.rehash(old_size * 2);
  BOOST_TEST(table.size() == old_size);
}
