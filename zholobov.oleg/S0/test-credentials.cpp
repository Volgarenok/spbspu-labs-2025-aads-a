#include <boost/test/unit_test.hpp>
#include <sstream>

#include "credentials.hpp"

BOOST_AUTO_TEST_SUITE(credentials_suite)

BOOST_AUTO_TEST_CASE(credentials_test)
{
  std::stringstream ss;
  zholobov::out_credentials(ss);
  BOOST_TEST(ss.str() == "zholobov.oleg");
}

BOOST_AUTO_TEST_SUITE_END()
