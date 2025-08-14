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

