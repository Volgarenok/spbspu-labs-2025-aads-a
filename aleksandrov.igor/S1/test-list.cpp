#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_CASE(construction)
{
  aleksandrov::List< int > list1;
  aleksandrov::List< int > list2(52, '\0');
  aleksandrov::List< int > list3(list2);
  aleksandrov::List< int > list4(std::move(list2));
  aleksandrov::List< int > list5({10, 1});
  BOOST_TEST(list4.size() == 52);
  BOOST_TEST(list5.size() == 10);
}

BOOST_AUTO_TEST_CASE(assignment)
{
  aleksandrov::List< char > list1(2, 'a');
  aleksandrov::List< char > list2;
  list2 = list1;
  BOOST_TEST(list2.size() == list1.size( ));
  aleksandrov::List< char > list3;
  list3 = std::move(list2);
  BOOST_TEST(list3.size() == 2);
}

BOOST_AUTO_TEST_CASE(begin_end)
{
  aleksandrov::List< int > list;
  list.pushBack(12);
  list.pushBack(34);
  list.pushBack(56);
  BOOST_TEST(*list.begin() == 12);
}

BOOST_AUTO_TEST_CASE(front_back)
{
  aleksandrov::List< char > list;
  list.pushFront('a');
  list.pushFront('b');
  list.pushFront('c');
  BOOST_TEST(list.front() == 'c');
  BOOST_TEST(list.back() == 'a');
}

BOOST_AUTO_TEST_CASE(empty)
{
  aleksandrov::List< char > list;
  BOOST_TEST(list.empty());
  list.pushFront('a');
  BOOST_TEST(!list.empty());
}

BOOST_AUTO_TEST_CASE(size)
{
  aleksandrov::List< double > list;
  BOOST_TEST(list.size() == 0);
  list.pushFront(3.14159);
  BOOST_TEST(list.size());
  list.pushFront(2.71828);
  BOOST_TEST(list.size() == 2);
}

BOOST_AUTO_TEST_CASE(pushFront)
{
  aleksandrov::List< float > list;
  list.pushFront(1.0f);
  BOOST_TEST(list.front() == 1.0f);
  list.pushFront(3.14f);
  BOOST_TEST(list.front() == 3.14f);
}

BOOST_AUTO_TEST_CASE(pushBack)
{
  aleksandrov::List< float > list;
  list.pushBack(2.5f);
  BOOST_TEST(list.back() == 2.5f);
  list.pushBack(5.0f);
  BOOST_TEST(list.back() == 5.0f);
}

BOOST_AUTO_TEST_CASE(popFront)
{
  aleksandrov::List< int > list;
  list.pushBack(123);
  list.pushBack(456);
  list.popFront();
  BOOST_TEST(list.front() == 456);
}

BOOST_AUTO_TEST_CASE(popBack)
{
  aleksandrov::List< double > list;
  list.pushFront(1000);
  list.popBack();
  BOOST_TEST(list.empty());
  list.pushBack(1);
  list.pushBack(2);
  list.popBack();
  BOOST_TEST(list.size() == 1);
}

BOOST_AUTO_TEST_CASE(swap)
{
  aleksandrov::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  aleksandrov::List< int > list2;
  list2.pushBack(3);
  list1.swap(list2);
  BOOST_TEST(list1.front() == 3);
  BOOST_TEST(list2.back() == 2);
}

BOOST_AUTO_TEST_CASE(clear)
{
  aleksandrov::List< short > list;
  list.clear();
  list.clear();
  list.pushFront(-1);
  list.pushFront(-2);
  list.clear();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(remove_if)
{
  aleksandrov::List< char > list;
  list.pushBack('a');
  list.pushBack('a');
  list.remove('a');
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(splice)
{
  aleksandrov::List< int > list1(7, 0);
  aleksandrov::List< int > list2(3, 1);
  auto itBefore = list1.cbegin();
  auto itAfter = list1.cbegin();
  std::advance(itBefore, 2);
  list1.splice(itBefore, list2);
  std::advance(itAfter, 2);
  BOOST_TEST(list1.size() == 10);
  BOOST_TEST(*itBefore == 0);
  BOOST_TEST(*itAfter == 1);
}

BOOST_AUTO_TEST_CASE(assign)
{
  aleksandrov::List< char > list(2, 'a');
  list.assign(5, 'b');
  BOOST_TEST(list.size() == 5);
}

BOOST_AUTO_TEST_CASE(eraseAfter)
{
  aleksandrov::List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.eraseAfter(list.begin());
  BOOST_TEST(list.size() == 1);
  for (int i = 2; i < 8; ++i)
  {
    list.pushBack(i);
  }
  auto first = std::next(list.begin());
  auto last = std::next(first, 4);
  auto it = list.eraseAfter(first, last);
  BOOST_TEST(*it == 6);
  BOOST_TEST(list.size() == 5);
}

BOOST_AUTO_TEST_CASE(bool_operators)
{
  aleksandrov::List< unsigned char > list1(3, 'x');
  aleksandrov::List< unsigned char > list2(3, 'y');
  aleksandrov::List< unsigned char > list3(3, 'x');
  BOOST_TEST(list1 != list2);
  BOOST_TEST(list1 == list3);
}

