#include <sstream>
#include <string>
#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "tree.hpp"
#include "keySum.hpp"

using namespace kiselev;

BOOST_AUTO_TEST_CASE(traverse)
{
  RBTree< int, std::string > tree({ { 10, "a" }, { 7, "b" }, { 15, "c" }, { 5, "d" }, { 8, "e" } });
  std::ostringstream iss;
  KeySum funLnr;
  funLnr = tree.traverse_lnr(funLnr);
  iss << funLnr.sum << funLnr.str;
  BOOST_TEST(iss.str() == "45 d b e a c");
  KeySum funRnl;
  funRnl = tree.traverse_rnl(funRnl);
  iss.str("");
  iss << funRnl.sum << funRnl.str;
  BOOST_TEST(iss.str() == "45 c a e b d");
  iss.str("");
  KeySum funBreadth;
  funBreadth = tree.traverse_breadth(funBreadth);
  iss << funBreadth.sum << funBreadth.str;
  BOOST_TEST(iss.str() == "45 a b c d e");
}
