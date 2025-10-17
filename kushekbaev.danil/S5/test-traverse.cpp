#include <boost/test/unit_test.hpp>
#include <functional>
#include <string>
#include <limits>
#include "keysum.hpp"
#include <tree.hpp>
#include <iostream>

using namespace kushekbaev;

namespace
{
  Tree< long long int, std::string, std::less< long long int > > create_test_tree()
  {
    Tree< long long int, std::string, std::less< long long int > > tree;
    tree.insert(std::make_pair(8, "eight" ));
    tree.insert(std::make_pair(4, "four" ));
    tree.insert(std::make_pair(1, "one" ));
    tree.insert(std::make_pair(3, "three" ));
    return tree;
  }
}

BOOST_AUTO_TEST_CASE(LNRTraverseTest)
{
  auto tree = create_test_tree();
  KeySum ks;
  ks = tree.traverse_lnr(ks);
  BOOST_TEST(ks.value == "one three four eight ");
  BOOST_TEST(ks.sum == 16);
  const auto& constTree = tree;
  KeySum constks;
  constks = constTree.traverse_lnr(constks);
  BOOST_TEST(constks.value == "one three four eight ");
  BOOST_TEST(constks.sum == 16);
}

BOOST_AUTO_TEST_CASE(RNLTraverseTest)
{
  auto tree = create_test_tree();
  KeySum ks;
  ks = tree.traverse_rnl(ks);
  BOOST_TEST(ks.value == "eight four three one ");
  BOOST_TEST(ks.sum == 16);
  const auto& constTree = tree;
  KeySum constks;
  constks = constTree.traverse_rnl(constks);
  BOOST_TEST(constks.value == "eight four three one ");
  BOOST_TEST(constks.sum == 16);
}

BOOST_AUTO_TEST_CASE(BreadthTraverseTest)
{
  auto tree = create_test_tree();
  KeySum ks;
  ks = tree.traverse_breadth(ks);
  BOOST_TEST(ks.value == "eight four one three ");
  BOOST_TEST(ks.sum == 16);
  const auto& constTree = tree;
  KeySum constks;
  constks = constTree.traverse_breadth(constks);
  BOOST_TEST(constks.value == "eight four one three ");
  BOOST_TEST(constks.sum == 16);
}

BOOST_AUTO_TEST_CASE(EmptyTreeTraverseTest)
{
  Tree< long long int, std::string, std::less< long long int > > emptyTree;
  KeySum ks;
  ks = emptyTree.traverse_lnr(ks);
  BOOST_TEST(ks.sum == 0);
  BOOST_TEST(ks.value == "");
}
