#include <boost/test/unit_test.hpp>
#include "binary_tree.hpp"

BOOST_AUTO_TEST_CASE(pre_increment_const)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(6, "a");
  tree.insert(4, "b");
  tree.insert(8, "c");
  auto it = tree.cbegin();
  BOOST_TEST(it->second == "b");
  BOOST_TEST((++it)->second == "a");
  BOOST_TEST((++it)->second == "c");
}

BOOST_AUTO_TEST_CASE(post_increment_const)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(5, "a");
  tree.insert(3, "b");
  tree.insert(7, "c");
  auto it = tree.cbegin();
  BOOST_TEST((it++)->second == "b");
  BOOST_TEST((it++)->second == "a");
  BOOST_TEST(it->second == "c");
}

BOOST_AUTO_TEST_CASE(pre_decrement_const)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(6, "a");
  tree.insert(4, "b");
  tree.insert(8, "c");
  auto it = tree.cfind(8);
  BOOST_TEST(it->second == "c");
  BOOST_TEST((--it)->second == "a");
  BOOST_TEST((--it)->second == "b");
}

BOOST_AUTO_TEST_CASE(post_decrement_const)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(5, "a");
  tree.insert(3, "b");
  tree.insert(7, "c");
  auto it = tree.cfind(7);
  BOOST_TEST((it--)->second == "c");
  BOOST_TEST((it--)->second == "a");
  BOOST_TEST(it->second == "b");
}

BOOST_AUTO_TEST_CASE(equality_const)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(6, "a");
  tree.insert(4, "b");
  tree.insert(8, "c");
  auto it = tree.cfind(6);
  auto iter = ++tree.cbegin();
  bool b = (it == iter);
  BOOST_TEST(b);
}

BOOST_AUTO_TEST_CASE(inequality_const)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(6, "a");
  tree.insert(4, "b");
  tree.insert(8, "c");
  auto it1 = tree.cfind(6);
  auto it2 = tree.cfind(4);
  bool b = (it1 != it2);
  BOOST_TEST(b);
}

BOOST_AUTO_TEST_CASE(dereferencing_const)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(0, "a");
  auto it = tree.cbegin();
  BOOST_TEST((*it).second == "a");
}

BOOST_AUTO_TEST_CASE(arrow_operator_const)
{
  struct point_t
  {
    int x;
    int y;
  };

  abramov::BinarySearchTree< int, point_t > tree;
  point_t p{ 1, 2};
  tree.insert(6, p);
  auto it = tree.cbegin();
  BOOST_TEST(it->second.y == 2);
}
