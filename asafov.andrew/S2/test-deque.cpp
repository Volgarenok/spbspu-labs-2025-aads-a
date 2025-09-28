#include <boost/test/unit_test.hpp>
#include "deque.hpp"

BOOST_AUTO_TEST_CASE(push_back_and_size_test)
{
  asafov::Deque< size_t > deque;
  deque.push_back(1);
  deque.push_back(2);
  deque.push_back(3);
  BOOST_TEST(deque.size() == 3);
  BOOST_CHECK_EQUAL(deque.back(), 3);
}

BOOST_AUTO_TEST_CASE(push_front_test)
{
  asafov::Deque< size_t > deque;
  deque.push_front(1);
  deque.push_front(2);
  deque.push_front(3);
  BOOST_TEST(deque.size() == 3);
  BOOST_CHECK_EQUAL(deque.front(), 3);
}

BOOST_AUTO_TEST_CASE(front_and_back_test)
{
  asafov::Deque< size_t > deque;
  deque.push_back(10);
  deque.push_front(5);
  BOOST_CHECK_EQUAL(deque.front(), 5);
  BOOST_CHECK_EQUAL(deque.back(), 10);
}

BOOST_AUTO_TEST_CASE(pop_back_test)
{
  asafov::Deque< size_t > deque;
  deque.push_back(1);
  deque.push_back(2);
  deque.pop_back();
  BOOST_TEST(deque.size() == 1);
  BOOST_CHECK_EQUAL(deque.back(), 1);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  asafov::Deque< size_t > deque;
  deque.push_front(1);
  deque.push_front(2);
  deque.pop_front();
  BOOST_TEST(deque.size() == 1);
  BOOST_CHECK_EQUAL(deque.front(), 1);
}

BOOST_AUTO_TEST_CASE(empty_test)
{
  asafov::Deque< size_t > deque;
  BOOST_TEST(deque.empty());
}

BOOST_AUTO_TEST_CASE(nonempty_test)
{
  asafov::Deque< size_t > deque;
  deque.push_back(42);
  BOOST_TEST(!deque.empty());
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  asafov::Deque< size_t > deque;
  deque.push_back(1);
  deque.push_back(2);
  deque.clear();
  BOOST_TEST(deque.size() == 0);
  BOOST_TEST(deque.empty());
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  asafov::Deque< size_t > deque;
  deque.push_back(1);
  deque.push_back(2);
  asafov::Deque< size_t > copy(deque);
  BOOST_TEST(copy.size() == 2);
  BOOST_CHECK_EQUAL(copy.front(), 1);
  BOOST_CHECK_EQUAL(copy.back(), 2);
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  asafov::Deque< size_t > deque;
  deque.push_back(1);
  deque.push_back(2);
  asafov::Deque< size_t > moved(std::move(deque));
  BOOST_TEST(moved.size() == 2);
  BOOST_TEST(deque.empty());
}

BOOST_AUTO_TEST_CASE(assignment_operator_lvalue_test)
{
  asafov::Deque< size_t > deque;
  deque.push_back(1);
  deque.push_back(2);
  asafov::Deque< size_t > assigned;
  assigned = deque;
  BOOST_TEST(assigned.size() == 2);
  BOOST_CHECK_EQUAL(assigned.front(), 1);
  BOOST_CHECK_EQUAL(assigned.back(), 2);
}

BOOST_AUTO_TEST_CASE(assignment_operator_rvalue_test)
{
  auto createDeque = []() -> asafov::Deque< size_t >
  {
    asafov::Deque< size_t > temp;
    temp.push_back(1);
    temp.push_back(2);
    return temp;
  };

  asafov::Deque< size_t > assigned = createDeque();
  BOOST_TEST(assigned.size() == 2);
  BOOST_CHECK_EQUAL(assigned.front(), 1);
  BOOST_CHECK_EQUAL(assigned.back(), 2);
}

BOOST_AUTO_TEST_CASE(exception_on_empty_front_back_test)
{
  asafov::Deque< size_t > deque;
  BOOST_CHECK_THROW(deque.front(), std::out_of_range);
  BOOST_CHECK_THROW(deque.back(), std::out_of_range);
  BOOST_CHECK_THROW(deque.pop_front(), std::out_of_range);
  BOOST_CHECK_THROW(deque.pop_back(), std::out_of_range);
}
