#include <boost/test/unit_test.hpp>
#include <avlTree.hpp>

using namespace dribas;


BOOST_AUTO_TEST_CASE(MoveAssignmentEmptyMap)
{
  AVLTree< int, std::string > src;
  AVLTree< int, std::string > dst;
  dst = std::move(src);
  BOOST_CHECK(dst.empty());
  BOOST_CHECK(src.empty());
}

BOOST_AUTO_TEST_CASE(MoveAssignmentNonEmptyMap)
{
  AVLTree< int, std::string > src({{1, "one"}, {2, "two"}});
  AVLTree< int, std::string > dst;
  dst = std::move(src);
  BOOST_CHECK(!dst.empty());
  BOOST_CHECK_EQUAL(dst.size(), 2);
  BOOST_CHECK_EQUAL(dst.at(1), "one");
  BOOST_CHECK(src.empty() || !src.empty());
}

BOOST_AUTO_TEST_SUITE(MapCopyAssignmentTests)

BOOST_AUTO_TEST_CASE(CopyAssignmentEmptyMap)
{
  AVLTree< int, std::string > src;
  AVLTree< int, std::string > dst;
  dst = src;
  BOOST_CHECK(dst.empty());
  BOOST_CHECK_EQUAL(dst.size(), 0);
}

BOOST_AUTO_TEST_CASE(CopyAssignmentNonEmptyMap)
{
  AVLTree< int, std::string > src({{1, "one"}, {2, "two"}, {3, "three"}});
  AVLTree< int, std::string > dst;
  dst = src;
  BOOST_CHECK(!dst.empty());
  BOOST_CHECK_EQUAL(dst.size(), 3);
  BOOST_CHECK_EQUAL(dst.at(1), "one");
  BOOST_CHECK_EQUAL(dst.at(2), "two");
  BOOST_CHECK_EQUAL(dst.at(3), "three");
}

BOOST_AUTO_TEST_CASE(CopyAssignmentSelf)
{
  AVLTree< int, std::string > m({{1, "one"}, {2, "two"}});
  m = m;
  BOOST_CHECK(!m.empty());
  BOOST_CHECK_EQUAL(m.size(), 2);
  BOOST_CHECK_EQUAL(m.at(1), "one");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MapOperatorTests)

BOOST_AUTO_TEST_CASE(SubscriptInsertNewKey)
{
  AVLTree< int, std::string > m;
  m[3] = "three";
  BOOST_CHECK_EQUAL(m[3], "three");
  BOOST_CHECK_EQUAL(m.size(), 1);
}

BOOST_AUTO_TEST_CASE(SubscriptDefaultValue)
{
  AVLTree< int, std::string > m;
  std::string& value = m[10];
  BOOST_CHECK(value.empty());
  BOOST_CHECK_EQUAL(m.size(), 1);
}

BOOST_AUTO_TEST_CASE(SubscriptModifyValue)
{
  AVLTree< int, std::string > m({{1, "old"}});
  m[1] = "new";
  BOOST_CHECK_EQUAL(m[1], "new");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AtMethodTests)

BOOST_AUTO_TEST_CASE(AtAccessExistingKey)
{
  AVLTree< int, std::string > m({{1, "one"}, {2, "two"}});
  BOOST_CHECK_EQUAL(m.at(1), "one");
  BOOST_CHECK_EQUAL(m.at(2), "two");
}

BOOST_AUTO_TEST_CASE(AtThrowsOnMissingKey) {
  AVLTree< int, std::string > m({{1, "one"}});
  BOOST_CHECK_THROW(m.at(999), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(AtConstVersion)
{
  const AVLTree< int, std::string > m({{1, "const_one"}});
  BOOST_CHECK_EQUAL(m.at(1), "const_one");
  BOOST_CHECK_THROW(m.at(2), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MapInsertTests)

BOOST_AUTO_TEST_CASE(InsertLValuePair)
{
  AVLTree< int, std::string > m;
  const std::pair< int, std::string > p1({1, "one"});
  auto result1 = m.insert(p1);
  BOOST_CHECK(result1.second);
  BOOST_CHECK_EQUAL(result1.first->first, 1);
  BOOST_CHECK_EQUAL(result1.first->second, "one");
  BOOST_CHECK_EQUAL(m.size(), 1);
  auto result2 = m.insert(p1);
  BOOST_CHECK(!result2.second);
  BOOST_CHECK_EQUAL(m.size(), 1);
}

BOOST_AUTO_TEST_CASE(InsertRValuePair)
{
  AVLTree< int, std::string > m;
  auto result = m.insert({2, "two"});
  BOOST_CHECK(result.second);
  BOOST_CHECK_EQUAL(result.first->first, 2);
  BOOST_CHECK_EQUAL(result.first->second, "two");
  BOOST_CHECK_EQUAL(m.size(), 1);
}

BOOST_AUTO_TEST_CASE(InsertRValueDuplicate)
{
  AVLTree< int, std::string > m({{3, "three"}});
  auto result = m.insert({3, "duplicate"});
  BOOST_CHECK(!result.second);
  BOOST_CHECK_EQUAL(result.first->second, "three");
}

BOOST_AUTO_TEST_CASE(InsertRange)
{
  AVLTree< int, std::string > source({{1, "one"}, {2, "two"}, {3, "three"}});
  AVLTree< int, std::string > target;
  target.insert(source.begin(), source.end());
  BOOST_CHECK_EQUAL(target.size(), 3);
  BOOST_CHECK_EQUAL(target.at(2), "two");
}

BOOST_AUTO_TEST_CASE(InsertOverlappingRange)
{
  AVLTree < int, std::string > m({{1, "old"}, {3, "three"}});
  std::vector< std::pair< int, std::string > > v({{1, "new"}, {2, "two"}});
  m.insert(v.begin(), v.end());
  BOOST_CHECK_EQUAL(m.size(), 3);
  BOOST_CHECK_EQUAL(m.at(1), "old");
  BOOST_CHECK_EQUAL(m.at(2), "two");
}

BOOST_AUTO_TEST_CASE(InsertInitializerList)
{
  AVLTree< int, std::string > m;
  m.insert({{4, "four"}, {5, "five"}, {4, "duplicate"}});
  BOOST_CHECK_EQUAL(m.size(), 2);
}

BOOST_AUTO_TEST_CASE(InsertInitializerListToNonEmpty)
{
  AVLTree< int, std::string > m({{1, "one"}});
  m.insert({{1, "uno"}, {2, "two"}});
  BOOST_CHECK_EQUAL(m.size(), 2);
  BOOST_CHECK_EQUAL(m.at(1), "one");
}

BOOST_AUTO_TEST_CASE(InsertEmptyRange)
{
  AVLTree< int, std::string > m({{1, "one"}});
  std::vector< std::pair< int, std::string > > empty_vec;
  m.insert(empty_vec.begin(), empty_vec.end());
  BOOST_CHECK_EQUAL(m.size(), 1);
}

BOOST_AUTO_TEST_CASE(InsertEmptyInitializerList)
{
  AVLTree< int, std::string > m;
  m.insert({});
  BOOST_CHECK(m.empty());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MapIteratorTests)

BOOST_AUTO_TEST_CASE(NonConstBeginEnd)
{
  std::map< int, std::string > m({{1, "one"}, {2, "two"}});
  auto it_begin = m.begin();
  BOOST_CHECK_EQUAL(it_begin->first, 1);
  BOOST_CHECK_EQUAL(it_begin->second, "one");
  auto it_end = m.end();
  BOOST_CHECK(it_begin != it_end);
  size_t count = 0;
  for (auto it = m.begin(); it != m.end(); ++it) {
    ++count;
  }
  BOOST_CHECK_EQUAL(count, 2);
}

BOOST_AUTO_TEST_CASE(ConstBeginEnd)
{
  const std::map< int, std::string > m({{1, "one"}, {2, "two"}});
  auto cit_begin = m.begin();
  BOOST_CHECK_EQUAL(cit_begin->first, 1);
  BOOST_CHECK_EQUAL(cit_begin->second, "one");
  auto cit_end = m.end();
  BOOST_CHECK(cit_begin != cit_end);
}

BOOST_AUTO_TEST_CASE(ConstCBeginCEnd)
{
  std::map< int, std::string > m({{1, "one"}, {2, "two"}});
  auto cit = m.cbegin();
  BOOST_CHECK_EQUAL(cit->first, 1);
  BOOST_CHECK_EQUAL(cit->second, "one");
  BOOST_CHECK(cit != m.cend());
  std::string concatenated;
  for (auto it = m.cbegin(); it != m.cend(); ++it) {
    concatenated += it->second;
  }
  BOOST_CHECK_EQUAL(concatenated, "onetwo");
}

BOOST_AUTO_TEST_CASE(EmptyContainer)
{
  std::map< int, std::string > m;
  BOOST_CHECK(m.begin() == m.end());
  BOOST_CHECK(m.cbegin() == m.cend());
  const std::map< int, std::string > const_m;
  BOOST_CHECK(const_m.begin() == const_m.end());
}

BOOST_AUTO_TEST_CASE(IterationOrder)
{
  std::map< int, std::string > m({{3, "three"}, {1, "one"}, {2, "two"}});
  auto it = m.begin();
  BOOST_CHECK_EQUAL(it->first, 1);
  BOOST_CHECK_EQUAL((++it)->first, 2);
  BOOST_CHECK_EQUAL((++it)->first, 3);
  BOOST_CHECK(++it == m.end());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AVLTreeMethodTests)

BOOST_AUTO_TEST_CASE(SwapTest)
{
  AVLTree< int, std::string > tree1({{1, "one"}, {2, "two"}});
  AVLTree< int, std::string > tree2({{3, "three"}});
  tree1.swap(tree2);
  BOOST_CHECK_EQUAL(tree1.size(), 1);
  BOOST_CHECK_EQUAL(tree2.size(), 2);
  BOOST_CHECK_EQUAL(tree1.find(3)->second, "three");
  BOOST_CHECK_EQUAL(tree2.find(1)->second, "one");
}

BOOST_AUTO_TEST_CASE(ClearTest)
{
  AVLTree< int, std::string > tree({{1, "one"}, {2, "two"}});

  tree.clear();

  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK(tree.find(1) == tree.end());
}

BOOST_AUTO_TEST_CASE(EmptyTest)
{
  AVLTree< int, std::string > emptyTree;
  AVLTree< int, std::string > nonEmptyTree({{1, "one"}});

  BOOST_CHECK(emptyTree.empty());
  BOOST_CHECK(!nonEmptyTree.empty());
}

BOOST_AUTO_TEST_CASE(SizeTest)
{
  AVLTree< int, std::string > tree;

  BOOST_CHECK_EQUAL(tree.size(), 0);

  tree.insert({1, "one"});
  BOOST_CHECK_EQUAL(tree.size(), 1);

  tree.insert({2, "two"});
  BOOST_CHECK_EQUAL(tree.size(), 2);
}

BOOST_AUTO_TEST_CASE(FindTest)
{
  AVLTree< int, std::string > tree({{1, "one"}, {2, "two"}});

  auto it1 = tree.find(1);
  BOOST_REQUIRE(it1 != tree.end());
  BOOST_CHECK_EQUAL(it1->second, "one");

  auto it2 = tree.find(99);
  BOOST_CHECK(it2 == tree.end());
}

BOOST_AUTO_TEST_CASE(ConstFindTest)
{
  const AVLTree< int, std::string > tree({{1, "one"}});
  auto it1 = tree.find(1);
  BOOST_REQUIRE(it1 != tree.end());
  BOOST_CHECK_EQUAL(it1->second, "one");
  auto it2 = tree.find(99);
  BOOST_CHECK(it2 == tree.end());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AVLTreeEraseTests)
BOOST_AUTO_TEST_CASE(EraseByKey)
{
  AVLTree< int, std::string > tree({{1, "one"}, {2, "two"}, {3, "three"}});
  size_t count = tree.erase(2);
  BOOST_CHECK_EQUAL(count, 1);
  BOOST_CHECK_EQUAL(tree.size(), 2);
  BOOST_CHECK(tree.find(2) == tree.end());
  count = tree.erase(99);
  BOOST_CHECK_EQUAL(count, 0);
  BOOST_CHECK_EQUAL(tree.size(), 2);
}

BOOST_AUTO_TEST_CASE(EraseAllElements)
{
  AVLTree< int, std::string > tree({{1, "one"}, {2, "two"}});
  tree.erase(tree.begin(), tree.end());
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.size(), 0);
  BOOST_CHECK(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(EraseEmptyRange)
{
  AVLTree< int, std::string > tree({{1, "one"}});
  BOOST_CHECK_EQUAL(tree.size(), 1);
  BOOST_CHECK_EQUAL(tree.find(1)->second, "one");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AVLTreeRangeSearchTests)

BOOST_AUTO_TEST_CASE(EqualRangeTest)
{
  AVLTree< int, std::string > tree({{1, "one"}, {2, "two"}, {2, "two_dup"}, {3, "three"}});
  auto it1 = tree.equal_range(2).first;
  BOOST_CHECK_EQUAL(it1->second, "two");
  auto it3 = tree.equal_range(99).first;
  BOOST_CHECK(it3 == tree.end());
}

BOOST_AUTO_TEST_CASE(LowerBoundTest)
{
  AVLTree< int, std::string > tree({{1, "one"}, {3, "three"}, {5, "five"}});
  auto it1 = tree.lower_bound(2);
  BOOST_CHECK_EQUAL(it1->first, 3);
  auto it2 = tree.lower_bound(3);
  BOOST_CHECK_EQUAL(it2->first, 3);
  const auto& ctree = tree;
  auto cit1 = ctree.lower_bound(4);
  BOOST_CHECK_EQUAL(cit1->first, 5);
  auto it3 = tree.lower_bound(6);
  BOOST_CHECK(it3 == tree.end());
}

BOOST_AUTO_TEST_CASE(UpperBoundTest)
{
  AVLTree< int, std::string > tree({{1, "one"}, {3, "three"}, {5, "five"}});
  auto it1 = tree.upper_bound(2);
  BOOST_CHECK_EQUAL(it1->first, 3);
  auto it2 = tree.upper_bound(3);
  BOOST_CHECK_EQUAL(it2->first, 5);
  const auto& ctree = tree;
  auto cit1 = ctree.upper_bound(4);
  BOOST_CHECK_EQUAL(cit1->first, 5);
  auto it3 = tree.upper_bound(5);
  BOOST_CHECK(it3 == tree.end());
}

BOOST_AUTO_TEST_CASE(EmptyTreeTest)
{
  AVLTree< int, std::string > emptyTree;
  auto it2 = emptyTree.equal_range(1).first;
  BOOST_CHECK(it2 == emptyTree.end());
  auto it3 = emptyTree.lower_bound(1);
  BOOST_CHECK(it3 == emptyTree.end());
  auto it4 = emptyTree.upper_bound(1);
  BOOST_CHECK(it4 == emptyTree.end());
}

BOOST_AUTO_TEST_CASE(DuplicateKeysTest)
{
  AVLTree< int, std::string > tree({{1, "a"}, {2, "b"}, {2, "c"}, {2, "d"}, {3, "e"}});
  auto lb = tree.lower_bound(2);
  BOOST_CHECK_EQUAL(lb->first, 2);
  BOOST_CHECK_EQUAL(lb->second, "b");
  auto ub = tree.upper_bound(2);
  BOOST_CHECK_EQUAL(ub->first, 3);
}

BOOST_AUTO_TEST_SUITE_END()
