#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
#include <AVLtree.hpp>
#include "key_summ.hpp"

using namespace tkach;
template< class Key, class Value >
void print(const AvlTree< Key, Value >& avltree, std::ostream& out)
{
  out << avltree.cbegin()->first << " " << avltree.cbegin()->second;
  for (auto it = ++avltree.cbegin(); it != avltree.cend(); ++it)
  {
    out << " " << it->first << " " << it->second;
  }
}

BOOST_AUTO_TEST_CASE(base_traverse_const_test)
{
  AvlTree< int, std::string > tree;
  tree.insert(std::make_pair(2, "second"));
  tree.insert(std::make_pair(1, "first"));
  tree.insert(std::make_pair(3, "third"));
  const auto& constTree = tree;
  KeySumm res_lnr, res_rnl, res_nlr;
  res_lnr = constTree.traverseLnr(res_lnr);
  res_rnl = constTree.traverseRnl(res_rnl);
  res_nlr = constTree.traverseBreadth(res_nlr);
  BOOST_TEST(res_lnr.key_sum == 6);
  BOOST_TEST(res_rnl.key_sum == 6);
  BOOST_TEST(res_nlr.key_sum == 6);
  BOOST_TEST(res_lnr.values == "first second third");
  BOOST_TEST(res_rnl.values == "third second first");
  BOOST_TEST(res_nlr.values == "second first third");
}


BOOST_AUTO_TEST_CASE(base_traverse_test)
{
  AvlTree< int, std::string > tree;
  tree.insert(std::make_pair(2, "second"));
  tree.insert(std::make_pair(1, "first"));
  tree.insert(std::make_pair(3, "third"));
  KeySumm res_lnr, res_rnl, res_nlr;
  res_lnr = tree.traverseLnr(res_lnr);
  res_rnl = tree.traverseRnl(res_rnl);
  res_nlr = tree.traverseBreadth(res_nlr);
  BOOST_TEST(res_lnr.key_sum == 6);
  BOOST_TEST(res_rnl.key_sum == 6);
  BOOST_TEST(res_nlr.key_sum == 6);
  BOOST_TEST(res_lnr.values == "first second third");
  BOOST_TEST(res_rnl.values == "third second first");
  BOOST_TEST(res_nlr.values == "second first third");
}
