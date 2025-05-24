#include <boost/test/unit_test.hpp>
#include <tree/tree-2-3.hpp>
#include "tree-sum-functor.hpp"

BOOST_AUTO_TEST_CASE(test_traverses)
{
  using namespace alymova;
  using Tree = alymova::TwoThreeTree< int, std::string, std::less< int > >;

  TreeSum result;
  Tree tree;
  BOOST_TEST(tree.traverse_lnr(result).keys == 0);
  BOOST_TEST(tree.traverse_rnl(result).keys == 0);
  BOOST_TEST(tree.traverse_breadth(result).keys == 0);

  tree.emplace(1, "a");
  tree.emplace(2, "b");
  tree.emplace(3, "c");

  TreeSum result1;
  result1 = tree.traverse_lnr(result1);
  BOOST_TEST(result1.keys == 6);
  BOOST_TEST(result1.values.front() == "a");
  result1.values.pop();
  BOOST_TEST(result1.values.front() == "b");
  result1.values.pop();
  BOOST_TEST(result1.values.front() == "c");
  result1.values.pop();

  TreeSum result2;
  result2 = tree.traverse_rnl(result2);
  BOOST_TEST(result2.keys == 6);
  BOOST_TEST(result2.values.front() == "c");
  result2.values.pop();
  BOOST_TEST(result2.values.front() == "b");
  result2.values.pop();
  BOOST_TEST(result2.values.front() == "a");
  result2.values.pop();

  TreeSum result3;
  result3 = tree.traverse_breadth(result3);
  BOOST_TEST(result3.keys == 6);
  BOOST_TEST(result3.values.front() == "b");
  result3.values.pop();
  BOOST_TEST(result3.values.front() == "a");
  result3.values.pop();
  BOOST_TEST(result3.values.front() == "c");
  result3.values.pop();
}
