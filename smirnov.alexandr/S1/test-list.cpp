#include <boost/test/unit_test.hpp>
#include <list/list.hpp>

BOOST_AUTO_TEST_CASE(DefaultConstructorTest)
{
  smirnov::List< int > list;
  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(PushFrontTest)
{
  smirnov::List< int > list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);

  BOOST_TEST(list.size() == 3);
  BOOST_TEST(list.front() == 3);
  BOOST_TEST(list.back() == 1);
}

BOOST_AUTO_TEST_CASE(PopFrontTest)
{
  smirnov::List<int> list;
  list.push_front(1);
  list.push_front(2);

  list.pop_front();
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.size() == 1);

  list.pop_front();
  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(CopyConstructorTest)
{
  smirnov::List<int> list1;
  list1.push_front(3);
  list1.push_front(2);
  list1.push_front(1);
  smirnov::List< int > list2(list1);

  BOOST_TEST(list2.size() == 3);
  BOOST_TEST(list2.front() == 1);
  BOOST_TEST(list2.back() == 3);
}

BOOST_AUTO_TEST_CASE(MoveConstructorTest)
{
  smirnov::List< int > list1;
  list1.push_front(3);
  list1.push_front(2);
  list1.push_front(1);

  smirnov::List< int > list2(std::move(list1));

  BOOST_TEST(list2.size() == 3);
  BOOST_TEST(list2.front() == 1);
  BOOST_TEST(list2.back() == 3);
  BOOST_TEST(list1.empty());
}

BOOST_AUTO_TEST_CASE(CopyAssignmentTest)
{
  smirnov::List< int > list1;
  list1.push_front(3);
  list1.push_front(2);
  list1.push_front(1);

  smirnov::List< int > list2;
  list2 = list1;

  BOOST_TEST(list2.size() == 3);
  BOOST_TEST(list2.front() == 1);
  BOOST_TEST(list2.back() == 3);
}

BOOST_AUTO_TEST_CASE(MoveAssignmentTest)
{
  smirnov::List< int > list1;
  list1.push_front(1);
  list1.push_front(2);
  list1.push_front(3);

  smirnov::List< int > list2;
  list2 = std::move(list1);

  BOOST_TEST(list2.size() == 3);
  BOOST_TEST(list2.front() == 3);
  BOOST_TEST(list2.back() == 1);
  BOOST_TEST(list1.empty());
}

BOOST_AUTO_TEST_CASE(SwapTest)
{
  smirnov::List< int > list1;
  list1.push_front(1);
  list1.push_front(2);

  smirnov::List< int > list2;
  list2.push_front(3);

  list1.swap(list2);

  BOOST_TEST(list1.size() == 1);
  BOOST_TEST(list1.front() == 3);
  BOOST_TEST(list2.size() == 2);
  BOOST_TEST(list2.front() == 2);
  BOOST_TEST(list2.back() == 1);
}

BOOST_AUTO_TEST_CASE(IteratorTest)
{
  smirnov::List< int > list;
  list.push_front(3);
  list.push_front(2);
  list.push_front(1);

  auto it = list.begin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(ConstIteratorTest)
{
  smirnov::List< int > list;
  list.push_front(3);
  list.push_front(2);
  list.push_front(1);

  auto it = list.cbegin();
  BOOST_CHECK(*it == 1);
  ++it;
  BOOST_CHECK(*it == 2);
  ++it;
  BOOST_CHECK(*it == 3);
  ++it;
  BOOST_CHECK(it == list.cend());
}

