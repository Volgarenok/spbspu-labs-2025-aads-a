#include <sstream>
#include <boost/test/unit_test.hpp>
#include "credentials.hpp"
BOOST_AUTO_TEST_CASE(credentials_test)
{
  std::ostringstream output;
  kiselev::credentials(output);
  BOOST_TEST(output.str() == "kiselev.sergey");
}
