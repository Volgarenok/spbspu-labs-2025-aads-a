#include <boost/test/unit_test.hpp>
#include <map.hpp>

BOOST_AUTO_TEST_CASE(insert_test)
{
  asafov::Map<int, std::string> map;
  BOOST_TEST(map.insert(1, "one"));
}

BOOST_AUTO_TEST_CASE(operator_brackets_test)
{
  asafov::Map<int, std::string> map;
  map[2] = "two";
  BOOST_CHECK_EQUAL(map[2], "two");
}

BOOST_AUTO_TEST_CASE(at_test)
{
  asafov::Map<int, std::string> map;
  map[3] = "three";
  BOOST_CHECK_EQUAL(map.at(3), "three");
}

BOOST_AUTO_TEST_CASE(find_test)
{
  asafov::Map<int, std::string> map;
  map[4] = "four";
  BOOST_CHECK(map.find(4) != map.end());
}

BOOST_AUTO_TEST_CASE(begin_end_test)
{
  asafov::Map<int, std::string> map;
  map[5] = "five";
  BOOST_CHECK(map.begin() != map.end());
}

BOOST_AUTO_TEST_CASE(size_test)
{
  asafov::Map<int, std::string> map;
  map[6] = "six";
  BOOST_TEST(map.size() == 1);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  asafov::Map<int, std::string> map;
  BOOST_TEST(map.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  asafov::Map<int, std::string> map;
  map[8] = "eight";
  asafov::Map<int, std::string> copy(map);
  BOOST_CHECK_EQUAL(copy[8], "eight");
}

BOOST_AUTO_TEST_CASE(copy_assignment_test)
{
  asafov::Map<int, std::string> map;
  map[9] = "nine";
  asafov::Map<int, std::string> assigned;
  assigned = map;
  BOOST_CHECK_EQUAL(assigned[9], "nine");
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  asafov::Map<int, std::string> map;
  map[10] = "ten";
  asafov::Map<int, std::string> moved(std::move(map));
  BOOST_CHECK_EQUAL(moved[10], "ten");
}

BOOST_AUTO_TEST_CASE(move_assignment_test)
{
  asafov::Map<int, std::string> map;
  map[11] = "eleven";
  asafov::Map<int, std::string> assigned;
  assigned = std::move(map);
  BOOST_CHECK_EQUAL(assigned[11], "eleven");
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  asafov::Map<int, std::string> a;
  asafov::Map<int, std::string> b;
  a[12] = "twelve";
  b.swap(a);
  BOOST_CHECK_EQUAL(b[12], "twelve");
}
