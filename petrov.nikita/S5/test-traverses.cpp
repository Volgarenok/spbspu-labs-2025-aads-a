#include <boost/test/unit_test.hpp>
#include <sstream>
#include <avl_tree.hpp>
#include "traverse_info_handler.hpp"

BOOST_AUTO_TEST_SUITE(traverse_lnr)

BOOST_AUTO_TEST_CASE(traverse_empty_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  petrov::SumUpKeysDisplayValues summator;
  petrov::SumUpKeysDisplayValues new_summator = tree.traverse_lnr(summator);
  out << new_summator.result;
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_CASE(traverse_one_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 5, "good" });
  petrov::SumUpKeysDisplayValues summator;
  petrov::SumUpKeysDisplayValues new_summator = tree.traverse_rnl(summator);
  out << new_summator.result;
  BOOST_TEST(out.str() == "5");
}

BOOST_AUTO_TEST_CASE(traverse_three_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 5, "good" });
  tree.insert({ 6, "vibes" });
  tree.insert({ 7, "forever" });
  petrov::SumUpKeysDisplayValues summator;
  petrov::SumUpKeysDisplayValues new_summator = tree.traverse_lnr(summator);
  out << new_summator.result;
  BOOST_TEST(out.str() == "18");
}

BOOST_AUTO_TEST_CASE(traverse_five_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  petrov::SumUpKeysDisplayValues summator;
  tree.insert({ 5, "good" });
  tree.insert({ 6, "vibes" });
  tree.insert({ 7, "forever" });
  tree.insert({ 15, "trust" });
  tree.insert({ 16, "me" });
  petrov::SumUpKeysDisplayValues new_summator = tree.traverse_lnr(summator);
  out << new_summator.result;
  BOOST_TEST(out.str() == "49");
}

BOOST_AUTO_TEST_CASE(traverse_five_element_tree_output)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  petrov::SumUpKeysDisplayValues summator;
  tree.insert({ 5, "good" });
  tree.insert({ 6, "vibes" });
  tree.insert({ 7, "forever" });
  tree.insert({ 15, "trust" });
  tree.insert({ 16, "me" });
  petrov::SumUpKeysDisplayValues new_summator = tree.traverse_lnr(summator);
  new_summator.displayValuesInTraverseOrder(out);
  BOOST_TEST(out.str() == "good vibes forever trust me");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(traverse_rnl)

BOOST_AUTO_TEST_CASE(traverse_empty_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  petrov::SumUpKeysDisplayValues summator;
  petrov::SumUpKeysDisplayValues new_summator = tree.traverse_rnl(summator);
  out << new_summator.result;
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_CASE(traverse_one_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 5, "good" });
  petrov::SumUpKeysDisplayValues summator;
  petrov::SumUpKeysDisplayValues new_summator = tree.traverse_rnl(summator);
  out << new_summator.result;
  BOOST_TEST(out.str() == "5");
}

BOOST_AUTO_TEST_CASE(traverse_three_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 5, "good" });
  tree.insert({ 6, "vibes" });
  tree.insert({ 7, "forever" });
  petrov::SumUpKeysDisplayValues summator;
  petrov::SumUpKeysDisplayValues new_summator = tree.traverse_rnl(summator);
  out << new_summator.result;
  BOOST_TEST(out.str() == "18");
}

BOOST_AUTO_TEST_CASE(traverse_five_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  petrov::SumUpKeysDisplayValues summator;
  tree.insert({ 5, "good" });
  tree.insert({ 6, "vibes" });
  tree.insert({ 7, "forever" });
  tree.insert({ 15, "trust" });
  tree.insert({ 16, "me" });
  petrov::SumUpKeysDisplayValues new_summator = tree.traverse_rnl(summator);
  out << new_summator.result;
  BOOST_TEST(out.str() == "49");
}

BOOST_AUTO_TEST_CASE(traverse_five_element_tree_output)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  petrov::SumUpKeysDisplayValues summator;
  tree.insert({ 5, "good" });
  tree.insert({ 6, "vibes" });
  tree.insert({ 7, "forever" });
  tree.insert({ 15, "trust" });
  tree.insert({ 16, "me" });
  petrov::SumUpKeysDisplayValues new_summator = tree.traverse_rnl(summator);
  new_summator.displayValuesInTraverseOrder(out);
  BOOST_TEST(out.str() == "me trust forever vibes good");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(traverse_breadth)

BOOST_AUTO_TEST_CASE(traverse_empty_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  petrov::SumUpKeysDisplayValues summator;
  petrov::SumUpKeysDisplayValues new_summator = tree.traverse_breadth(summator);
  out << new_summator.result;
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_CASE(traverse_one_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 5, "good" });
  petrov::SumUpKeysDisplayValues summator;
  petrov::SumUpKeysDisplayValues new_summator = tree.traverse_breadth(summator);
  out << new_summator.result;
  BOOST_TEST(out.str() == "5");
}

BOOST_AUTO_TEST_CASE(traverse_three_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 5, "good" });
  tree.insert({ 6, "vibes" });
  tree.insert({ 7, "forever" });
  petrov::SumUpKeysDisplayValues summator;
  petrov::SumUpKeysDisplayValues new_summator = tree.traverse_breadth(summator);
  out << new_summator.result;
  BOOST_TEST(out.str() == "18");
}

BOOST_AUTO_TEST_CASE(traverse_five_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  petrov::SumUpKeysDisplayValues summator;
  tree.insert({ 5, "good" });
  tree.insert({ 6, "vibes" });
  tree.insert({ 7, "forever" });
  tree.insert({ 15, "trust" });
  tree.insert({ 16, "me" });
  petrov::SumUpKeysDisplayValues new_summator = tree.traverse_breadth(summator);
  out << new_summator.result;
  BOOST_TEST(out.str() == "49");
}

BOOST_AUTO_TEST_CASE(traverse_five_element_tree_output)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  petrov::SumUpKeysDisplayValues summator;
  tree.insert({ 5, "good" });
  tree.insert({ 6, "vibes" });
  tree.insert({ 7, "forever" });
  tree.insert({ 15, "trust" });
  tree.insert({ 17, "me" });
  petrov::SumUpKeysDisplayValues new_summator = tree.traverse_breadth(summator);
  new_summator.displayValuesInTraverseOrder(out);
  BOOST_TEST(out.str() == "vibes good trust forever me");
}

BOOST_AUTO_TEST_SUITE_END()

