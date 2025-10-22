#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include "BinarySearchTree.hpp"

using namespace nikonov;

BOOST_AUTO_TEST_CASE(defaultConstructor)
{
  BinarySearchTree< int, std::string, std::less< int > > tree;
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(copyConstructor)
{
  BinarySearchTree< int, std::string, std::less< int > > tree;
  tree[1] = "first";
  tree[2] = "second";
  tree[3] = "third";
  BinarySearchTree< int, std::string, std::less< int > > copy(tree);
  BOOST_TEST(copy.size() == 3);
  BOOST_TEST(tree.size() == 3);
}

BOOST_AUTO_TEST_CASE(moveConstructor)
{
  BinarySearchTree< int, std::string, std::less< int > > tree;
  tree[1] = "first";
  tree[2] = "second";
  tree[3] = "third";
  BinarySearchTree< int, std::string, std::less< int > > moved(std::move(tree));
  BOOST_TEST(tree.empty());
  BOOST_TEST(moved.size() = 3);
}

BOOST_AUTO_TEST_CASE(copyOperator)
{
  BinarySearchTree< int, std::string, std::less< int > > tree;
  tree[1] = "first";
  tree[2] = "second";
  tree[3] = "third";
  BinarySearchTree< int, std::string, std::less< int > > copy;
  copy = tree;
  BOOST_TEST(copy.size() == 3);
  BOOST_TEST(tree.size() == 3);
}

BOOST_AUTO_TEST_CASE(moveOperator)
{
  BinarySearchTree< int, std::string, std::less< int > > tree;
  tree[1] = "first";
  tree[2] = "second";
  tree[3] = "third";
  BinarySearchTree< int, std::string, std::less< int > > moved;
  moved = std::move(tree);
  BOOST_TEST(moved.size() == 3);
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(operatorSquareBrackets)
{
  BinarySearchTree< int, std::string, std::less< int > > tree;
  tree[1] = "first";
  tree[2] = "second";
  tree[3] = "third";
  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(tree.at(1) == "first");
}

BOOST_AUTO_TEST_CASE(size)
{
  BinarySearchTree< int, std::string, std::less< int > > tree;
  tree[1] = "first";
  tree[2] = "second";
  tree[3] = "third";
  BOOST_TEST(tree.size() == 3);
  tree[2] = "SECOND";
  BOOST_TEST(tree.size() == 3);
}

BOOST_AUTO_TEST_CASE(at)
{
  BinarySearchTree< int, std::string, std::less< int > > tree;
  tree[1] = "first";
  tree[2] = "second";
  tree[3] = "third";
  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(tree[1] == "first");
}

BOOST_AUTO_TEST_CASE(find)
{
  BinarySearchTree< int, std::string, std::less< int > > tree;
  tree[1] = "first";
  tree[2] = "second";
  tree[3] = "third";
  auto it1 = tree.find(1);
  auto it2 = tree.find(4);
  bool flag = (it2 == tree.cend());
  BOOST_TEST(it1->second == "first");
  BOOST_TEST(flag);
}