#include <boost/test/unit_test.hpp>
#include <sstream>
#include "credentials.h"

BOOST_AUTO_TEST_CASE(credentials_test)
{
  std::ostringstream out;
  asafov::idents(out);
  BOOST_TEST(out.str() == "asafov.andrew");
}
