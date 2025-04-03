#include <boost/test/unit_test.hpp>
#include <sstream>

BOOST_AUTO_TEST_CASE(test_for_passing_tests)
{
  std::ostringstream out;
  out << 1;
  BOOST_TEST(out.str() == "1");
}
