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

BOOST_AUTO_TEST_SUITE(special_methods_tests)

BOOST_AUTO_TEST_CASE(default_constructor)
{
  shramko::ForwardList< int > myList;
  BOOST_TEST(myList.isEmpty());
  BOOST_TEST(myList.getSize() == 0);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  shramko::ForwardList< int > original;
  original.addToBack(1);
  original.addToBack(2);
  original.addToBack(3);

  shramko::ForwardList< int > copy(original);
  BOOST_TEST(copy.getSize() == original.getSize());
  BOOST_TEST(copy.getFront() == original.getFront());
  BOOST_TEST(copy.getBack() == original.getBack());

  auto orig_it = original.begin();
  auto copy_it = copy.begin();
  for (; orig_it != original.end() && copy_it != copy.end(); ++orig_it, ++copy_it)
  {
    BOOST_TEST(*orig_it == *copy_it);
  }
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  shramko::ForwardList< int > original;
  original.addToBack(1);
  original.addToBack(2);
  original.addToBack(3);

  size_t size_before = original.getSize();
  shramko::ForwardList< int > moved(std::move(original));

  BOOST_TEST(moved.getSize() == size_before);
  BOOST_TEST(original.isEmpty());
  BOOST_TEST(original.getSize() == 0);
  BOOST_TEST(moved.getFront() == 1);
  BOOST_TEST(moved.getBack() == 3);
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  shramko::ForwardList< int > original;
  original.addToBack(1);
  original.addToBack(2);

  shramko::ForwardList< int > copy;
  copy.addToBack(10);
  copy = original;

  BOOST_TEST(copy.getSize() == original.getSize());
  BOOST_TEST(copy.getFront() == original.getFront());
  BOOST_TEST(copy.getBack() == original.getBack());

  auto orig_it = original.begin();
  auto copy_it = copy.begin();
  for (; orig_it != original.end() && copy_it != copy.end(); ++orig_it, ++copy_it)
  {
    BOOST_TEST(*orig_it == *copy_it);
  }
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  shramko::ForwardList< int > original;
  original.addToBack(1);
  original.addToBack(2);

  shramko::ForwardList< int > moved;
  moved.addToBack(10);
  size_t size_before = original.getSize();
  moved = std::move(original);

  BOOST_TEST(moved.getSize() == size_before);
  BOOST_TEST(original.isEmpty());
  BOOST_TEST(original.getSize() == 0);
  BOOST_TEST(moved.getFront() == 1);
  BOOST_TEST(moved.getBack() == 2);
}

BOOST_AUTO_TEST_CASE(destructor)
{
  shramko::ForwardList< int >* ptr = new shramko::ForwardList< int >;
  ptr->addToBack(1);
  ptr->addToBack(2);
  delete ptr;
  BOOST_TEST(true);
}

BOOST_AUTO_TEST_SUITE_END()
