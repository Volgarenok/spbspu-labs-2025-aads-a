#include <boost/test/unit_test.hpp>
#include <set>
#include <tree/avlTee.hpp>
#include "key_sum.hpp"

BOOST_AUTO_TEST_CASE(lnr_traversal_standard)
{
  smirnov::AvlTree< int, std::string > tree;
  tree[5] = "five";
  tree[3] = "three";
  tree[7] = "seven";
  tree[2] = "two";
  tree[4] = "four";
  tree[6] = "six";
  tree[8] = "eight";
  smirnov::KeySum collector;
  collector = tree.traverse_lnr(collector);
  BOOST_TEST(collector.result == 35);
  BOOST_TEST(collector.elems == "two three four five six seven eight");
}

BOOST_AUTO_TEST_CASE(rnl_traversal_standard)
{
  smirnov::AvlTree< int, std::string > tree;
  tree[5] = "five";
  tree[3] = "three";
  tree[7] = "seven";
  tree[2] = "two";
  tree[4] = "four";
  tree[6] = "six";
  tree[8] = "eight";
  smirnov::KeySum collector;
  collector = tree.traverse_rnl(collector);
  BOOST_TEST(collector.result == 35);
  BOOST_TEST(collector.elems == "eight seven six five four three two");
}

BOOST_AUTO_TEST_CASE(breadth_traversal_standard)
{
  smirnov::AvlTree< int, std::string > tree;
  tree[5] = "five";
  tree[3] = "three";
  tree[7] = "seven";
  tree[2] = "two";
  tree[4] = "four";
  tree[6] = "six";
  tree[8] = "eight";
  smirnov::KeySum collector;
  collector = tree.traverse_breadth(collector);
  BOOST_TEST(collector.elems.find("two") != std::string::npos);
  BOOST_TEST(collector.elems.find("three") != std::string::npos);
  BOOST_TEST(collector.elems.find("four") != std::string::npos);
  BOOST_TEST(collector.elems.find("five") != std::string::npos);
  BOOST_TEST(collector.elems.find("six") != std::string::npos);
  BOOST_TEST(collector.elems.find("seven") != std::string::npos);
  BOOST_TEST(collector.elems.find("eight") != std::string::npos);
  size_t space_count = std::count(collector.elems.begin(), collector.elems.end(), ' ');
  BOOST_TEST(space_count == 6);
  std::istringstream iss(collector.elems);
  std::string first_word;
  iss >> first_word;
  BOOST_TEST((first_word == "five" || first_word == "four" || first_word == "three"));
}

BOOST_AUTO_TEST_CASE(empty_tree_traversal)
{
  smirnov::AvlTree< int, std::string > tree;
  smirnov::KeySum lnr_collector;
  lnr_collector = tree.traverse_lnr(lnr_collector);
  BOOST_TEST(lnr_collector.result == 0);
  BOOST_TEST(lnr_collector.elems.empty());
  smirnov::KeySum rnl_collector;
  rnl_collector = tree.traverse_rnl(rnl_collector);
  BOOST_TEST(rnl_collector.result == 0);
  BOOST_TEST(rnl_collector.elems.empty());
  smirnov::KeySum breadth_collector;
  breadth_collector = tree.traverse_breadth(breadth_collector);
  BOOST_TEST(breadth_collector.result == 0);
  BOOST_TEST(breadth_collector.elems.empty());
}

BOOST_AUTO_TEST_CASE(single_node_traversal)
{
  smirnov::AvlTree< int, std::string > tree;
  tree[1] = "one";
  smirnov::KeySum collector;
  collector = tree.traverse_lnr(collector);
  BOOST_TEST(collector.result == 1);
  BOOST_TEST(collector.elems == "one");
  collector.result = 0;
  collector.elems.clear();
  collector = tree.traverse_rnl(collector);
  BOOST_TEST(collector.result == 1);
  BOOST_TEST(collector.elems == "one");
  collector.result = 0;
  collector.elems.clear();
  collector = tree.traverse_breadth(collector);
  BOOST_TEST(collector.result == 1);
  BOOST_TEST(collector.elems == "one");
}

BOOST_AUTO_TEST_CASE(multiple_traversals_consistency)
{
  smirnov::AvlTree< int, std::string > tree;
  tree[3] = "three";
  tree[1] = "one";
  tree[2] = "two";
  tree[4] = "four";
  smirnov::KeySum collector;
  collector = tree.traverse_lnr(collector);
  BOOST_TEST(collector.result == 10);
  BOOST_TEST(collector.elems == "one two three four");
  collector.result = 0;
  collector.elems.clear();
  collector = tree.traverse_lnr(collector);
  BOOST_TEST(collector.result == 10);
  BOOST_TEST(collector.elems == "one two three four");
}

BOOST_AUTO_TEST_CASE(duplicate_values_traversal)
{
  smirnov::AvlTree< int, std::string > tree;
  tree[1] = "one";
  tree[1] = "uno";
  tree[2] = "two";
  smirnov::KeySum collector;
  collector = tree.traverse_lnr(collector);
  BOOST_TEST(collector.result == 3);
  BOOST_TEST(collector.elems == "uno two");
}
