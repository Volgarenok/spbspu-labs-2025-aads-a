#include <boost/test/unit_test.hpp>

#include <sstream>
#include <string>
#include <tree/avl_tree.hpp>
#include "key_sum.hpp"

BOOST_AUTO_TEST_CASE(traversal_lnr_test)
{
  karnauhova::AvlTree< int, std::string > tree;
  tree[5] = "root";
  tree[3] = "left";
  tree[7] = "right";
  tree[2] = "leftleft";
  tree[4] = "leftright";
  tree[6] = "rightleft";
  tree[8] = "rightright";

  karnauhova::KeySum res_lnr;
  res_lnr = tree.traverse_lnr(res_lnr);
  BOOST_TEST(res_lnr.result_ == 35);
  BOOST_TEST(res_lnr.itInput_ == "leftleft left leftright root rightleft right rightright");
}

BOOST_AUTO_TEST_CASE(traversal_rnl_test)
{
  karnauhova::AvlTree< int, std::string > tree;
  tree[5] = "root";
  tree[3] = "left";
  tree[7] = "right";
  tree[2] = "leftleft";
  tree[4] = "leftright";
  tree[6] = "rightleft";
  tree[8] = "rightright";

  karnauhova::KeySum res_rnl;
  res_rnl = tree.traverse_rnl(res_rnl);
  BOOST_TEST(res_rnl.result_ == 35);
  BOOST_TEST(res_rnl.itInput_ == "rightright right rightleft root leftright left leftleft");
}

BOOST_AUTO_TEST_CASE(traversal_breadth_test)
{
  karnauhova::AvlTree< int, std::string > tree;
  tree[5] = "root";
  tree[3] = "left";
  tree[7] = "right";
  tree[2] = "leftleft";
  tree[4] = "leftright";
  tree[6] = "rightleft";
  tree[8] = "rightright";

  karnauhova::KeySum res_breadth;
  res_breadth = tree.traverse_breadth(res_breadth);
  BOOST_TEST(res_breadth.result_ == 35);
  BOOST_TEST(res_breadth.itInput_ == "root left right leftleft leftright rightleft rightright");
}
