#include <boost/test/unit_test.hpp>
#include <boost/test/tools/detail/print_helper.hpp>
#include "hash_table.hpp"

namespace boost::test_tools::tt_detail
{
  template< class Key, class Value, class Hash, class Equal >
  struct print_log_value< abramov::Iterator< Key, Value, Hash, Equal  > >
  {
    void operator()(std::ostream &os, const abramov::Iterator< Key, Value, Hash, Equal > &iter)
    {
      os << "iter";
    }
  };

  template< class Key, class Value, class Hash, class Equal >
  struct print_log_value< abramov::ConstIterator< Key, Value, Hash, Equal > >
  {
    void operator()(std::ostream &os, const abramov::ConstIterator< Key, Value, Hash, Equal > &iter)
    {
      os << "cIter";
    }
  };
}

BOOST_AUTO_TEST_CASE(defaultconstructor_insert_find)
{
  abramov::HashTable< int, std::string > table;
  table.insert(10, "a");
  table.insert(8, "b");
  auto it = table.find(8);
  BOOST_TEST(it->second == "b");
  it = table.find(10);
  BOOST_TEST(it->second == "a");
}

BOOST_AUTO_TEST_CASE(begin)
{
  abramov::HashTable< int, std::string > table;
  table.insert(1, "a");
  table.insert(5, "b");
  auto it = table.begin();
  bool b = (it->second == "a") || (it->second == "b");
  BOOST_TEST(b);
}

BOOST_AUTO_TEST_CASE(cbegin)
{
  abramov::HashTable< int, std::string > table;
  table.insert(5, "a");
  table.insert(1, "b");
  auto it = table.cbegin();
  bool b = (it->second == "a") || (it->second == "b");
  BOOST_TEST(b);
}

BOOST_AUTO_TEST_CASE(end)
{
  abramov::HashTable< int, std::string > table;
  table.insert(1, "a");
  table.insert(5, "b");
  auto it = ++table.begin();
  BOOST_TEST(++it == table.end());
}

BOOST_AUTO_TEST_CASE(cend)
{
  abramov::HashTable< int, std::string > table;
  table.insert(5, "a");
  table.insert(1, "b");
  auto it = ++table.cbegin();
  BOOST_TEST(++it == table.cend());
}

BOOST_AUTO_TEST_CASE(erase)
{
  abramov::HashTable< int, std::string > table;
  table.insert(5, "a");
  table.insert(1, "b");
  auto it = table.find(5);
  BOOST_TEST(it->second == "a");
  table.erase(5);
  it = table.find(5);
  BOOST_TEST(it == table.end());
}

BOOST_AUTO_TEST_CASE(loadfactor_rehash)
{
  abramov::HashTable< int, std::string > table;
  table.insert(10, "a");
  table.insert(8, "b");
  table.insert(5, "a");
  table.insert(1, "b");
  double d1 = table.loadFactor();
  table.rehash(100);
  auto it = table.find(10);
  BOOST_TEST(it->second == "a");
  double d2 = table.loadFactor();
  BOOST_TEST(d2 < d1);
}

BOOST_AUTO_TEST_CASE(size)
{
  abramov::HashTable< int, std::string > table;
  table.insert(10, "a");
  table.insert(8, "b");
  table.insert(5, "a");
  table.insert(1, "b");
  BOOST_TEST(table.size() == 4);
}

BOOST_AUTO_TEST_CASE(empty)
{
  abramov::HashTable< int, std::string > table;
  BOOST_TEST(table.empty());
  table.insert(10, "a");
  table.insert(8, "b");
  BOOST_TEST(!table.empty());
}
