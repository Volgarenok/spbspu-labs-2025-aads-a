#include <boost/test/unit_test.hpp>
#include <sstream>
#include "../common/list.hpp"

BOOST_AUTO_TEST_SUITE(main_tests)

BOOST_AUTO_TEST_CASE(construct_test)
{
  using list_ull = evstyunichev::List< size_t >;
  list_ull list1{}, list2{ list1 };
}

BOOST_AUTO_TEST_CASE(push_back_test)
{
  evstyunichev::List< int > list1{};
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  std::stringstream out;
  list1.out(out);
  BOOST_TEST(out.str() == "1 2 3");
}

BOOST_AUTO_TEST_CASE(front_back_test)
{
  evstyunichev::List< int > list2{};
  list2.push_back(2);
  list2.push_front(1);
  list2.push_front(3);
  BOOST_CHECK(list2.front() == 3);
  BOOST_CHECK(list2.back() == 2);
}

BOOST_AUTO_TEST_CASE(pop_test)
{
  evstyunichev::List< int > list3{};
  list3.push_back(2);
  list3.push_back(3);
  BOOST_CHECK(list3.back() == 3);
  list3.pop_back();
  BOOST_CHECK(list3.back() == 2);
  list3.push_front(4);
  list3.push_front(6);
  list3.pop_front();
  BOOST_CHECK(list3.front() == 4);
}

BOOST_AUTO_TEST_SUITE_END()
