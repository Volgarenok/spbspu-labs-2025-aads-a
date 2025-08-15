#include <boost/test/unit_test.hpp>
#include "hash-table.hpp"

using aleksandrov::HashTable;

BOOST_AUTO_TEST_SUITE(hash_table_construction)

BOOST_AUTO_TEST_CASE(default_construction)
{
  HashTable< int, int > hashtable;
  BOOST_TEST(hashtable.empty());
  BOOST_TEST(hashtable.size() == 0);
  BOOST_TEST(hashtable.loadFactor() == 0);
  BOOST_TEST(hashtable.maxLoadFactor() == aleksandrov::maxLoadFactorValue);
}

BOOST_AUTO_TEST_CASE(copy_construction)
{
  HashTable< int, int > ht1;
  HashTable< int, int > ht2(ht1);
  BOOST_TEST(ht2.empty());
}

BOOST_AUTO_TEST_CASE(move_construction)
{
  HashTable< int, int > ht1;
  HashTable< int, int > ht2(std::move(ht1));
  BOOST_TEST(ht2.empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(hash_table_assignment)

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  HashTable< int, int > ht1;
  HashTable< int, int > ht2;
  ht2 = ht1;
  BOOST_TEST(ht2.empty());
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  HashTable< int, int > ht1;
  HashTable< int, int > ht2;
  ht2 = std::move(ht1);
  BOOST_TEST(ht2.empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(hash_table_iterators)

BOOST_AUTO_TEST_CASE(begin_end)
{
  HashTable< int, int > hashtable;
  BOOST_CHECK(hashtable.begin() == hashtable.end());
  BOOST_CHECK(hashtable.cbegin() == hashtable.cend());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(hash_table_element_access)

BOOST_AUTO_TEST_CASE(at)
{
  HashTable< int, int > hashtable;
  BOOST_CHECK_THROW(hashtable.at(0), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(hash_table_capacity)

BOOST_AUTO_TEST_CASE(empty)
{
  HashTable< int, int > hashtable;
  BOOST_TEST(hashtable.empty());
}

BOOST_AUTO_TEST_CASE(size)
{
  HashTable< int, int > hashtable;
  BOOST_TEST(hashtable.size() == 0);
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(hash_table_modifiers)

BOOST_AUTO_TEST_CASE(clear)
{
  HashTable< int, int > hashtable;
  hashtable.clear();
  hashtable.clear();
  BOOST_TEST(hashtable.empty());
}

BOOST_AUTO_TEST_CASE(swap)
{
  HashTable< int, int > ht1;
  HashTable< int, int > ht2;
  ht1.swap(ht2);
  ht2.swap(ht1);
  BOOST_TEST(ht1.empty());
  BOOST_TEST(ht2.empty());
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE(hash_table_lookup)

BOOST_AUTO_TEST_CASE(find)
{
  HashTable< int, int > hashtable;
  BOOST_CHECK(hashtable.find(0) == hashtable.end());
  BOOST_CHECK(hashtable.find(1) == hashtable.end());
}

BOOST_AUTO_TEST_SUITE_END();

