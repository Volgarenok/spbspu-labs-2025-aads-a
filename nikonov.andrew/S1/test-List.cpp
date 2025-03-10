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
  //for (auto el : list)
  //{
  //  BOOST_TEST(el == data);
  //}
  auto it = list.begin();
  for (; it != list.end(); ++it)
  {
    BOOST_TEST(*it == data);
  }
}
BOOST_AUTO_TEST_CASE(ListCopyConstructor_test)
{
  nikonov::List< int > list(10, 100);
  nikonov::List< int > copyList(list);
  BOOST_TEST(list.size() == copyList.size());
  auto it = list.begin();
  auto copyIt = copyList.begin();
  for (; it != list.end(); ++it, ++copyIt)
  {
    BOOST_TEST(*it == *copyIt);
  }
}
