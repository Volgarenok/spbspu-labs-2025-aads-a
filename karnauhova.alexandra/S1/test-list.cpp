#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_CASE(test_element_access)
{
  karnauhova::List< int > list;
  list.push_front(0);
  list.push_front(1);
  list.push_front(2);
  BOOST_TEST(list.front() == 2);
  BOOST_TEST(list.back() == 0);
}

BOOST_AUTO_TEST_CASE(test_capacity_list)
{
  karnauhova::List< int > list;
  BOOST_TEST(list.empty() == true);
  list.push_front(0);
  list.push_front(1);
  list.push_front(2);
  BOOST_TEST(list.size() == 3);
}

BOOST_AUTO_TEST_CASE(test_clear_list)
{
  karnauhova::List< int > list;
  list.push_front(0);
  list.push_front(1);
  list.push_front(2);
  list.clear();
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_reverse_list)
{
  karnauhova::List< int > list;
  list.push_front(0);
  list.push_front(1);
  list.push_front(2);
  list.reverse();
  BOOST_TEST(list.front() == 0);
  BOOST_TEST(list.back() == 2);
}

