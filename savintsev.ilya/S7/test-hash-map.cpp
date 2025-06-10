#include <boost/test/unit_test.hpp>
#include <boost/hash2/xxhash.hpp>
#include <string>
#include <iostream>
#include "hash-map.hpp"

using namespace savintsev;

BOOST_AUTO_TEST_CASE(hm_test_operator_sq)
{
  HashMap< int, std::string > book(10);

  book[1] = "apple";
  book[2] = "orange";
  book[3] = "banana";

  BOOST_TEST(book[1] == "apple");
  BOOST_TEST(book[2] == "orange");
  BOOST_TEST(book[3] == "banana");

  HashMap< std::string, std::string > cringe;

  cringe["lox1"] = "apple";
  cringe["lox2"] = "orange";
  cringe["lox3"] = "banana";

  BOOST_TEST(cringe["lox1"] == "apple");
  BOOST_TEST(cringe["lox2"] == "orange");
  BOOST_TEST(cringe["lox3"] == "banana");
}
