#include <boost/test/unit_test.hpp>
#include <boost/test/tools/detail/print_helper.hpp>
#include <string>
#include "binary_tree.hpp"

namespace boost::test_tools::tt_detail
{
  template< class Key, class Value >
  struct print_log_value< abramov::Iterator< Key, Value > >
  {
    void operator()(std::ostream &os, const abramov::Iterator< Key, Value > &iter)
    {
      os << "iter";
    }
  };

  template< class Key, class Value >
  struct print_log_value< abramov::ConstIterator< Key, Value > >
  {
    void operator()(std::ostream &os, const abramov::ConstIterator< Key, Value > &iter)
    {
      os << "cIter";
    }
  };
}

BOOST_AUTO_TEST_CASE(defaultconstructor_insert)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(10, "a");
  tree.insert(8, "b");
  auto it = tree.find(8);
  BOOST_TEST(it->second == "b");
}

BOOST_AUTO_TEST_CASE(copyconstructor)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(10, "a");
  tree.insert(8, "b");
  abramov::BinarySearchTree< int, std::string > copy_tree(tree);
  BOOST_TEST(copy_tree.find(8)->second == "b");
  BOOST_TEST(copy_tree.find(10)->second == "a");
}

BOOST_AUTO_TEST_CASE(copyoperator)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(10, "a");
  tree.insert(8, "b");
  abramov::BinarySearchTree< int, std::string > copy_tree = tree;
  BOOST_TEST(copy_tree.find(8)->second == "b");
  BOOST_TEST(copy_tree.find(10)->second == "a");
}

BOOST_AUTO_TEST_CASE(operatorbrackets)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree[10] = "a";
  tree[8] = "b";
  tree[12] = "c";
  BOOST_TEST(tree.find(12)->second == "c");
  BOOST_TEST(tree.find(8)->second == "b");
  BOOST_TEST(tree.find(10)->second == "a");
  tree[8] = "d";
  BOOST_TEST(tree.find(8)->second == "d");
}

BOOST_AUTO_TEST_CASE(erase_iter)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(10, "a");
  tree.insert(8, "b");
  tree.insert(12, "c");
  tree.insert(5, "d");
  tree.insert(14, "e");
  tree.erase(tree.find(5));
  BOOST_TEST(tree.find(5) == tree.end());
  tree.erase(tree.find(8));
  BOOST_TEST(tree.find(8) == tree.end());
  tree.erase(tree.find(12));
  BOOST_TEST(tree.find(12) == tree.end());
}

BOOST_AUTO_TEST_CASE(erase_key)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(10, "a");
  tree.insert(8, "b");
  tree.insert(12, "c");
  tree.insert(5, "d");
  tree.insert(14, "e");
  tree.erase(5);
  BOOST_TEST(tree.find(5) == tree.end());
  tree.erase(12);
  BOOST_TEST(tree.find(12) == tree.end());
  tree.erase(8);
  BOOST_TEST(tree.find(8) == tree.end());
}

BOOST_AUTO_TEST_CASE(begin)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(5, "a");
  tree.insert(1, "b");
  auto it = tree.begin();
  BOOST_TEST(it->second == "b");
}

BOOST_AUTO_TEST_CASE(cbegin)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(1, "a");
  tree.insert(5, "b");
  auto it = tree.cbegin();
  BOOST_TEST(it->second == "a");
}

BOOST_AUTO_TEST_CASE(end)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(5, "a");
  tree.insert(1, "b");
  auto it = --tree.end();
  BOOST_TEST(it->second == "a");
}

BOOST_AUTO_TEST_CASE(cend)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(5, "b");
  tree.insert(1, "a");
  auto it = --tree.cend();
  BOOST_TEST(it->second == "b");
  BOOST_TEST(++it == tree.cend());
}

BOOST_AUTO_TEST_CASE(find)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(10, "a");
  tree.insert(3, "b");
  tree.insert(12, "c");
  tree.insert(14, "d");
  auto it = tree.find(10);
  BOOST_TEST(it->second == "a");
  it = tree.find(3);
  BOOST_TEST(it->second == "b");
  it = tree.find(12);
  BOOST_TEST(it->second == "c");
  it = tree.find(14);
  BOOST_TEST(it->second == "d");
}

BOOST_AUTO_TEST_CASE(cfind)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(12, "a");
  tree.insert(9, "b");
  tree.insert(8, "c");
  tree.insert(7, "d");
  auto it = tree.cfind(12);
  BOOST_TEST(it->second == "a");
  it = tree.cfind(9);
  BOOST_TEST(it->second == "b");
  it = tree.cfind(8);
  BOOST_TEST(it->second == "c");
  it = tree.cfind(7);
  BOOST_TEST(it->second == "d");
}

BOOST_AUTO_TEST_CASE(count)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(12, "a");
  BOOST_TEST(tree.count(10) == 0);
  tree.insert(10, "b");
  BOOST_TEST(tree.count(10) == 1);
}

BOOST_AUTO_TEST_CASE(size)
{
  abramov::BinarySearchTree< int, std::string > tree;
  BOOST_TEST(tree.size() == 0);
  tree.insert(12, "a");
  BOOST_TEST(tree.size() == 1);
  tree.insert(14, "b");
  BOOST_TEST(tree.size() == 2);
}

BOOST_AUTO_TEST_CASE(empty)
{
  abramov::BinarySearchTree< int, std::string > tree;
  BOOST_TEST(tree.empty());
  tree.insert(10, "a");
  BOOST_TEST(!tree.empty());
}

BOOST_AUTO_TEST_CASE(swap)
{
  abramov::BinarySearchTree< int, std::string > tree1;
  tree1.insert(4, "a");
  tree1.insert(2, "b");
  abramov::BinarySearchTree< int, std::string > tree2;
  tree2.insert(5, "c");
  tree2.insert(1, "d");
  tree1.swap(tree2);
  auto it1 = tree1.begin();
  auto it2 = tree2.begin();
  BOOST_TEST(it1->second == "d");
  BOOST_TEST(it2->second == "b");
}

BOOST_AUTO_TEST_CASE(clear)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(6, "a");
  tree.insert(3, "b");
  tree.clear();
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(equalrange)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(5, "a");
  auto p = tree.equal_range(6);
  BOOST_TEST(p.first == p.second);
  BOOST_TEST(p.first == tree.end());
}

BOOST_AUTO_TEST_CASE(cequalrange)
{
  abramov::BinarySearchTree< int, std::string > tree;
  tree.insert(5, "a");
  auto p = tree.cequal_range(6);
  BOOST_TEST(p.first == p.second);
  BOOST_TEST(p.first == tree.cend());
}
