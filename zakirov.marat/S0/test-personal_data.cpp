#include <boost/test/unit_test.hpp>
#include <sstream>
#include "personal_data.hpp"

BOOST_AUTO_TEST_CASE(personal_data_test)
{
  std::ostringstream out;
  zakirov::out_personal_data(out);
  BOOST_TEST(out.str() == "zakirov.marat");
}
