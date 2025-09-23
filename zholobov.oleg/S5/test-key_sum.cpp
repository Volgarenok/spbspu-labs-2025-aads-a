#include <boost/test/unit_test.hpp>
#include <limits>

#include "key_sum.hpp"

BOOST_AUTO_TEST_SUITE(key_sum)

BOOST_AUTO_TEST_CASE(basic)
{
  zholobov::KeySum< int > key_sum;
  key_sum({1, "one"});
  key_sum({2, "two"});
  key_sum({3, "three"});
  BOOST_TEST(key_sum.sum_ == 6);
  BOOST_TEST(key_sum.values_ == " one two three");
}

BOOST_AUTO_TEST_CASE(overflow_1)
{
  zholobov::KeySum< int > key_sum;
  try {
    key_sum({1, "one"});
    key_sum({4, "four"});
    key_sum({std::numeric_limits< int >::max() - 4, "very_big"});
    BOOST_TEST_ERROR("Should have thrown overflow");
  } catch (const std::overflow_error& e) {
    BOOST_TEST(e.what() == "Numeric overflow");
  }
}

BOOST_AUTO_TEST_CASE(overflow_2)
{
  zholobov::KeySum< int > key_sum;
  try {
    key_sum({-1, "one"});
    key_sum({-4, "four"});
    key_sum({std::numeric_limits< int >::min() + 4, "very_big_negative"});
    BOOST_TEST_ERROR("Should have thrown overflow");
  } catch (const std::overflow_error& e) {
    BOOST_TEST(e.what() == "Numeric overflow");
  }
}

BOOST_AUTO_TEST_SUITE_END()
