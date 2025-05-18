#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_CASE(DefaultConstructorTest)
{
  smirnov::List< int > list;
  BOOST_CHECK(list.empty());
  BOOST_CHECK(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(PushBackTest)
{
  smirnov::List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  BOOST_CHECK(list.size() == 3);
  BOOST_CHECK(list.front() == 1);
  BOOST_CHECK(list.back() == 3);
}

BOOST_AUTO_TEST_CASE(PushFrontTest)
{
  smirnov::List< int > list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);

  BOOST_CHECK(list.size() == 3);
  BOOST_CHECK(list.front() == 3);
  BOOST_CHECK(list.back() == 1);
}

BOOST_AUTO_TEST_CASE(PopBackTest)
{
  smirnov::List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  list.pop_back();
  BOOST_CHECK(list.size() == 2);
  BOOST_CHECK(list.back() == 2);

  list.pop_back();
  BOOST_CHECK(list.size() == 1);
  BOOST_CHECK(list.back() == 1);

  list.pop_back();
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(PopFrontTest)
{
  smirnov::List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  list.pop_front();
  BOOST_CHECK(list.size() == 2);
  BOOST_CHECK(list.front() == 2);

  list.pop_front();
  BOOST_CHECK(list.size() == 1);
  BOOST_CHECK(list.front() == 3);

  list.pop_front();
  BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(CopyConstructorTest)
{
  smirnov::List< int > list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);

  smirnov::List< int > list2(list1);

  BOOST_CHECK(list2.size() == 3);
  BOOST_CHECK(list2.front() == 1);
  BOOST_CHECK(list2.back() == 3);
}

BOOST_AUTO_TEST_CASE(MoveConstructorTest)
{
  smirnov::List< int > list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);

  smirnov::List< int > list2(std::move(list1));

  BOOST_CHECK(list2.size() == 3);
  BOOST_CHECK(list2.front() == 1);
  BOOST_CHECK(list2.back() == 3);
  BOOST_CHECK(list1.empty());
}

BOOST_AUTO_TEST_CASE(CopyAssignmentTest)
{
  smirnov::List< int > list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);

  smirnov::List< int > list2;
  list2 = list1;

  BOOST_CHECK(list2.size() == 3);
  BOOST_CHECK(list2.front() == 1);
  BOOST_CHECK(list2.back() == 3);
}

BOOST_AUTO_TEST_CASE(MoveAssignmentTest)
{
  smirnov::List< int > list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);

  smirnov::List< int > list2;
  list2 = std::move(list1);

  BOOST_CHECK(list2.size() == 3);
  BOOST_CHECK(list2.front() == 1);
  BOOST_CHECK(list2.back() == 3);
  BOOST_CHECK(list1.empty());
}

BOOST_AUTO_TEST_CASE(IteratorTest)
{
  smirnov::List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  auto it = list.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(it == list.end());
}


