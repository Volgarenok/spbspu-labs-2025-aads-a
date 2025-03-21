#include <boost/test/unit_test.hpp>
#include "fwd_list.hpp"

BOOST_AUTO_TEST_CASE(test_element_access)
{
  karnauhova::Fwd_list< int > list;
  list.push_front(0);
  list.push_front(1);
  list.push_front(2);
  BOOST_TEST(list.front() == 2);
  BOOST_TEST(list.back() == 0);
}

BOOST_AUTO_TEST_CASE(test_capacity_list)
{
  karnauhova::Fwd_list< int > list;
  BOOST_TEST(list.empty() == true);
  list.push_front(0);
  list.push_front(1);
  list.push_front(2);
  BOOST_TEST(list.size() == 3);
}

BOOST_AUTO_TEST_CASE(test_clear_list)
{
  karnauhova::Fwd_list< int > list;
  list.push_front(0);
  list.push_front(1);
  list.push_front(2);
  list.clear();
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_reverse_list)
{
  karnauhova::Fwd_list< int > list;
  list.push_front(0);
  list.push_front(1);
  list.push_front(2);
  list.reverse();
  BOOST_TEST(list.front() == 0);
  BOOST_TEST(list.back() == 2);
}

BOOST_AUTO_TEST_CASE(test_remove)
{
  karnauhova::Fwd_list< int > list;
  list.push_front(0);
  list.push_front(1);
  list.push_front(1);
  list.push_front(2);
  list.reverse();
  list.remove(1);
  BOOST_TEST(list.front() == 0);
  BOOST_TEST(list.back() == 2);
}

BOOST_AUTO_TEST_CASE(test_erase)
{
  karnauhova::Fwd_list< int > list;
  list.push_front(0);
  list.push_front(1);
  list.push_front(1);
  list.push_front(2);
  list.reverse();
  auto it = list.cbegin();
  list.erase(it);
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.back() == 2);
}

