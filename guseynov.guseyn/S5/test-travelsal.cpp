#include <boost/test/unit_test.hpp>
#include <algorithm>
#include "binary_search_tree.hpp"
#include "files.hpp"

BOOST_AUTO_TEST_CASE(simple_ascending_test)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({2, "first"});
  tree.insert({1, "name"});
  tree.insert({3, "surname"});
  KeySumm collector;
  collector = tree.traverse_lnr(collector);
  BOOST_TEST(collector.result_ == 6);
  BOOST_TEST(collector.values_ == "name first surname");
}

BOOST_AUTO_TEST_CASE(simple_descending_test)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({2, "first"});
  tree.insert({1, "name"});
  tree.insert({3, "surname"});
  KeySumm collector;
  collector = tree.traverse_rnl(collector);
  BOOST_TEST(collector.result_ == 6);
  BOOST_TEST(collector.values_ == "surname first name");
}

BOOST_AUTO_TEST_CASE(simple_breadth_test)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({2, "first"});
  tree.insert({1, "name"});
  tree.insert({3, "surname"});
  KeySumm collector;
  collector = tree.traverse_breadth(collector);
  BOOST_TEST(collector.result_ == 6);
  bool has_first = collector.values_.find("first") != std::string::npos;
  bool has_name = collector.values_.find("name") != std::string::npos;
  bool has_surname = collector.values_.find("surname") != std::string::npos;
  BOOST_TEST(has_first);
  BOOST_TEST(has_name);
  BOOST_TEST(has_surname);
  size_t space_count = 0;
  for (char c : collector.values_) {
    if (c == ' ') {
      space_count++;
    }
  }
  BOOST_TEST(space_count == 2);
}

BOOST_AUTO_TEST_CASE(empty_tree_traversal)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  KeySumm lnr_collector;
  lnr_collector = tree.traverse_lnr(lnr_collector);
  BOOST_TEST(lnr_collector.result_ == 0);
  BOOST_TEST(lnr_collector.values_.empty());
  KeySumm rnl_collector;
  rnl_collector = tree.traverse_rnl(rnl_collector);
  BOOST_TEST(rnl_collector.result_ == 0);
  BOOST_TEST(rnl_collector.values_.empty());
  KeySumm breadth_collector;
  breadth_collector = tree.traverse_breadth(breadth_collector);
  BOOST_TEST(breadth_collector.result_ == 0);
  BOOST_TEST(breadth_collector.values_.empty());
}

BOOST_AUTO_TEST_CASE(overflow_protection_test)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({std::numeric_limits<int>::max(), "max"});
  tree.insert({1, "one"});
  KeySumm collector;
  BOOST_CHECK_THROW(tree.traverse_lnr(collector), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(underflow_protection_test)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({std::numeric_limits<int>::min(), "min"});
  tree.insert({-1, "negative"});
  KeySumm collector;
  BOOST_CHECK_THROW(tree.traverse_lnr(collector), std::underflow_error);
}
