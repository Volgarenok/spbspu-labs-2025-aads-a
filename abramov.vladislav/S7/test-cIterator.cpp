#include <boost/test/unit_test.hpp>
#include "hash_table.hpp"

BOOST_AUTO_TEST_CASE(pre_increment_const)
{
  abramov::HashTable< int, std::string > table;
  table.insert(6, "a");
  table.insert(4, "b");
  table.insert(8, "c");
  auto it = ++table.cfind(6);
  bool b = true;
  if (it != table.cend())
  {
    b = (it->second == "b") || (it->second == "c");
  }
  BOOST_TEST(b);
}

BOOST_AUTO_TEST_CASE(post_increment_const)
{
  abramov::HashTable< int, std::string > table;
  table.insert(6, "a");
  table.insert(4, "b");
  table.insert(8, "c");
  auto it = table.cfind(6);
  it++;
  bool b = true;
  if (it != table.cend())
  {
    b = (it->second == "b") || (it->second == "c");
  }
  BOOST_TEST(b);
}

BOOST_AUTO_TEST_CASE(equality_const)
{
  abramov::HashTable< int, std::string > table;
  table.insert(6, "a");
  bool b = ++table.cfind(6) == table.cend();
  BOOST_TEST(b);
}

BOOST_AUTO_TEST_CASE(inequality_const)
{
  abramov::HashTable< int, std::string > table;
  table.insert(6, "a");
  bool b = table.cfind(6) != table.cend();
  BOOST_TEST(b);
}

BOOST_AUTO_TEST_CASE(dereferencing_const)
{
  abramov::HashTable< int, std::string > table;
  table.insert(0, "a");
  auto it = table.cfind(0);
  BOOST_TEST((*it).second == "a");
}

BOOST_AUTO_TEST_CASE(arrow_operator_const)
{
  struct point_t
  {
    int x;
    int y;
  };

  abramov::HashTable< int, point_t > table;
  point_t p{ 1, 2 };
  table.insert(6, p);
  auto it = table.cfind(6);
  BOOST_TEST(it->second.y == 2);
}
