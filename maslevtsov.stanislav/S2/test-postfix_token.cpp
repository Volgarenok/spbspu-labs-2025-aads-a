#include <boost/test/unit_test.hpp>
#include "postfix_token.hpp"

BOOST_AUTO_TEST_SUITE(postfix_token_constructor_tests)
BOOST_AUTO_TEST_CASE(convert_constructor_test)
{
  maslevtsov::PostfixToken exp("1 + 1");
  BOOST_TEST(exp() == 2);
}
BOOST_AUTO_TEST_SUITE_END()
