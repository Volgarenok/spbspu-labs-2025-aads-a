#include <boost/test/unit_test.hpp>
#include <hashTable.hpp>

BOOST_AUTO_TEST_CASE(insert_emplace_access)
{
  mozhegova::HashTable< int, std::string > table;
  BOOST_TEST(table.empty());
  table[5] = "five";
  table.insert(std::make_pair(1, "one"));
  table.emplace(std::make_pair(2, "two"));
  BOOST_TEST(table.size() == 3);
  table.insert(std::make_pair(1, "three"));
  BOOST_TEST(table.size() == 3);
  BOOST_TEST(table[1] == "one");
  BOOST_TEST(table.at(2) == "two");
  BOOST_TEST(table[5] == "five");
}

BOOST_AUTO_TEST_CASE(erase)
{
  mozhegova::HashTable< int, std::string > table;
  table[1] = "one";
  table[2] = "two";
  table[3] = "three";
  BOOST_TEST(table.size() == 3);
  BOOST_TEST(table.erase(2) == 1);
  BOOST_TEST(table.erase(4) == 0);
  BOOST_TEST(table.size() == 2);
  table.erase(table.cbegin());
  BOOST_TEST(table.size() == 1);
}

BOOST_AUTO_TEST_CASE(rehash)
{
  mozhegova::HashTable< int, std::string > table;
  for (int i = 0; i < 20; ++i)
  {
    table[i] = i;
  }
  BOOST_TEST(table.size() == 20);
  BOOST_TEST(table.load_factor() < table.max_load_factor());
}

BOOST_AUTO_TEST_CASE(Clear)
{
  mozhegova::HashTable< int, std::string > table;
  table[1] = "one";
  table[2] = "two";
  BOOST_TEST(!table.empty());
  table.clear();
  BOOST_TEST(table.empty());
}

BOOST_AUTO_TEST_CASE(LoadFactor)
{
  mozhegova::HashTable< int, std::string > table;
  table.max_load_factor(0.5);
  for (int i = 0; i < 6; ++i)
  {
    table[i] = i;
  }
  BOOST_TEST(table.load_factor() <= table.max_load_factor());
}

BOOST_AUTO_TEST_CASE(Swap)
{
  mozhegova::HashTable< int, std::string > table1;
  table1[1] = "one";
  mozhegova::HashTable< int, std::string > table2;
  table2[2] = "two";
  table1.swap(table2);
  BOOST_TEST(table1.find(2)->second == "two");
  BOOST_TEST(table2.find(1)->second == "one");
}
