#include <boost/test/unit_test.hpp>
#include "FwdList.hpp"
#include "IteratorFwd.hpp"
#include "ConstIteratorFwd.hpp"
#include <iostream>

BOOST_AUTO_TEST_CASE(test_element_access)
{
  gavrilova::FwdList< int > list;
  list.push_front(0);
  list.push_front(1);
  BOOST_TEST(list.front() == 1);
}

BOOST_AUTO_TEST_CASE(test_reverse)
{
  gavrilova::FwdList< int > list;
  list.push_front(0);
  list.push_front(1);
  list.push_front(2);
  list.reverse();
  BOOST_TEST(list.front() == 0);
}

BOOST_AUTO_TEST_CASE(test_assignment_operator)
{
  gavrilova::FwdList< int > list1;
  list1.push_front(0);
  list1.push_front(1);
  list1.push_front(2);
  gavrilova::FwdList< int > list2;
  list2 = list1;
  BOOST_TEST(list1.size() == list2.size());
  BOOST_TEST(list2.front() == list1.front());
}
