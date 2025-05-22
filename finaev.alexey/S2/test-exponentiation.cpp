#include <boost/test/unit_test.hpp>
#include "mathOperations.hpp"

BOOST_AUTO_TEST_CASE(calculate_prime_degree)
{
  long long a = 2ll;
  long long b = 3ll;
  BOOST_TEST(finaev::count(a, b, '^') == 8);
}

BOOST_AUTO_TEST_CASE(overflow_exponentation)
{
  long long a = 4163462375ll;
  long long b = 24623462346ll;
  BOOST_CHECK_THROW(finaev::count(a, b, '^'), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(raized_zero_to_uncoorect_degree)
{
  long long a = 0ll;
  long long b = -124ll;
  BOOST_CHECK_THROW(finaev::count(a, b, '^'), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(sign_when_exponentiating_negative_number)
{
  long long a = -1ll;
  long long b = 5ll;
  long long c = 6ll;
  BOOST_TEST(finaev::count(a, b, '^') == -1);
  BOOST_TEST(finaev::count(a, c, '^') == 1);
}
