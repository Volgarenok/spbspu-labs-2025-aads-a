#include <string>
#include <boost/test/unit_test.hpp>
#include "variant.hpp"

struct A
{};

BOOST_AUTO_TEST_SUITE(S2_variant_test)

BOOST_AUTO_TEST_CASE(print_info_test)
{
  rychkov::Variant< A, int32_t, char > variant;
  *rychkov::get_if< A >(&variant) = A();
  rychkov::Variant< A, int32_t, char, A > variant2{rychkov::in_place_index_t< 3 >()};
  rychkov::Variant< A, int32_t, char > variant3{rychkov::in_place_type_t< A >()};
  variant3 = variant;
  variant3 = std::move(variant);
  variant3 = 934;
  rychkov::Variant< A, int32_t, char > variant4 = std::move(variant3);
  rychkov::Variant< int32_t, A, char > variant5;
  variant5.emplace< A >();
  variant5.emplace< 2 >('!');
  rychkov::Variant< int32_t, A, char > variant6(72);
  rychkov::Variant< int32_t, A, char > variant7(variant6);
  rychkov::Variant< char, std::string > variant8 = "works";
  variant8 = "works twice";
}

BOOST_AUTO_TEST_SUITE_END()
