#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_CASE(default_constructor_queue)
{
  zakirov::Queue< int > queue_t;
  BOOST_TEST(queue_t.size() == 0);
  BOOST_TEST(queue_t.empty() == true);
}

BOOST_AUTO_TEST_CASE(copy_constructor_queue)
{
  zakirov::Queue< int > queue_o;
  queue_o.push(0);
  queue_o.push(1);
  zakirov::Queue< int > queue_t(queue_o);
  BOOST_TEST(queue_t.size() == 2);
  BOOST_TEST(queue_t.front() - queue_t.back() == -1);
  BOOST_TEST(queue_o.size() == 2);
  BOOST_TEST(queue_o.front() - queue_t.back() == -1);
}

BOOST_AUTO_TEST_CASE(move_constructor_queue)
{
  zakirov::Queue< int > queue_o;
  queue_o.push(0);
  queue_o.push(1);
  zakirov::Queue< int > queue_t(std::move(queue_o));
  BOOST_TEST(queue_t.size() == 2);
  BOOST_TEST(queue_t.front() - queue_t.back() == -1);
  BOOST_TEST(queue_o.size() == 0);
  BOOST_TEST(queue_o.empty() == true);
}

BOOST_AUTO_TEST_CASE(copy_assignment_operator_queue)
{
  zakirov::Queue< int > queue_o;
  zakirov::Queue< int > queue_t;
  queue_o.push(0);
  queue_o.push(1);
  queue_t = queue_o;
  BOOST_TEST(queue_t.size() == 2);
  BOOST_TEST(queue_t.front() - queue_t.back() == -1);
  BOOST_TEST(queue_o.size() == 2);
  BOOST_TEST(queue_o.front() - queue_t.back() == -1);
}

BOOST_AUTO_TEST_CASE(move_assignment_operator_queue)
{
  zakirov::Queue< int > queue_o;
  zakirov::Queue< int > queue_t;
  queue_o.push(0);
  queue_o.push(1);
  queue_t = std::move(queue_o);
  BOOST_TEST(queue_t.size() == 2);
  BOOST_TEST(queue_t.front() - queue_t.back() == -1);
  BOOST_TEST(queue_o.size() == 0);
  BOOST_TEST(queue_o.empty() == true);
}

BOOST_AUTO_TEST_CASE(empty_queue)
{
  zakirov::Queue< int > queue_t;
  BOOST_TEST(queue_t.empty() == true);
  queue_t.push(0);
  BOOST_TEST(queue_t.empty() == false);
}

BOOST_AUTO_TEST_CASE(size_queue)
{
  zakirov::Queue< int > queue_t;
  BOOST_TEST(queue_t.size() == 0);
  queue_t.push(0);
  BOOST_TEST(queue_t.size() == 1);
  queue_t.push(1);
  BOOST_TEST(queue_t.size() == 2);
}

BOOST_AUTO_TEST_CASE(front_queue)
{
  zakirov::Queue< int > queue_t;
  queue_t.push(0);
  queue_t.push(1);
  BOOST_TEST(queue_t.front() == 0);
}

BOOST_AUTO_TEST_CASE(back_queue)
{
  zakirov::Queue< int > queue_t;
  queue_t.push(0);
  queue_t.push(1);
  BOOST_TEST(queue_t.back() == 1);
}

BOOST_AUTO_TEST_CASE(push_lvalue_queue)
{
  zakirov::Queue< int > queue_t;
  queue_t.push(0);
  queue_t.push(1);
  queue_t.push(2);
  BOOST_TEST(queue_t.back() - queue_t.front() == 2);
  BOOST_TEST(queue_t.size() == 3);
  BOOST_TEST(queue_t.empty() == false);
}

BOOST_AUTO_TEST_CASE(push_rvalue_queue)
{
  zakirov::Queue< zakirov::Queue< int > > queue_t;
  zakirov::Queue< int > queue_o;
  queue_o.push(0);
  queue_o.push(1);
  queue_o.push(2);
  queue_t.push(std::move(queue_o));
  BOOST_TEST(queue_t.size() == 1);
  BOOST_TEST(queue_t.empty() == false);
  BOOST_TEST(queue_o.size() == 0);
  BOOST_TEST(queue_o.empty() == true);
}

BOOST_AUTO_TEST_CASE(pop_queue)
{
  zakirov::Queue< int > queue_t;
  queue_t.push(0);
  queue_t.push(1);
  queue_t.push(2);
  BOOST_TEST(queue_t.back() - queue_t.front() == 2);
  queue_t.pop();
  BOOST_TEST(queue_t.back() - queue_t.front() == 1);
}

BOOST_AUTO_TEST_CASE(swap_queue)
{
  zakirov::Queue< int > queue_t;
  queue_t.push(0);
  queue_t.push(1);
  queue_t.push(2);
  zakirov::Queue< int > queue_o;
  queue_o.push(5);
  queue_o.push(10);
  BOOST_TEST(queue_t.back() - queue_t.front() == 2);
  BOOST_TEST(queue_t.size() == 3);
  BOOST_TEST(queue_o.back() - queue_o.front() == 5);
  BOOST_TEST(queue_o.size() == 2);
  queue_t.swap(queue_o);
  BOOST_TEST(queue_o.back() - queue_o.front() == 2);
  BOOST_TEST(queue_o.size() == 3);
  BOOST_TEST(queue_t.back() - queue_t.front() == 5);
  BOOST_TEST(queue_t.size() == 2);
}
