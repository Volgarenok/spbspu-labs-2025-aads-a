#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>
#include <limits>
#include <tree/tree.hpp>
#include "key_summ.hpp"

BOOST_AUTO_TEST_CASE(traversal_simple_test)
{
  demehin::Tree< int, std::string > tree;

  tree[10] = "keyboard";
  tree[12] = "monitor";
  tree[-5] = "mouse";

  demehin::KeySumm res_lnr;
  demehin::KeySumm res_rnl;
  demehin::KeySumm res_br;

  res_lnr = tree.traverse_lnr(res_lnr);
  res_rnl = tree.traverse_rnl(res_rnl);
  res_br = tree.traverse_breadth(res_br);

  BOOST_TEST(res_lnr.val_res == 17);
  BOOST_TEST(res_rnl.val_res == 17);
  BOOST_TEST(res_br.val_res == 17);

  BOOST_TEST(res_lnr.str_res == "mouse keyboard monitor");
  BOOST_TEST(res_rnl.str_res == "monitor keyboard mouse");
  BOOST_TEST(res_br.str_res == "keyboard mouse monitor");
}

BOOST_AUTO_TEST_CASE(traversal_overflow_test)
{
  demehin::Tree< int, std::string > tree;

  tree[std::numeric_limits< int >::max()] = "max";
  tree[1] = "one";

  std::ostringstream out;
  demehin::KeySumm res;
  try
  {
    res = tree.traverse_breadth(res);
  }
  catch (const std::logic_error&)
  {
    out << "error";
  }

  BOOST_TEST(out.str() == "error");
}

BOOST_AUTO_TEST_CASE(traversal_big_tree_test)
{
  demehin::Tree< int , std::string > tree;
  tree[50] = "root";
  tree[30] = "left";
  tree[70] = "right";
  tree[20] = "leftleft";
  tree[40] = "leftright";
  tree[60] = "rightleft";
  tree[80] = "rightright";

  demehin::KeySumm lnr_res;
  demehin::KeySumm rnl_res;
  demehin::KeySumm br_res;

  lnr_res = tree.traverse_lnr(lnr_res);
  rnl_res = tree.traverse_rnl(rnl_res);
  br_res = tree.traverse_breadth(br_res);

  BOOST_TEST(lnr_res.val_res == 350);
  BOOST_TEST(rnl_res.val_res == 350);
  BOOST_TEST(br_res.val_res == 350);

  BOOST_TEST(lnr_res.str_res == "leftleft left leftright root rightleft right rightright");
  BOOST_TEST(rnl_res.str_res == "rightright right rightleft root leftright left leftleft");
  BOOST_TEST(br_res.str_res == "root left right leftleft leftright rightleft rightright");
}
