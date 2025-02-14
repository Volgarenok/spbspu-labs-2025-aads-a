#include <boost/test/unit_test.hpp>
#include <sstream>
#include "credentials.hpp"

BOOST_AUTO_TEST_SUITE(credential_suite)

BOOST_AUTO_TEST_CASE(credentials_test_first)
{
  std::ostringstream out;
  evstyunichev::out_credentials(out);
  BOOST_TEST(out.str() == "evstyunichev.konstantin");
}

BOOST_AUTO_TEST_CASE(credentials_test_second)
{
  std::ostringstream out;
  evstyunichev::out_credentials(out);
  BOOST_TEST(out.str() == "evstyunichev.konstantin");
}

BOOST_AUTO_TEST_CASE(credentials_test_third)
{
  std::ostringstream out;
  evstyunichev::out_credentials(out);
  BOOST_TEST(out.str() == "evstyunichev.konstantin");
}

BOOST_AUTO_TEST_SUITE_END()
