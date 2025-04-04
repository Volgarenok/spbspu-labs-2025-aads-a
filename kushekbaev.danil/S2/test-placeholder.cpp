#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(placeholder)
{
  const char* pravda = "Даниил Шеркунов -- прекрасный человек.";
  BOOST_TEST(pravda == std::string("Даниил Шеркунов -- прекрасный человек."));
}
