#include <boost/test/unit_test.hpp>
#include <algorithm>
#include "tree.hpp"
#include "key_summer.hpp"

BOOST_AUTO_TEST_CASE(simple_ascending_test)
{
  guseynov::Tree< int, std::string, std::less<int> > tree;
  tree.insert({2, "first"});
  tree.insert({1, "name"});
  tree.insert({3, "surname"});
  guseynov::KeySummer collector;
  tree.traverse_lnr(collector);
  BOOST_TEST(collector.result_ == 6);
  BOOST_TEST(collector.values_ == "name first surname");
}

BOOST_AUTO_TEST_CASE(simple_descending_test)
{
  guseynov::Tree< int, std::string, std::less<int> > tree;
  tree.insert({2, "first"});
  tree.insert({1, "name"});
  tree.insert({3, "surname"});
  guseynov::KeySummer collector;
  tree.traverse_rnl(collector);
  BOOST_TEST(collector.result_ == 6);
  BOOST_TEST(collector.values_ == "surname first name");
}

BOOST_AUTO_TEST_CASE(simple_breadth_test)
{
  guseynov::Tree< int, std::string, std::less<int> > tree;
  tree.insert({2, "first"});
  tree.insert({1, "name"});
  tree.insert({3, "surname"});
  guseynov::KeySummer collector;
  tree.traverse_breadth(collector);
  BOOST_TEST(collector.result_ == 6);
  BOOST_TEST(collector.values_ == "first name surname");
}

BOOST_AUTO_TEST_CASE(empty_tree_traversal)
{
  guseynov::Tree< int, std::string, std::less<int> > tree;
  guseynov::KeySummer lnr_collector;
  tree.traverse_lnr(lnr_collector);
  BOOST_TEST(lnr_collector.result_ == 0);
  BOOST_TEST(lnr_collector.values_.empty());
  guseynov::KeySummer rnl_collector;
  tree.traverse_rnl(rnl_collector);
  BOOST_TEST(rnl_collector.result_ == 0);
  BOOST_TEST(rnl_collector.values_.empty());
  guseynov::KeySummer breadth_collector;
  tree.traverse_breadth(breadth_collector);
  BOOST_TEST(breadth_collector.result_ == 0);
  BOOST_TEST(breadth_collector.values_.empty());
}

BOOST_AUTO_TEST_CASE(overflow_protection_test)
{
  guseynov::Tree< long long, std::string, std::less<long long> > tree;
  tree.insert({std::numeric_limits<long long>::max(), "max"});
  tree.insert({1, "one"});
  guseynov::KeySummer collector;
  BOOST_CHECK_THROW(tree.traverse_lnr(collector), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(underflow_protection_test)
{
  guseynov::Tree< long long, std::string, std::less<long long> > tree;
  tree.insert({std::numeric_limits<long long>::min(), "min"});
  tree.insert({-1, "negative"});
  guseynov::KeySummer collector;
  BOOST_CHECK_THROW(tree.traverse_lnr(collector), std::underflow_error);
}
