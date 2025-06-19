#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "FwdList.hpp"

BOOST_AUTO_TEST_SUITE(iterator_tests)

BOOST_AUTO_TEST_CASE(check_begin)
{
  shramko::ForwardList< int > myList;
  myList.addToFront(5);
  myList.addToFront(10);
  auto it = myList.begin();
  BOOST_TEST(*it == 10);
}

BOOST_AUTO_TEST_CASE(check_end)
{
  shramko::ForwardList< int > myList;
  myList.addToFront(1);
  myList.addToFront(2);
  auto it = myList.end();
  BOOST_TEST(*it == 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(front_tests)

BOOST_AUTO_TEST_CASE(empty_front)
{
  shramko::ForwardList< int > myList;
  BOOST_CHECK_THROW(myList.getFront(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(normal_front)
{
  shramko::ForwardList< int > myList;
  myList.addToFront(99);
  BOOST_TEST(myList.getFront() == 99);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(back_tests)

BOOST_AUTO_TEST_CASE(empty_back)
{
  shramko::ForwardList< int > myList;
  BOOST_CHECK_THROW(myList.getBack(), std::logic_error);
}

BOOST_AUTO_TEST_CASE(normal_back)
{
  shramko::ForwardList< int > myList;
  myList.addToFront(1);
  myList.addToFront(2);
  BOOST_TEST(myList.getBack() == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(add_front_test)
{
  shramko::ForwardList< int > myList;
  myList.addToFront(42);
  myList.addToFront(24);
  BOOST_TEST(myList.getSize() == 2);
  BOOST_TEST(myList.getFront() == 24);
}

BOOST_AUTO_TEST_CASE(remove_front_test)
{
  shramko::ForwardList< int > myList;
  myList.addToFront(1);
  myList.addToFront(2);
  myList.removeFront();
  BOOST_TEST(myList.getSize() == 1);
  myList.removeFront();
  BOOST_TEST(myList.isEmpty());
}

BOOST_AUTO_TEST_CASE(swap_test)
{
  shramko::ForwardList< int > listA;
  shramko::ForwardList< int > listB;
  listA.addToFront(100);
  listB.addToFront(200);
  listB.addToFront(300);
  listA.swapLists(listB);
  BOOST_TEST(listA.getFront() == 300);
  BOOST_TEST(listB.getFront() == 100);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  shramko::ForwardList< double > myList;
  myList.addToFront(3.14);
  myList.clearAll();
  BOOST_TEST(myList.isEmpty());
}
