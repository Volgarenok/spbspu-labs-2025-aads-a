#include "list.hpp"
#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>

using sharifullina::List;

template< typename T >
std::string listToString(const List< T > & list)
{
  std::ostringstream oss;
  bool first = true;
  typename List< T >::ConstIterator it = list.begin();
  typename List< T >::ConstIterator end = list.end();
  for (; it != end; ++it)
  {
    if (!first)
    {
      oss << " ";
    }
    oss << *it;
    first = false;
  }
  return oss.str();
}

BOOST_AUTO_TEST_CASE(TestDefaultConstructor)
{
  List< int > list;
  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(TestPushBack)
{
  List< int > list;
  list.pushBack(1);
  BOOST_TEST(!list.empty());
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.back() == 1);

  list.pushBack(2);
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.back() == 2);
}

BOOST_AUTO_TEST_CASE(TestPushFront)
{
  List< int > list;
  list.pushFront(1);
  BOOST_TEST(!list.empty());
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.back() == 1);

  list.pushFront(2);
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(list.front() == 2);
  BOOST_TEST(list.back() == 1);
}

BOOST_AUTO_TEST_CASE(TestPopBack)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);

  list.popBack();
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(list.back() == 2);

  list.popBack();
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.back() == 1);

  list.popBack();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(TestPopFront)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);

  list.popFront();
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(list.front() == 2);

  list.popFront();
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() == 3);

  list.popFront();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(TestClear)
{
  List< int > list;
  for (int i = 0; i < 5; ++i)
  {
    list.pushBack(i);
  }
  BOOST_TEST(list.size() == 5);

  list.clear();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(TestFrontBack)
{
  List< int > list;
  list.pushBack(1);
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.back() == 1);

  list.pushBack(2);
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.back() == 2);

  list.pushFront(0);
  BOOST_TEST(list.front() == 0);
  BOOST_TEST(list.back() == 2);
}

BOOST_AUTO_TEST_CASE(TestIterators)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);

  auto it = list.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 3);
  ++it;
  BOOST_TEST(it == list.end());
}

BOOST_AUTO_TEST_CASE(TestCopyConstructor)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  list1.pushBack(3);

  List< int > list2(list1);
  BOOST_TEST(list2.size() == 3);
  BOOST_TEST(list2.front() == 1);
  BOOST_TEST(list2.back() == 3);
  BOOST_TEST(list1.size() == 3);
  BOOST_TEST(list1.front() == 1);
  BOOST_TEST(list1.back() == 3);
}

BOOST_AUTO_TEST_CASE(TestMoveConstructor)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  list1.pushBack(3);

  List< int > list2(std::move(list1));
  BOOST_TEST(list2.size() == 3);
  BOOST_TEST(list2.front() == 1);
  BOOST_TEST(list2.back() == 3);
  BOOST_TEST(list1.size() == 0);
}

BOOST_AUTO_TEST_CASE(TestCopyAssignment)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  list1.pushBack(3);

  List< int > list2;
  list2 = list1;
  BOOST_TEST(list2.size() == 3);
  BOOST_TEST(list2.front() == 1);
  BOOST_TEST(list2.back() == 3);
  list2 = list2;
  BOOST_TEST(list2.size() == 3);
}

BOOST_AUTO_TEST_CASE(TestMoveAssignment)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  list1.pushBack(3);

  List< int > list2;
  list2 = std::move(list1);
  BOOST_TEST(list2.size() == 3);
  BOOST_TEST(list2.front() == 1);
  BOOST_TEST(list2.back() == 3);
  BOOST_TEST(list1.size() == 0);
}

BOOST_AUTO_TEST_CASE(TestSwap)
{
  List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);

  List< int > list2;
  list2.pushBack(3);
  list2.pushBack(4);
  list2.pushBack(5);

  list1.swap(list2);
  BOOST_TEST(list1.size() == 3);
  BOOST_TEST(list2.size() == 2);
  BOOST_TEST(list1.front() == 3);
  BOOST_TEST(list2.front() == 1);
}

BOOST_AUTO_TEST_CASE(TestListOutput)
{
  List< int > list;
  list.pushBack(1);
  list.pushBack(2);
  list.pushBack(3);

  BOOST_TEST(listToString(list) == "1 2 3");
}

BOOST_AUTO_TEST_CASE(TestComplexOperations)
{
  List< int > list;
  list.pushBack(1);
  list.pushFront(0);
  list.pushBack(2);
  list.pushBack(3);
  list.popFront();
  list.popBack();

  BOOST_TEST(listToString(list) == "1 2");
  BOOST_TEST(list.size() == 2);
}
