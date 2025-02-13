#include <boost/test/unit_tets.hpp>
#include <sstream>
#include "credentials.hpp"

BOOOST_AUTO_TEST_CASE(test-credentials)
{
  std::ostringstream out;
  shramko::outCredentials(out);
  BOOST_TEST(out.str() == "shramko.maksim");
}
