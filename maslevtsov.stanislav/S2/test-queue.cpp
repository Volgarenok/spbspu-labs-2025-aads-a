#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(queue_constructors_tests)
BOOST_AUTO_TEST_CASE(queue_default_constructor_test)
{
  maslevtsov::Queue< int > q;
  BOOST_TEST(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_copy_constructor_test)
{
  maslevtsov::Queue< int > q1;
  q1.push(1);
  q1.push(2);
  maslevtsov::Queue< int > q2(q1);
  BOOST_TEST(q1.front() == q2.front());
  q1.pop();
  q2.pop();
  BOOST_TEST(q1.front() == q2.front());
  BOOST_TEST(q1.size() == q2.size());
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(queue_operators_tests)
BOOST_AUTO_TEST_CASE(queue_copy_assignment_operator_test)
{
  maslevtsov::Queue< int > q1;
  q1.push(1);
  q1.push(2);
  maslevtsov::Queue< int > q2;
  q2 = q1;
  BOOST_TEST(q1.front() == q2.front());
  q1.pop();
  q2.pop();
  BOOST_TEST(q1.front() == q2.front());
  BOOST_TEST(q1.size() == q2.size());
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(queue_elements_access_tests)
BOOST_AUTO_TEST_CASE(queue_front_test)
{
  maslevtsov::Queue< int > q;
  q.push(1);
  q.push(2);
  BOOST_TEST(q.front() == 1);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(queue_capacity_tests)
BOOST_AUTO_TEST_CASE(queue_empty_test)
{
  maslevtsov::Queue< int > q;
  BOOST_TEST(q.empty());
  q.push(1);
  BOOST_TEST(!q.empty());
}

BOOST_AUTO_TEST_CASE(queue_size_test)
{
  maslevtsov::Queue< int > q;
  q.push(1);
  BOOST_TEST(q.size() == 1);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(queue_modifiers_tests)
BOOST_AUTO_TEST_CASE(queue_push_test)
{
  maslevtsov::Queue< int > q;
  q.push(1);
  BOOST_TEST(q.front() == 1);
  BOOST_TEST(q.size() == 1);
  int n = 2;
  q.push(n);
  q.pop();
  BOOST_TEST(q.front() == 2);
  BOOST_TEST(q.size() == 1);
}

BOOST_AUTO_TEST_CASE(queue_pop_test)
{
  maslevtsov::Queue< int > q;
  q.push(1);
  q.push(2);
  q.pop();
  BOOST_TEST(q.size() == 1);
  BOOST_TEST(q.front() == 2);
  q.pop();
  BOOST_TEST(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_swap_test)
{
  maslevtsov::Queue< int > s1;
  s1.push(1);
  maslevtsov::Queue< int > s2;
  s2.push(2);
  s2.push(3);
  s1.swap(s2);
  BOOST_TEST(s1.front() == 2);
  s1.pop();
  BOOST_TEST(s1.front() == 3);
  BOOST_TEST(s1.size() == 1);
  BOOST_TEST(s2.front() == 1);
  BOOST_TEST(s2.size() == 1);
}
BOOST_AUTO_TEST_SUITE_END()
