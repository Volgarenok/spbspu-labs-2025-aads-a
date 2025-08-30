#include <boost/test/unit_test.hpp>
#include <sstream>
#include <AVLtree.hpp>

namespace
{
  void printTree(std::ostream& out, const finaev::AVLtree< int, std::string >& tree)
  {
    for (auto it = tree.cBegin(); it != tree.cEnd(); ++it)
    {
      out << it->second;
    }
  }
}

BOOST_AUTO_TEST_CASE(default_constructor)
{
  finaev::AVLtree< int, std::string > tree;
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  finaev::AVLtree< int, std::string > tree;
  tree.insert({1, "1"});
  tree.insert({2, "2"});
  tree.insert({3, "3"});
  tree.insert({4, "4"});
  std::ostringstream out1;
  printTree(out1, tree);
  BOOST_TEST(out1.str() == "1234");
  finaev::AVLtree< int, std::string > treeCopy(tree);
  treeCopy.insert({5, "5"});
  std::ostringstream out2;
  printTree(out2, treeCopy);
  BOOST_TEST(out2.str() == "12345");
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  finaev::AVLtree< int, std::string > tree;
  tree.insert({1, "1"});
  tree.insert({2, "2"});
  tree.insert({3, "3"});
  tree.insert({4, "4"});
  finaev::AVLtree< int, std::string > treeMove(std::move(tree));
  std::ostringstream out;
  printTree(out, treeMove);
  BOOST_TEST(out.str() == "1234");
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(copy_operator)
{
  finaev::AVLtree< int, std::string > tree;
  tree.insert({1, "1"});
  tree.insert({2, "2"});
  tree.insert({3, "3"});
  finaev::AVLtree< int, std::string > treeCopy;
  treeCopy = tree;
  std::ostringstream out1;
  printTree(out1, tree);
  std::ostringstream out2;
  printTree(out2, treeCopy);
  BOOST_TEST(out1.str() == out2.str());
}

BOOST_AUTO_TEST_CASE(move_operator)
{
  finaev::AVLtree< int, std::string > tree;
  tree.insert({1, "1"});
  tree.insert({2, "2"});
  tree.insert({3, "3"});
  finaev::AVLtree< int, std::string > treeMove;
  treeMove = std::move(tree);
  std::ostringstream out;
  printTree(out, treeMove);
  BOOST_TEST(out.str() == "123");
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(the_square_brackets_operator)
{
  finaev::AVLtree< int, std::string > tree;
  tree[1] = "1";
  tree[2] = "2";
  std::ostringstream out1;
  printTree(out1, tree);
  BOOST_TEST(out1.str() == "12");
}

BOOST_AUTO_TEST_CASE(operator_at)
{
  finaev::AVLtree< int, std::string > tree;
  tree.insert({1, "1"});
  tree.insert({2, "2"});
  tree.insert({3, "3"});
  BOOST_TEST(tree.at(2) == "2");
}

BOOST_AUTO_TEST_CASE(tree_find)
{
  finaev::AVLtree< int, std::string > tree;
  tree.insert({1, "1"});
  tree.insert({2, "2"});
  tree.insert({3, "3"});
  bool t = (tree.find(1) != tree.end());
  BOOST_TEST(t);
  bool f = (tree.find(4) == tree.end());
  BOOST_TEST(f);
}

BOOST_AUTO_TEST_CASE(erase_by_position)
{
  finaev::AVLtree< int, std::string > tree;
  tree.insert({1, "1"});
  tree.insert({2, "2"});
  tree.insert({3, "3"});
  auto it = tree.find(2);
  it = tree.erase(it);
  BOOST_TEST(tree.size() == 2);
  std::ostringstream out;
  printTree(out, tree);
  BOOST_TEST(out.str() == "13");
}

BOOST_AUTO_TEST_CASE(erase_by_key)
{
  finaev::AVLtree< int, std::string > tree;
  tree.insert({1, "1"});
  tree.insert({2, "2"});
  tree.insert({3, "3"});
  size_t erased = tree.erase(2);
  BOOST_TEST(erased == 1);
  BOOST_TEST(tree.size() == 2);
}

BOOST_AUTO_TEST_CASE(count)
{
  finaev::AVLtree< int, std::string > tree;
  tree.insert({1, "1"});
  tree.insert({2, "2"});
  size_t a = tree.count(2);
  size_t b = tree.count(3);
  BOOST_TEST(a == 1);
  BOOST_TEST(b == 0);
}
