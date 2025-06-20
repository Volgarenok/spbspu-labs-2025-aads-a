#include <sstream>
#include <string>
#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include "tree.hpp"
#include "keySum.hpp"

using namespace brevnov;

BOOST_AUTO_TEST_CASE(traverse)
{
  AVLTree< int, std::string > tree({ { 13, "a" }, { 10, "b" }, { 18, "c" }, { 8, "d" }, { 11, "e" } });
  std::ostringstream iss;
  KeySum funLnr;
  funLnr = tree.traverse_lnr(funLnr);
  iss << funLnr.sum << funLnr.str;
  BOOST_TEST(iss.str() == "60 d b e a c");
  KeySum funRnl;
  funRnl = tree.traverse_rnl(funRnl);
  iss.str("");
  iss << funRnl.sum << funRnl.str;
  BOOST_TEST(iss.str() == "60 c a e b d");
  iss.str("");
  KeySum funBr;
  funBr = tree.traverse_breadth(funBr);
  iss << funBr.sum << funBr.str;
  BOOST_TEST(iss.str() == "60 a b c d e");
}
