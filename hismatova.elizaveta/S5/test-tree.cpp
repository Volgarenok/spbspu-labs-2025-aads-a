#include <boost/test/unit_test.hpp>
#include "BinaryTree.hpp"
using namespace hismatova;
BOOST_AUTO_TEST_SUITE(BinaryTreeTests)

BOOST_AUTO_TEST_CASE(ascending_order_test)
{
  BinaryTree< int, std::string > tree;
  tree.insert(10, "keyboard");
  tree.insert(12, "monitor");
  tree.insert(-5, "mouse");

  std::vector< std::string > result;
  tree.traverseAscending([&](int, const std::string& val){ result.push_back(val); });

  BOOST_CHECK_EQUAL(result[0], "mouse");
  BOOST_CHECK_EQUAL(result[1], "keyboard");
  BOOST_CHECK_EQUAL(result[2], "monitor");
}

BOOST_AUTO_TEST_CASE(descending_order_test)
{
  BinaryTree< int, std::string > tree;
  tree.insert(10, "A");
  tree.insert(12, "B");
  tree.insert(-5, "C");

  std::vector< std::string > result;
  tree.traverseDescending([&](int, const std::string& val){ result.push_back(val); });

  BOOST_CHECK_EQUAL(result[0], "B");
  BOOST_CHECK_EQUAL(result[2], "C");
}

BOOST_AUTO_TEST_CASE(breadth_order_test)
{
  BinaryTree< int, std::string > tree;
  tree.insert(10, "root");
  tree.insert(5, "left");
  tree.insert(15, "right");

  std::vector< std::string > result;
  tree.traverseBreadth([&](int, const std::string& val){ result.push_back(val); });

  BOOST_CHECK_EQUAL(result[0], "root");
  BOOST_CHECK_EQUAL(result[1], "left");
  BOOST_CHECK_EQUAL(result[2], "right");
}
BOOST_AUTO_TEST_SUITE_END()
