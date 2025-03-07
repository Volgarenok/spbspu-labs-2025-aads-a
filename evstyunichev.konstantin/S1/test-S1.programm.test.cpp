#include <boost/test/unit_test.hpp>
#include <sstream>
#include "../common/list.hpp"

BOOST_AUTO_TEST_SUITE(main_tests)

BOOST_AUTO_TEST_CASE(push_back_test)
{
  evstyunichev::List< int > list1{};
  list1.push_back(1);
  list1.push_back(2);
  std::stringstream out;
  list1.out(out);
  BOOST_TEST(out.str() == "1 2");
}

BOOST_AUTO_TEST_CASE(front_back_test)
{
  evstyunichev::List< int > list1{};
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  BOOST_TEST(list1.front() == 1);
  BOOST_TEST(list1.back() == 3);
  list1.pop_back();
  BOOST_TEST(list1.back() == 2);
}

BOOST_AUTO_TEST_CASE(iterator_test)
{
  evstyunichev::List< int > list1{};
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  std::stringstream out;
  for (auto it = list1.begin(); it != list1.end(); ++it)
  {
    out << *it;
  }
  BOOST_TEST(out.str() == "123");
}

BOOST_AUTO_TEST_SUITE_END()
