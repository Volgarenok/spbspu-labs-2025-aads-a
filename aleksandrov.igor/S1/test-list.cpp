#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_CASE(construction)
{
  aleksandrov::List< int > list1;
  BOOST_TEST(list1.empty());
  aleksandrov::List< char > list2(52, '\0');
  BOOST_TEST(list2.size() == 52);
  aleksandrov::List< char > list3(list2);
  BOOST_TEST(list3.size() == 52);
  aleksandrov::List< char > list4(std::move(list2));
  BOOST_TEST(list4.size() == 52);
  aleksandrov::List< int > list5{0, 1, 2, 3, 4, 5};
  BOOST_TEST(list5.size() == 6);
  aleksandrov::List< int > list6(++list5.begin(), list5.end());
  BOOST_TEST(list6.size() == 5);
}

BOOST_AUTO_TEST_CASE(assignment)
{
  aleksandrov::List< char > list1(2, 'a');
  aleksandrov::List< char > list2;
  list2 = list1;
  BOOST_TEST(list2.size() == list1.size());
  aleksandrov::List< char > list3;
  list3 = std::move(list2);
  BOOST_TEST(list3.size() == 2);
  aleksandrov::List< char > list4;
  list4 = {'a', 'b', 'c', 'd'};
  BOOST_TEST(list4.size() == 4);
}

BOOST_AUTO_TEST_CASE(begin_end)
{
  aleksandrov::List< int > list{12, 34, 56};
  BOOST_TEST(*list.begin() == 12);
  BOOST_TEST(*list.cbegin() == 12);
}

BOOST_AUTO_TEST_CASE(front_back)
{
  aleksandrov::List< char > list;
  list.pushBack('a');
  list.pushFront('b');
  list.pushFront('c');
  BOOST_TEST(list.front() == 'c');
  BOOST_TEST(list.back() == 'a');
}

BOOST_AUTO_TEST_CASE(empty)
{
  aleksandrov::List< char > list;
  BOOST_TEST(list.empty());
  list = {'a'};
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
  float value = 1.0f;
  list.pushFront(value);
  BOOST_TEST(list.front() == 1.0f);
  list.pushFront(3.14f);
  BOOST_TEST(list.front() == 3.14f);
}

BOOST_AUTO_TEST_CASE(pushBack)
{
  aleksandrov::List< float > list;
  float value = 2.5f;
  list.pushBack(value);
  BOOST_TEST(list.back() == 2.5f);
  list.pushBack(5.0f);
  BOOST_TEST(list.back() == 5.0f);
}

BOOST_AUTO_TEST_CASE(popFront)
{
  aleksandrov::List< int > list{123, 456};
  list.popFront();
  BOOST_TEST(list.front() == 456);
}

BOOST_AUTO_TEST_CASE(popBack)
{
  aleksandrov::List< double > list{1000};
  list.popBack();
  BOOST_TEST(list.empty());
  list.pushBack(1);
  list.pushBack(2);
  list.popBack();
  BOOST_TEST(list.size() == 1);
}

BOOST_AUTO_TEST_CASE(swap)
{
  aleksandrov::List< int > list1{1, 2};
  aleksandrov::List< int > list2{3};
  list1.swap(list2);
  BOOST_TEST(list1.front() == 3);
  BOOST_TEST(list2.back() == 2);
}

BOOST_AUTO_TEST_CASE(clear)
{
  aleksandrov::List< short > list;
  list.clear();
  list.clear();
  list = {-1, -2};
  list.clear();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(remove_if)
{
  aleksandrov::List< char > list(2, 'a');
  list.remove('a');
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(splice)
{
  aleksandrov::List< int > list1{1, 2, 3, 4, 5};
  aleksandrov::List< int > list2{10, 20, 30, 40, 50};
  auto it = list1.cbegin();
  std::advance(it, 2);
  list1.splice(it, std::move(list2));
  BOOST_TEST(list2.empty());
  BOOST_TEST(list1.size() == 10);
  list2.splice(list2.cbegin(), list1, it, list1.cend());
  BOOST_TEST(list1.size() == 7);
  BOOST_TEST(list2.size() == 3);
}

BOOST_AUTO_TEST_CASE(assign)
{
  aleksandrov::List< char > list1(2, 'a');
  list1.assign(5, 'b');
  BOOST_TEST(list1.size() == 5);

  aleksandrov::List< char > list2(3, 'c');
  list2.assign(list1.begin(), list1.end());
  BOOST_TEST(list2.size() == 5);

  aleksandrov::List< char > list3(1, 'd');
  list3.assign({'e', 'f', 'g'});
  BOOST_TEST(list3.size() == 3);
}

BOOST_AUTO_TEST_CASE(insert)
{
  aleksandrov::List< double > list{2.0, 3.0, 5.0};
  double value = 4.0;
  auto it = list.insertAfter(++list.cbegin(), value);
  BOOST_TEST(*it == 4.0);
  list.insertAfter(list.cend(), 1.0);
  BOOST_TEST(list.front() == 1.0);
}

BOOST_AUTO_TEST_CASE(erase)
{
  aleksandrov::List< int > list{1, 2};
  list.eraseAfter(list.cbegin());
  BOOST_TEST(list.size() == 1);
  for (int i = 2; i < 8; ++i)
  {
    list.pushBack(i);
  }
  auto first = std::next(list.cbegin());
  auto last = std::next(first, 4);
  auto it = list.eraseAfter(first, last);
  BOOST_TEST(*it == 6);
  BOOST_TEST(list.size() == 5);
}

BOOST_AUTO_TEST_CASE(bool_operators)
{
  aleksandrov::List< unsigned char > list1{'x', 'y', 'z'};
  aleksandrov::List< unsigned char > list2{'a', 'b', 'c'};
  aleksandrov::List< unsigned char > list3{'x', 'y', 'z'};
  BOOST_TEST(list1 != list2);
  BOOST_TEST(list1 == list3);
  BOOST_TEST(list2 < list1);
  BOOST_TEST(list2 <= list1);
  BOOST_TEST(list3 > list2);
  BOOST_TEST(list3 >= list1);
}

BOOST_AUTO_TEST_CASE(reverse)
{
  aleksandrov::List< float > list{1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
  list.reverse();
  BOOST_TEST(list.front() == 5.0f);
}

