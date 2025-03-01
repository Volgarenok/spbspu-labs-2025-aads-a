#include <boost/test/unit_test.hpp>
#include <list>
#include <sstream>

BOOST_AUTO_TEST_SUITE(main_tests)

BOOST_AUTO_TEST_CASE(some_input)
{
  std::list< int > l1{}, l2{1};
  l1.push_back(1);
  BOOST_TEST(l1 == l2);
}

BOOST_AUTO_TEST_SUITE_END()
