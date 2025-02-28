#include <sstream>
#include <boost/test/unit_test.hpp>
#include "credentials.hpp"

BOOST_AUTO_TEST_SUITE(S0_main_test)

BOOST_AUTO_TEST_CASE(S0_credentials_test)
{
  std::stringstream testout;
  rychkov::printCredentials(testout);
  BOOST_TEST(testout.str() == "rychkov.mihail");
}

BOOST_AUTO_TEST_SUITE_END()
