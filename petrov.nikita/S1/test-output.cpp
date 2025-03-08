#include <boost/test/unit_test.hpp>
#include <sstream>

BOOST_AUTO_TEST_CASE(output_test)
{
  std::ostringstream out;
  out << "FIRST";
  BOOST_TEST(out.str() == "FIRST");
}
