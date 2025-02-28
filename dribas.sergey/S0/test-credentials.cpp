#include <boost/test/unit_test.hpp>
#include <sstream>
#include "credentials.hpp"

BOOST_AUTO_TEST_SUITE(credentials_test)

BOOST_AUTO_TEST_CASE(credentials_test)
{
  std::ostringstream out;
  dribas::outCredentials(out);
  BOOST_TEST(out.str() == "dribas.sergey");
}

BOOST_AUTO_TEST_SUITE_END()
