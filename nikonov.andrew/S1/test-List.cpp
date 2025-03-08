#include <boost/test/unit_test.hpp>
#include <sstream>
#include "List.hpp"

BOOST_AUTO_TEST_CASE(List_test)
{
  nikonov::List< int > list;
  BOOST_TEST(list.size() == 0);
}
