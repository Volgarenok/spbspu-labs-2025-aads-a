#include <ArrayBuffer.hpp>
#include <boost/test/unit_test.hpp>
#include <limits>
#include <string>
#include <tree/ConstIterator.hpp>
#include <tree/Iterator.hpp>
#include <tree/TwoThreeTree.hpp>

struct Accumulator {
  gavrilova::ArrayBuffer< int > keys;
  gavrilova::ArrayBuffer< std::string > values;
  long long sum_keys = 0;

  void operator()(const std::pair< int, std::string >& p)
  {
    sum_keys += p.first;
    keys.push_back(p.first);
    values.push_back(p.second);
  }
};

BOOST_AUTO_TEST_CASE(TestTraverseLNREmptyTree)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  Accumulator acc = tree.traverse_lnr(Accumulator{});
  BOOST_TEST(tree.empty());
  BOOST_TEST(acc.keys.size() == 0);
  BOOST_TEST(acc.values.size() == 0);
  BOOST_TEST(acc.sum_keys == 0);
}

BOOST_AUTO_TEST_CASE(TestTraverseLNR)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  tree.insert({10, "ten"});
  tree.insert({5, "five"});
  tree.insert({15, "fifteen"});
  tree.insert({12, "twelve"});

  Accumulator acc = tree.traverse_lnr(Accumulator{});
  BOOST_TEST(acc.keys.size() == 4);
  BOOST_CHECK(acc.keys[0] == 5);
  BOOST_CHECK(acc.keys[1] == 10);
  BOOST_CHECK(acc.keys[2] == 12);
  BOOST_CHECK(acc.keys[3] == 15);

  BOOST_CHECK(acc.values[0] == "five");
  BOOST_CHECK(acc.values[1] == "ten");
  BOOST_CHECK(acc.values[2] == "twelve");
  BOOST_CHECK(acc.values[3] == "fifteen");

  BOOST_TEST(acc.sum_keys == 5 + 10 + 12 + 15);
}

BOOST_AUTO_TEST_CASE(TestTraverseRNL)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  tree.insert({10, "ten"});
  tree.insert({5, "five"});
  tree.insert({15, "fifteen"});
  tree.insert({12, "twelve"});

  Accumulator acc = tree.traverse_rnl(Accumulator{});
  BOOST_TEST(acc.keys.size() == 4);

  BOOST_CHECK(acc.keys[0] == 15);
  BOOST_CHECK(acc.keys[1] == 12);
  BOOST_CHECK(acc.keys[2] == 10);
  BOOST_CHECK(acc.keys[3] == 5);

  BOOST_CHECK(acc.values[0] == "fifteen");
  BOOST_CHECK(acc.values[1] == "twelve");
  BOOST_CHECK(acc.values[2] == "ten");
  BOOST_CHECK(acc.values[3] == "five");

  BOOST_TEST(acc.sum_keys == 15 + 12 + 10 + 5);
}

BOOST_AUTO_TEST_CASE(TestTraverseBreadth)
{
  gavrilova::TwoThreeTree< int, std::string > tree;
  tree.insert({10, "ten"});
  tree.insert({5, "five"});
  tree.insert({15, "fifteen"});
  tree.insert({12, "twelve"});
  tree.insert({3, "three"});

  Accumulator acc = tree.traverse_breadth(Accumulator{});
  BOOST_TEST(acc.keys.size() == 5);

  gavrilova::ArrayBuffer< int > sorted_keys = acc.keys;

  int expected_sum = 3 + 5 + 10 + 12 + 15;
  BOOST_TEST(acc.sum_keys == expected_sum);
}
