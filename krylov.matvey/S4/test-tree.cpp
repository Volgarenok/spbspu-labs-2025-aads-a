#include <boost/test/unit_test.hpp>
#include "binaryTree.hpp"

using namespace krylov;

BOOST_AUTO_TEST_CASE(defaultConstructor)
{
  BiTree< int, std::string, std::less< int > > tree;
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(copyConstructor)
{
  BiTree< int, std::string, std::less< int > > tree;
  tree[1] = "first";
  tree[2] = "second";
  tree[3] = "third";
  BiTree< int, std::string, std::less< int > > copy(tree);
  BOOST_TEST(copy.size() == 3);
  BOOST_TEST(tree.size() == 3);
}

BOOST_AUTO_TEST_CASE(moveConstructor)
{
  BiTree< int, std::string, std::less< int > > tree;
  tree[1] = "first";
  tree[2] = "second";
  tree[3] = "third";
  BiTree< int, std::string, std::less< int > > moved(std::move(tree));
  BOOST_TEST(tree.empty());
  BOOST_TEST(moved.size() = 3);
}

BOOST_AUTO_TEST_CASE(copyOperator)
{
  BiTree< int, std::string, std::less< int > > tree;
  tree[1] = "first";
  tree[2] = "second";
  tree[3] = "third";
  BiTree< int, std::string, std::less< int > > copy;
  copy = tree;
  BOOST_TEST(copy.size() == 3);
  BOOST_TEST(tree.size() == 3);
}

BOOST_AUTO_TEST_CASE(moveOperator)
{
  BiTree< int, std::string, std::less< int > > tree;
  tree[1] = "first";
  tree[2] = "second";
  tree[3] = "third";
  BiTree< int, std::string, std::less< int > > moved;
  moved = std::move(tree);
  BOOST_TEST(moved.size() == 3);
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(operatorSquareBrackets)
{
  BiTree< int, std::string, std::less< int > > tree;
  tree[1] = "first";
  tree[2] = "second";
  tree[3] = "third";
  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(tree.at(1) == "first");
}

BOOST_AUTO_TEST_CASE(size)
{
  BiTree< int, std::string, std::less< int > > tree;
  tree[1] = "first";
  tree[2] = "second";
  tree[3] = "third";
  BOOST_TEST(tree.size() == 3);
  tree[2] = "SECOND";
  BOOST_TEST(tree.size() == 3);
}

BOOST_AUTO_TEST_CASE(at)
{
  BiTree< int, std::string, std::less< int > > tree;
  tree[1] = "first";
  tree[2] = "second";
  tree[3] = "third";
  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(tree[1] == "first");
}

BOOST_AUTO_TEST_CASE(find)
{
  BiTree< int, std::string, std::less< int > > tree;
  tree[1] = "first";
  tree[2] = "second";
  tree[3] = "third";
  auto it1 = tree.find(1);
  auto it2 = tree.find(4);
  bool flag = (it2 == tree.cend());
  BOOST_TEST(it1->second == "first");
  BOOST_TEST(flag);
}
