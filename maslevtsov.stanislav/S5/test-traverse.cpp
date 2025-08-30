#include <boost/test/unit_test.hpp>
#include <tree/definition.hpp>
#include "keys_values_plus.hpp"

BOOST_AUTO_TEST_SUITE(traverse_tests)
BOOST_AUTO_TEST_CASE(traverse_lnr_test)
{
  maslevtsov::Tree< int, std::string > tree;
  maslevtsov::KeysValuesPlus plus;
  try {
    plus = tree.traverse_lnr(plus);
  } catch (const std::logic_error&) {
    BOOST_TEST(true);
  }
  tree.insert(std::make_pair(0, "0"));
  tree.insert(std::make_pair(1, "1"));
  plus = tree.traverse_lnr(plus);
  BOOST_TEST(plus.keys_sum == 1);
  BOOST_TEST(plus.values == " 0 1");
}

BOOST_AUTO_TEST_CASE(traverse_rnl_test)
{
  maslevtsov::Tree< int, std::string > tree;
  maslevtsov::KeysValuesPlus plus;
  try {
    plus = tree.traverse_rnl(plus);
  } catch (const std::logic_error&) {
    BOOST_TEST(true);
  }
  tree.insert(std::make_pair(0, "0"));
  tree.insert(std::make_pair(1, "1"));
  plus = tree.traverse_rnl(plus);
  BOOST_TEST(plus.keys_sum == 1);
  BOOST_TEST(plus.values == " 1 0");
}

BOOST_AUTO_TEST_CASE(traverse_breadth_test)
{
  maslevtsov::Tree< int, std::string > tree;
  maslevtsov::KeysValuesPlus plus;
  try {
    plus = tree.traverse_breadth(plus);
  } catch (const std::logic_error&) {
    BOOST_TEST(true);
  }
  tree.insert(std::make_pair(0, "0"));
  tree.insert(std::make_pair(1, "1"));
  tree.insert(std::make_pair(2, "2"));
  tree.insert(std::make_pair(3, "3"));
  tree.insert(std::make_pair(4, "4"));
  plus = tree.traverse_breadth(plus);
  BOOST_TEST(plus.keys_sum == 10);
  BOOST_TEST(plus.values == " 1 3 0 2 4");
}
BOOST_AUTO_TEST_SUITE_END()
