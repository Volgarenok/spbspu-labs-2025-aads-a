#include <memory>
#include <sstream>
#include <boost/test/unit_test.hpp>
#include "out-name.hpp"

BOOST_AUTO_TEST_CASE(out_name)
{
  const std::string expectedOut = "kizhin.evgeniy";
  std::stringstream out;
  std::ostream& result = kizhin::outName(out);
  BOOST_TEST(std::addressof(result) == std::addressof(out));
  BOOST_TEST(out.str() == expectedOut);
  BOOST_TEST(out.good());
}

