#include <boost/test/unit_test.hpp>
#include "BiTree.hpp"

using StringTree = averenkov::Tree< int, std::string, std::less< int > >;

std::string stringOfTree(const StringTree& tree)
{
  std::string str;
  for (auto i = tree.begin(); i != tree.end(); i++)
  {
    str += i.operator*().second;
  }
  return str;
}

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  StringTree tree;
  BOOST_TEST(tree.empty());
}


BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  StringTree tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");

  StringTree tree2(tree1);
  BOOST_TEST(stringOfTree(tree1) == "onetwo");
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  StringTree tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");

  StringTree tree2(std::move(tree1));
  BOOST_TEST(stringOfTree(tree2) == "onetwo");
  BOOST_TEST(tree1.empty());
}

BOOST_AUTO_TEST_CASE(OperatorAssig)
{
  StringTree tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");

  StringTree tree2;
  tree2 = tree1;
  BOOST_TEST(stringOfTree(tree2) == "onetwo");
  BOOST_TEST(stringOfTree(tree1) == "onetwo");
}

BOOST_AUTO_TEST_CASE(SizeEmpty)
{
  StringTree tree;
  BOOST_TEST(tree.empty());

  tree.push(1, "one");
  BOOST_TEST(tree.size() == 1);
}

BOOST_AUTO_TEST_CASE(Clear)
{
  StringTree tree;
  tree.push(1, "one");
  tree.push(2, "two");

  tree.clear();
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(Swap)
{
  StringTree tree1;
  tree1.push(1, "one");
  StringTree tree2;
  tree2.push(2, "two");

  tree1.swap(tree2);
  BOOST_TEST(stringOfTree(tree1) == "two");
  BOOST_TEST(stringOfTree(tree2) == "one");
}

BOOST_AUTO_TEST_CASE(Insert)
{
  StringTree tree;
  tree.insert(std::make_pair(2, "two"));

  BOOST_TEST(stringOfTree(tree) == "two");
}


BOOST_AUTO_TEST_CASE(At)
{
  StringTree tree;
  tree.push(1, "one");

  BOOST_TEST(tree.at(1) == "one");
}

BOOST_AUTO_TEST_CASE(Count)
{
  StringTree tree;
  tree.push(1, "one");

  BOOST_TEST(tree.count(1) == 1);
  BOOST_TEST(tree.count(2) == 0);
}

BOOST_AUTO_TEST_CASE(Find)
{
  StringTree tree;
  tree.push(1, "one");
  bool end = (tree.find(2) == tree.end());
  BOOST_TEST(end);
  bool noend = (tree.find(1) != tree.end());
  BOOST_TEST(noend);
}

