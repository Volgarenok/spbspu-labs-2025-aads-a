#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "tree.hpp"

using namespace brevnov;

BOOST_AUTO_TEST_SUITE(tree)

BOOST_AUTO_TEST_CASE(constructors)
{
  AVLTree< size_t, std::string > tree;
  BOOST_TEST(tree.size() == 0);
  AVLTree< size_t, std::string > testTree{ { 1, "1" }, { 2, "2" }, { 0, "0" } };
  BOOST_TEST(testTree.size() == 3);
  AVLTree< size_t, std::string > copyTree(testTree);
  BOOST_TEST(copyTree.size() == 3);
  AVLTree< size_t, std::string > moveTree(std::move(copyTree));
  BOOST_TEST(moveTree.size() == 3);
  AVLTree< size_t, std::string >::Iter it = testTree.begin();
  ++it;
  AVLTree< size_t, std::string > rangeTree(it, testTree.end());
  BOOST_TEST(rangeTree.size() == 2);
}

BOOST_AUTO_TEST_CASE(assignment_operators)
{
  AVLTree< size_t, std::string > tree;
  std::initializer_list< std::pair< size_t, std::string > > il = { { 1, "1" }, { 2, "2" }, { 3, "3" } };
  tree = il;
  BOOST_TEST(tree.size() == 3);
  AVLTree< size_t, std::string > copyTree;
  copyTree = tree;
  BOOST_TEST(copyTree.size() == 3);
  AVLTree< size_t, std::string > moveTree;
  moveTree = std::move(copyTree);
  BOOST_TEST(moveTree.size() == 3);
  BOOST_TEST(copyTree.size() == 0);
}

BOOST_AUTO_TEST_CASE(index_operator_at)
{
  AVLTree< size_t, std::string > tree({ { 1, "1" }, { 2, "2" }, { 3, "3" } });
  BOOST_TEST(tree[1] == "1");
  tree[4];
  BOOST_TEST(tree.size() == 4);
  BOOST_TEST(tree.at(2) == "2");
}

BOOST_AUTO_TEST_CASE(size_and_empty_and_clear)
{
  AVLTree< size_t, std::string > tree({ { 1, "1" }, { 2, "2" }, { 3, "3" }, { 4, "4" } });
  BOOST_TEST(tree.size() == 4);
  tree.clear();
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(insert_element)
{
  AVLTree< size_t, std::string > tree;
  AVLTree< size_t, std::string >::Iter it = tree.insert({ 1, "1" }).first;
  BOOST_TEST((*it).first == 1);
  bool isInserted = tree.insert({ 1, "1" }).second;
  BOOST_TEST(!isInserted);
  std::pair< size_t, std::string > pair1 = { 2, "2" };
  tree.insert(pair1);
  BOOST_TEST(tree.size() == 2);
  const std::pair< size_t, std::string > pair2 = { 3, "3" };
  tree.insert(pair2);
  BOOST_TEST(tree.size() == 3);
}

BOOST_AUTO_TEST_CASE(insert_hint)
{
  AVLTree< size_t, std::string > tree({ { 1, "1" }, { 2, "2" }, { 3, "3" } });
  AVLTree< size_t, std::string >::Iter it = tree.begin();
  AVLTree< size_t, std::string >::ConstIter cit = tree.cend();
  tree.insert(it, std::make_pair(4, "4"));
  BOOST_TEST(tree.size() == 4);
  tree.insert(cit, std::make_pair(5, "5"));
  BOOST_TEST(tree.size() == 5);
}

BOOST_AUTO_TEST_CASE(insert_range_and_init)
{
  AVLTree< size_t, std::string > tree;
  AVLTree< size_t, std::string > tree2({ { 1, "1" }, { 2, "2" }, { 3, "3" } });
  tree.insert(++tree2.begin(), tree2.end());
  BOOST_TEST(tree.size() == 2);
  tree.insert({ { 4, "4" }, { 1, "1" }, { 5, "5" } });
  BOOST_TEST(tree.size() == 5);
}
BOOST_AUTO_TEST_CASE(erase_position)
{
  AVLTree< size_t, std::string > tree({ { 1, "1" }, { 2, "2" }, { 3, "3" }, { 4, "4" } });
  AVLTree< size_t, std::string >::Iter it = tree.erase(tree.begin());
  BOOST_CHECK(it == tree.begin());
  it = tree.erase(tree.cbegin());
  BOOST_CHECK(it == tree.begin());
  size_t i = tree.erase(3);
  BOOST_TEST(i == 1);
  i = tree.erase(6);
  BOOST_TEST(i == 0);
  tree = { { 1, "1" }, { 2, "2" }, { 3, "3" } };
  tree.erase(tree.begin(), tree.end());
  BOOST_TEST(tree.empty());
  tree = { { 1, "1" }, { 2, "2" }, { 3, "3" } };
  tree.erase(tree.cbegin(), tree.cend());
  BOOST_TEST(tree.empty());
}
BOOST_AUTO_TEST_CASE(emplace_element_and_hint)
{
  AVLTree< size_t, std::string > tree;
  tree.emplace(1, "1");
  BOOST_TEST(tree.size() == 1);
  AVLTree< size_t, std::string >::Iter it = tree.emplace(2, "2").first;
  BOOST_TEST((*it).first == 2);
  it = tree.emplaceHint(tree.cbegin(), 5, "5");
  BOOST_TEST((*it).first == 5);
  tree.emplace(1, "1");
  BOOST_TEST(tree.size() == 3);
}

BOOST_AUTO_TEST_CASE(swap)
{
  AVLTree< size_t, std::string > tree({ { 1, "1" }, { 2, "2" }, { 3, "3" } });
  AVLTree< size_t, std::string >::Iter it = tree.begin();
  AVLTree< size_t, std::string > tree2;
  tree2.swap(tree);
  BOOST_CHECK(tree2.begin() == it);
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(find)
{
  AVLTree< size_t, std::string > tree({ { 1, "1" }, { 2, "2" }, { 3, "3" } });
  AVLTree< size_t, std::string >::Iter it = tree.find(1);
  BOOST_CHECK(it != tree.end());
  it = tree.find(4);
  BOOST_CHECK(it == tree.end());
  AVLTree< size_t, std::string >::ConstIter cit = tree.find(3);
  BOOST_CHECK(cit != tree.cend());
}

BOOST_AUTO_TEST_CASE(count_lowerBound_upperBound_equalRange)
{
  AVLTree< size_t, std::string > tree({ { 1, "1" }, { 2, "2" }, { 3, "3" }, { 4, "4" }, { 5, "5" } });
  size_t i = tree.count(1);
  BOOST_TEST(i == 1);
  i = tree.count(7);
  BOOST_TEST(i == 0);
  AVLTree< size_t, std::string >::Iter it = tree.lowerBound(2);
  BOOST_TEST((*it).first == 2);
  it = tree.lowerBound(7);
  BOOST_CHECK(it == tree.end());
  it = tree.upperBound(4);
  BOOST_CHECK((*it).first == 5);
  AVLTree< size_t, std::string >::Iter first = tree.lowerBound(3);
  it = tree.upperBound(3);
  BOOST_CHECK(first == tree.equalRange(3).first);
  BOOST_CHECK(it == tree.equalRange(3).second);
}
BOOST_AUTO_TEST_SUITE_END();
