#include <boost/test/unit_test.hpp>
#include <string>
#include <avl_tree/AVLtree.hpp>
#include "functionalObject.hpp"

using Tree = sharifullina::AVLtree< int, std::string >;

BOOST_AUTO_TEST_CASE(traverses_test)
{
  Tree tree;

  tree.insert(std::make_pair(2, "second"));
  tree.insert(std::make_pair(1, "first"));
  tree.insert(std::make_pair(3, "third"));

  const auto& constTree = tree;
  sharifullina::Collector res_lnr, res_rnl, res_nlr;

  res_lnr = constTree.traverse_lnr(res_lnr);
  BOOST_TEST(res_lnr.values == "first second third");

  res_rnl = constTree.traverse_rnl(res_rnl);
  BOOST_TEST(res_rnl.values == "third second first");

  res_nlr = constTree.traverse_breadth(res_nlr);
  BOOST_TEST(res_nlr.values == "second first third");
}
