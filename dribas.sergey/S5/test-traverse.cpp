#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>
#include <limits>
#include <avlTree.hpp>
#include "sumKey.hpp"

BOOST_AUTO_TEST_SUITE(SumKeyTraversalTests)

namespace
{
  dribas::AVLTree< long long, std::string > create_test_tree()
  {
    dribas::AVLTree< long long, std::string > tree;
    tree.insert({5, "five"});
    tree.insert({3, "three"});
    tree.insert({7, "seven"});
    tree.insert({2, "two"});
    tree.insert({4, "four"});
    tree.insert({6, "six"});
    tree.insert({8, "eight"});
    return tree;
  }
}

BOOST_AUTO_TEST_CASE(BasicSumKeyTest)
{
  dribas::SumKey sum;
  sum({1, "one"});
  sum({2, "two"});
  BOOST_CHECK_EQUAL(sum.key, 3);
  BOOST_CHECK_EQUAL(sum.value, "one two");
}

BOOST_AUTO_TEST_CASE(TreeTraversalSumTest)
{
  auto tree = create_test_tree();
  dribas::SumKey result = tree.traverse_lnr(dribas::SumKey{});
  BOOST_CHECK_EQUAL(result.key, 35);
  BOOST_CHECK_EQUAL(result.value, "two three four five six seven eight");
}

BOOST_AUTO_TEST_CASE(EmptyTreeTest)
{
  dribas::AVLTree< long long, std::string > empty_tree;
  dribas::SumKey result = empty_tree.traverse_lnr(dribas::SumKey{});

  BOOST_CHECK_EQUAL(result.key, 0);
  BOOST_CHECK(result.value.empty());
}

BOOST_AUTO_TEST_CASE(OverflowProtectionTest)
{
  dribas::AVLTree< long long, std::string > max_tree;
  max_tree.insert({std::numeric_limits<long long>::max(), "max"});
  max_tree.insert({1, "one"});
  BOOST_CHECK_THROW(
    max_tree.traverse_lnr(dribas::SumKey{}),
    std::overflow_error
  );
}

BOOST_AUTO_TEST_CASE(NegativeOverflowTest)
{
  dribas::AVLTree< long long, std::string > min_tree;
  min_tree.insert({std::numeric_limits<long long>::min(), "min"});
  min_tree.insert({-1, "minus"});
  BOOST_CHECK_THROW(min_tree.traverse_lnr(dribas::SumKey{}),std::overflow_error);
}

BOOST_AUTO_TEST_CASE(ValueConcatenationTest)
{
  dribas::SumKey sum;
  sum({1, "first"});
  sum({2, "second"});
  sum({3, "third"});
  BOOST_CHECK_EQUAL(sum.value, "first second third");
}

BOOST_AUTO_TEST_CASE(SingleNodeTest)
{
  dribas::AVLTree< long long, std::string > tree;
  tree.insert({10, "single"});
  dribas::SumKey result = tree.traverse_lnr(dribas::SumKey{});
  BOOST_CHECK_EQUAL(result.key, 10);
  BOOST_CHECK_EQUAL(result.value, "single");
}

BOOST_AUTO_TEST_SUITE_END()
