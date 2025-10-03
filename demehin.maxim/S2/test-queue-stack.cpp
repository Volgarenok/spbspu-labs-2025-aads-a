#include <boost/test/unit_test.hpp>
#include <sstream>
#include <stack.hpp>
#include <queue.hpp>

BOOST_AUTO_TEST_CASE(stack_test)
{
  demehin::Stack< int > orig_stack;
  BOOST_TEST(orig_stack.size() == 0);
  BOOST_TEST(orig_stack.empty());

  orig_stack.push(1);
  orig_stack.push(2);
  demehin::Stack< int > cpy_stack(orig_stack);
  BOOST_TEST(cpy_stack.size() == 2);
  BOOST_TEST(!cpy_stack.empty());
  BOOST_TEST(cpy_stack.top() == 2);

  cpy_stack.pop();
  demehin::Stack< int > mv_stack(std::move(cpy_stack));
  BOOST_TEST(mv_stack.size() == 1);
  BOOST_TEST(mv_stack.top() == 1);

  orig_stack.pop();
  mv_stack = orig_stack;
  BOOST_TEST(mv_stack.size() == 1);
  BOOST_TEST(mv_stack.top() == 1);

  orig_stack.push(3);
  mv_stack = std::move(orig_stack);
  BOOST_TEST(mv_stack.size() == 2);
  BOOST_TEST(mv_stack.top() == 3);
}

BOOST_AUTO_TEST_CASE(queue_test)
{
  demehin::Queue< int > orig_queue;
  BOOST_TEST(orig_queue.empty());
  BOOST_TEST(orig_queue.size() == 0);

  orig_queue.push(1);
  orig_queue.push(2);
  orig_queue.push(3);
  demehin::Queue< int > cpy_queue(orig_queue);
  BOOST_TEST(!cpy_queue.empty());
  BOOST_TEST(cpy_queue.size() == 3);
  BOOST_TEST(cpy_queue.front() == 1);
  BOOST_TEST(cpy_queue.back() == 3);

  cpy_queue.pop();
  demehin::Queue< int > mv_queue(std::move(cpy_queue));
  BOOST_TEST(mv_queue.size() == 2);
  BOOST_TEST(mv_queue.front() == 2);

  orig_queue.push(4);
  mv_queue = orig_queue;
  BOOST_TEST(mv_queue.size() == 4);
  BOOST_TEST(mv_queue.front() == 1);
  BOOST_TEST(mv_queue.back() == 4);

  orig_queue.pop();
  mv_queue = std::move(orig_queue);
  BOOST_TEST(mv_queue.size() == 3);
  BOOST_TEST(mv_queue.front() == 2);
  BOOST_TEST(mv_queue.back() == 4);
}
