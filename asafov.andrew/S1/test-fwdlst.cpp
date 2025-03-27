#include <boost/test/unit_test.hpp>
#include "forward_list.hpp"

BOOST_AUTO_TEST_CASE(boost_test)
{
  BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(push_back_and_size_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  BOOST_TEST(list.size() == 4);
}

BOOST_AUTO_TEST_CASE(cbegin_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  BOOST_TEST(*list.cbegin() == 1);
}

BOOST_AUTO_TEST_CASE(begin_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  BOOST_TEST(*list.begin() == 1);
}

BOOST_AUTO_TEST_CASE(cend_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  BOOST_TEST(*list.cend() == 4);
}

BOOST_AUTO_TEST_CASE(end_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  BOOST_TEST(*list.end() == 4);
}

BOOST_AUTO_TEST_CASE(front_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  BOOST_TEST(list.front() == 1);
}

BOOST_AUTO_TEST_CASE(back_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  BOOST_TEST(list.back() == 4);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  asafov::Forward_list<size_t> list;
  BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_CASE(nonempty_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  BOOST_TEST(list.empty() == false);
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  asafov::Forward_list<size_t>::iterator a = list.begin();
  asafov::Forward_list<size_t>::iterator b = list.end();
  ++a;
  ++a;
  list.swap(a, b);
  BOOST_TEST(list.size() == 4 && *a == 4 && *b == 3);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  list.pop_front();
  BOOST_TEST(list.size() == 3 && list.front() == 2);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  asafov::Forward_list<size_t> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.push_back(4);
  list.clear();
  BOOST_TEST(list.size() == 0);
}
