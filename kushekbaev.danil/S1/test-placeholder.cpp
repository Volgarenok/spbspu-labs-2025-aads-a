#include <boost/test/unit_test.hpp>
#include <sstream>

BOOST_AUTO_TEST_CASE(placeholder_test)
{
  std::ostringstream out;
  out << "ЯЛКВМОМ";
  BOOST_TEST(out.str() == "ЯЛКВМОМ");
}
