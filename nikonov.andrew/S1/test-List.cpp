#include <boost/test/unit_test.hpp>
#include <sstream>
#include "List.hpp"
BOOST_AUTO_TEST_CASE(ListStandardConstructor_test)
{
  nikonov::List< int > list;
  BOOST_TEST(list.size() == 0);
}
BOOST_AUTO_TEST_CASE(ListVariableConstructor_test)
{
  constexpr size_t size = 10;
  constexpr int data = 100;
  nikonov::List< int > list(size, data);
  BOOST_TEST(list.size() == size);
  for (auto el : list)
  {
   BOOST_TEST(el == data);
  }
}


BOOST_AUTO_TEST_CASE(ListCopyConstructor_test)
{
  constexpr size_t size = 10;
  constexpr int data = 100;
  nikonov::List< int > list(size, data);
  nikonov::List< int > copyList(list);
  BOOST_TEST(list.size() == copyList.size());
  auto it = list.begin();
  auto copyIt = copyList.begin();
  for (; it != list.end(); ++it, ++copyIt)
  {
    BOOST_TEST(*it == *copyIt);
  }
}
BOOST_AUTO_TEST_CASE(ListMoveConstructor_test)
{
  constexpr size_t size = 10;
  constexpr int data = 100;
  nikonov::List< int > list(10, 100);
  nikonov::List< int > copyList(std::move(list));
  BOOST_TEST(copyList.size() == size);
  for (auto el : copyList)
  {
    BOOST_TEST(el == data);
  }
}
BOOST_AUTO_TEST_CASE(ListInitLConstructor_test)
{
  constexpr int data1 = 10;
  constexpr int data2 = 100;
  nikonov::List< int > list{ data1, data2 };
  BOOST_TEST(list.front() == data1);
  BOOST_TEST(list.back() == data2);
}
BOOST_AUTO_TEST_CASE(ListRangeConstructor_test)
{
  using cIter = nikonov::ConstListIterator< int >;
  constexpr size_t size = 10;
  constexpr int data = 100;
  nikonov::List< int > list(size, data);
  cIter begin = list.cbegin();
  cIter end = list.cend();
  nikonov::List< int > newList(begin, end);
  cIter newBegin = newList.cbegin();
  cIter newEnd = newList.cend();
  for (; newBegin != newEnd; ++newBegin, ++begin)
  {
    BOOST_TEST(*begin == *newBegin);
  }
}
