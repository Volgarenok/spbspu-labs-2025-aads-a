#include <boost/test/unit_test.hpp>
#include <AVLtree.hpp>
#include <string>
#include "functionalObject.hpp"

using Tree = finaev::AVLtree< int, std::string >;

BOOST_AUTO_TEST_CASE(traverses_test)
{
  Tree tree;

  tree.insert(std::make_pair(2, "second"));
  tree.insert(std::make_pair(1, "first"));
  tree.insert(std::make_pair(3, "third"));

  const auto& constTree = tree;
  finaev::keySum res_lnr, res_rnl, res_nlr;

  res_lnr = constTree.traverse_lnr(res_lnr);
  res_rnl = constTree.traverse_rnl(res_rnl);
  res_nlr = constTree.traverse_breadth(res_nlr);

  BOOST_TEST(res_lnr.values == "first second third");
  BOOST_TEST(res_rnl.values == "third second first");
  BOOST_TEST(res_nlr.values == "second first third");
}
