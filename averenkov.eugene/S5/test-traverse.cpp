#include <boost/test/unit_test.hpp>
#include <functional>
#include <string>
#include <limits>
#include "key_sum.hpp"
#include <BiTree.hpp>
#include <iostream>

using namespace averenkov;

namespace
{
  Tree< long long, std::string, std::less< long long > > create_test_tree()
  {
    Tree< long long, std::string, std::less< long long > > tree;
    tree.push( 8, "eight" );
    tree.push( 5, "five" );
    tree.push( 2, "two" );
    tree.push( 3, "three" );
    tree.push( 4, "four" );
    return tree;
  }
}

BOOST_AUTO_TEST_CASE(LNRTraverseTest)
{
  auto tree = create_test_tree();

  KeySum ks;
  ks = tree.traverse_lnr(ks);
  BOOST_TEST(ks.values_ == "two three four five eight ");
  BOOST_TEST(ks.sum_ == 22);

  const auto& constTree = tree;
  KeySum ks1;
  ks1 = constTree.traverse_lnr(ks1);
  BOOST_TEST(ks1.values_ == "two three four five eight ");
  BOOST_TEST(ks1.sum_ == 22);
}

BOOST_AUTO_TEST_CASE(RNLTraverseTest)
{
  auto tree = create_test_tree();

  KeySum ks;
  ks = tree.traverse_rnl(ks);
  BOOST_TEST(ks.values_ == "eight five four three two ");
  BOOST_TEST(ks.sum_ == 22);

  const auto& constTree = tree;
  KeySum ks1;
  ks1 = constTree.traverse_rnl(ks1);
  BOOST_TEST(ks1.values_ == "eight five four three two ");
  BOOST_TEST(ks1.sum_ == 22);
}

BOOST_AUTO_TEST_CASE(BreadthTraverseTest)
{
  auto tree = create_test_tree();

  KeySum ks;
  ks = tree.traverse_breadth(ks);
  BOOST_TEST(ks.values_ == "five three eight two four ");
  BOOST_TEST(ks.sum_ == 22);

  const auto& constTree = tree;
  KeySum ks1;
  ks1 = constTree.traverse_breadth(ks1);
  BOOST_TEST(ks1.values_ == "five three eight two four ");
  BOOST_TEST(ks1.sum_ == 22);
}

BOOST_AUTO_TEST_CASE(EmptyTreeTraverseTest)
{
  Tree< long long, std::string, std::less< long long > > emptyTree;

  KeySum ks;
  ks = emptyTree.traverse_lnr(ks);
  BOOST_TEST(ks.sum_ == 0);
  BOOST_TEST(ks.values_ == "");
}
