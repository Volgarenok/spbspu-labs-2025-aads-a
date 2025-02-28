#include <boost/test/unit_test.hpp>
#include <sstream>
#include "credentials.h"

BOOST_AUTO_TEST_CASE(credentials_test)
{
  std::ostringstream out;
  aleksandrov::outputCredentials(out);
  BOOST_TEST(out.str() == "aleksandrov.igor");
}

