#include <boost/test/unit_test.hpp>
#include <sstream>
#include "credentials.hpp"

BOOST_AUTO_TEST_CASE(credentialsTest)
{
  std::ostringstream out;
  maslov::outCredentials(out);
  BOOST_TEST(out.str() == "maslov.aleksey");
}
