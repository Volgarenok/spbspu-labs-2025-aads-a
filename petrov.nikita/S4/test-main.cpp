#define BOOST_TEST_MODULE S4
#include <boost/test/included/unit_test.hpp>
#include <sstream>

BOOST_AUTO_TEST_CASE(fake_test)
{
  std::ostringstream out;
  out << 1;
  BOOST_TEST(out.str() == "1");
}
