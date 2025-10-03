#include <boost/test/unit_test.hpp>
#include "binary_tree.hpp"

BOOST_AUTO_TEST_CASE(pre_increment)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(6, "a");
  tree.insert(4, "b");
  tree.insert(8, "c");
  auto it = tree.begin();
  BOOST_TEST(it->second == "b");
  BOOST_TEST((++it)->second == "a");
  BOOST_TEST((++it)->second == "c");
}

BOOST_AUTO_TEST_CASE(post_increment)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(5, "a");
  tree.insert(3, "b");
  tree.insert(7, "c");
  auto it = tree.begin();
  BOOST_TEST((it++)->second == "b");
  BOOST_TEST((it++)->second == "a");
  BOOST_TEST(it->second == "c");
}

BOOST_AUTO_TEST_CASE(pre_decrement)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(6, "a");
  tree.insert(4, "b");
  tree.insert(8, "c");
  auto it = tree.find(8);
  BOOST_TEST(it->second == "c");
  BOOST_TEST((--it)->second == "a");
  BOOST_TEST((--it)->second == "b");
}

BOOST_AUTO_TEST_CASE(post_decrement)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(5, "a");
  tree.insert(3, "b");
  tree.insert(7, "c");
  auto it = tree.find(7);
  BOOST_TEST((it--)->second == "c");
  BOOST_TEST((it--)->second == "a");
  BOOST_TEST(it->second == "b");
}

BOOST_AUTO_TEST_CASE(equality)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(6, "a");
  tree.insert(4, "b");
  tree.insert(8, "c");
  auto it = tree.find(6);
  auto iter = ++tree.begin();
  bool b = (it == iter);
  BOOST_TEST(b);
}

BOOST_AUTO_TEST_CASE(inequality)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(6, "a");
  tree.insert(4, "b");
  tree.insert(8, "c");
  auto it1 = tree.find(6);
  auto it2 = tree.find(4);
  bool b = (it1 != it2);
  BOOST_TEST(b);
}

BOOST_AUTO_TEST_CASE(dereferencing)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(0, "a");
  auto it = tree.begin();
  BOOST_TEST((*it).second == "a");
}

BOOST_AUTO_TEST_CASE(arrow_operator)
{
  struct point_t
  {
    int x;
    int y;
  };

  abramov::BinarySearchTree< int, point_t > tree;
  point_t p{ 1, 2};
  tree.insert(6, p);
  auto it = tree.begin();
  BOOST_TEST(it->second.y == 2);
}
