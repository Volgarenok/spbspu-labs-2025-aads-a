#include <boost/test/unit_test.hpp>
#include <functional>
#include <string>
#include <limits>
#include <tree.hpp>
#include "KeyValue.hpp"

using namespace duhanina;

namespace
{
  Tree< long long, std::string, std::less< long long > > create_test_tree()
  {
    Tree< long long, std::string, std::less< long long > > tree;
    tree[5] = "five";
    tree[3] = "three";
    tree[7] = "seven";
    tree[2] = "two";
    tree[4] = "four";
    tree[6] = "six";
    tree[8] = "eight";
    return tree;
  }

  Tree< long long, std::string, std::less< long long > > create_overflow_tree()
  {
    Tree< long long, std::string, std::less< long long > > tree;
    tree[std::numeric_limits< long long >::max()] = "max";
    tree[1] = "one";
    return tree;
  }

  Tree< long long, std::string, std::less< long long > > create_underflow_tree()
  {
    Tree< long long, std::string, std::less< long long > > tree;
    tree[std::numeric_limits< long long >::min()] = "min";
    tree[-1] = "minus one";
    return tree;
  }
}

BOOST_AUTO_TEST_CASE(LNRTraverseTest)
{
  auto tree = create_test_tree();

  KeySum ks;
  ks = tree.traverse_lnr(ks);
  BOOST_TEST(ks.value == "two three four five six seven eight");
  BOOST_TEST(ks.key == 35);

  const auto& constTree = tree;
  KeySum ksConst;
  ksConst = constTree.traverse_lnr(ksConst);
  BOOST_TEST(ksConst.value == "two three four five six seven eight");
}

BOOST_AUTO_TEST_CASE(RNLTraverseTest)
{
  auto tree = create_test_tree();

  KeySum ks;
  ks = tree.traverse_rnl(ks);
  BOOST_TEST(ks.value == "eight seven six five four three two");
  BOOST_TEST(ks.key == 35);

  const auto& constTree = tree;
  KeySum ksConst;
  ksConst = constTree.traverse_rnl(ksConst);
  BOOST_TEST(ksConst.value == "eight seven six five four three two");
}

BOOST_AUTO_TEST_CASE(BreadthTraverseTest)
{
  auto tree = create_test_tree();

  KeySum ks;
  ks = tree.traverse_breadth(ks);
  BOOST_TEST(ks.value == "five three seven two four six eight");
  BOOST_TEST(ks.key == 35);

  const auto& constTree = tree;
  KeySum ksConst;
  ksConst = constTree.traverse_breadth(ksConst);
  BOOST_TEST(ksConst.value == "five three seven two four six eight");
}

BOOST_AUTO_TEST_CASE(EmptyTreeTraverseTest)
{
  Tree< long long, std::string, std::less< long long > > emptyTree;

  KeySum ks;
  ks = emptyTree.traverse_lnr(ks);
  BOOST_TEST(ks.value.empty());
  BOOST_TEST(ks.key == 0);
}

BOOST_AUTO_TEST_CASE(KeySumOverflowThrowsTest)
{
  auto tree = create_overflow_tree();
  KeySum ks;
  bool exceptionThrown = false;
  try
  {
    ks = tree.traverse_lnr(ks);
  }
  catch (...)
  {
    exceptionThrown = true;
  }
  BOOST_TEST(exceptionThrown);

  auto tree1 = create_underflow_tree();
  KeySum ks1;
  bool exceptionThrown1 = false;
  try
  {
    ks1 = tree1.traverse_lnr(ks1);
  }
  catch (...)
  {
    exceptionThrown1 = true;
  }
  BOOST_TEST(exceptionThrown1);
}
