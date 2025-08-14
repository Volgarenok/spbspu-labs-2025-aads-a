#include <boost/test/unit_test.hpp>
#include "hash-table.hpp"

using aleksandrov::HashTable;

BOOST_AUTO_TEST_SUITE(hashtable_construction)

BOOST_AUTO_TEST_CASE(default_construction)
{
  HashTable< int, int > hashtable;
  BOOST_TEST(hashtable.empty());
  BOOST_TEST(hashtable.size() == 0);
  BOOST_TEST(hashtable.loadFactor() == 0);
  BOOST_TEST(hashtable.maxLoadFactor() == aleksandrov::maxLoadFactorValue);
}

BOOST_AUTO_TEST_SUITE_END();

