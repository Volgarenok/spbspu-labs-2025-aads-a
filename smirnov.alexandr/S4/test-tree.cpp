#include <boost/test/unit_test.hpp>
#include <tree/avlTee.hpp>

using namespace smirnov;

BOOST_AUTO_TEST_CASE(EmptyTreeTest)
{
  AvlTree< int, int > tree;
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
  BOOST_TEST((tree.cbegin() == tree.cend()));
}

BOOST_AUTO_TEST_CASE(InsertAndFindTest)
{
  AvlTree< int, std::string > tree;
  tree[10] = "ten";
  tree[5] = "five";
  tree[15] = "fifteen";
  BOOST_TEST(!tree.empty());
  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(tree.at(10) == "ten");
  BOOST_TEST(tree.at(5) == "five");
  BOOST_TEST(tree.at(15) == "fifteen");
  bool caught = false;
  try
  {
    tree.at(20);
  }
  catch (const std::out_of_range &)
  {
    caught = true;
  }
  BOOST_TEST(caught);
  auto it = tree.find(5);
  BOOST_TEST((it != tree.cend()));
  BOOST_TEST(it->second == "five");
  auto it_notfound = tree.find(100);
  BOOST_TEST((it_notfound == tree.cend()));
}

BOOST_AUTO_TEST_CASE(IteratorTraversalTest)
{
  AvlTree< int, int > tree;
  for (int i = 20; i >= 0; --i)
  {
    tree[i] = i * 10;
  }
  int expected_key = 0;
  for (auto it = tree.cbegin(); it != tree.cend(); ++it)
  {
    BOOST_TEST(it->first == expected_key);
    BOOST_TEST(it->second == expected_key * 10);
    ++expected_key;
  }
  BOOST_TEST(expected_key == 21);
}

BOOST_AUTO_TEST_CASE(UpdateValueTest)
{
  AvlTree< int, std::string > tree;
  tree[1] = "one";
  BOOST_TEST(tree[1] == "one");
  tree[1] = "uno";
  BOOST_TEST(tree[1] == "uno");
  BOOST_TEST(tree.size() == 1);
}

BOOST_AUTO_TEST_CASE(CopyConstructorAndAssignmentTest)
{
  AvlTree< int, std::string > tree1;
  tree1[1] = "one";
  AvlTree< int, std::string > tree2(tree1);
  BOOST_TEST(tree2.size() == 1);
  BOOST_TEST((tree2.find(1) != tree2.cend()));
  AvlTree< int, std::string > tree3;
  tree3 = tree1;
  BOOST_TEST(tree3.size() == 1);
  BOOST_TEST((tree3.find(1) != tree3.cend()));
}

BOOST_AUTO_TEST_CASE(MoveConstructorAndAssignmentTest)
{
  AvlTree< int, std::string > tree1;
  tree1[1] = "one";
  AvlTree< int, std::string > tree2(std::move(tree1));
  BOOST_TEST(tree2.size() == 1);
  BOOST_TEST(tree1.empty());
  AvlTree< int, std::string > tree3;
  tree3 = std::move(tree2);
  BOOST_TEST(tree3.size() == 1);
  BOOST_TEST(tree2.empty());
}

BOOST_AUTO_TEST_CASE(ClearTest)
{
  AvlTree< int, std::string > tree;
  tree[1] = "val1";
  tree[2] = "val2";
  tree[3] = "val3";
  BOOST_TEST(tree.size() == 3);
  tree.clear();
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
  BOOST_TEST((tree.cbegin() == tree.cend()));
}
