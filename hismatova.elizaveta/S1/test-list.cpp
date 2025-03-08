#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_CASE(emptyList)
{
  List list;
  BOOST_TEST(list.empty());
}
