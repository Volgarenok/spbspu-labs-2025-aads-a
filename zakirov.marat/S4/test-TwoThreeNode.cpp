#include <boost/test/unit_test.hpp>
#include "TwoThreeNode.hpp"

BOOST_AUTO_TEST_CASE(stub_test_queue)
{
  BOOST_TEST(true);
}
/*
BOOST_AUTO_TEST_CASE(one_pair_constructor)
{
  zakirov::TwoThreeNode< int, size_t > test_node(5, 2);
  BOOST_TEST(test_node.first_value_.first == 5);
  BOOST_TEST(test_node.first_value_.second == 2);
  BOOST_TEST(test_node.second_exists_ == false);
}

BOOST_AUTO_TEST_CASE(straight_two_pair_constructor)
{
  zakirov::TwoThreeNode< int, size_t > test_node(5, 2, 6, 9);
  BOOST_TEST(test_node.first_value_.first == 5);
  BOOST_TEST(test_node.first_value_.second == 2);
  BOOST_TEST(test_node.first_value_.first == 6);
  BOOST_TEST(test_node.first_value_.second == 9);
  BOOST_TEST(test_node.second_exists_ == true);
}

BOOST_AUTO_TEST_CASE(reverce_two_pair_constructor)
{
  zakirov::TwoThreeNode< int, size_t > test_node(6, 9, 5, 2);
  BOOST_TEST(test_node.first_value_.first == 5);
  BOOST_TEST(test_node.first_value_.second == 2);
  BOOST_TEST(test_node.first_value_.first == 6);
  BOOST_TEST(test_node.first_value_.second == 9);
  BOOST_TEST(test_node.second_exists_ == true);
}
*/
