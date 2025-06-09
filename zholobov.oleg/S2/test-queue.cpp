#include <boost/test/unit_test.hpp>
#include <sstream>

#include <queue.hpp>

template < typename T >
std::string to_string(const zholobov::Queue< T >& queue)
{
  std::stringstream ss;
  zholobov::Queue< T > temp(queue);
  if (!temp.empty()) {
    ss << temp.front();
    temp.pop();
    while (!temp.empty()) {
      ss << " " << temp.front();
      temp.pop();
    }
  }
  return ss.str();
}

BOOST_AUTO_TEST_SUITE(Queue_Constructors)

BOOST_AUTO_TEST_CASE(default_ctor)
{
  zholobov::Queue< std::string > queue;
  BOOST_TEST(queue.empty());
  BOOST_TEST(to_string(queue) == "");
}

BOOST_AUTO_TEST_CASE(copy_ctor)
{
  zholobov::Queue< std::string > queue;
  queue.push("This");
  queue.push("is");
  queue.push("content");
  zholobov::Queue< std::string > new_queue(queue);
  BOOST_TEST(to_string(queue) == "This is content");
  BOOST_TEST(to_string(new_queue) == "This is content");
}

BOOST_AUTO_TEST_CASE(move_ctor)
{
  zholobov::Queue< std::string > queue;
  queue.push("This");
  queue.push("is");
  queue.push("content");
  zholobov::Queue< std::string > new_queue(std::move(queue));
  BOOST_TEST(to_string(queue) == "");
  BOOST_TEST(to_string(new_queue) == "This is content");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Queue_Assign_Operators)

BOOST_AUTO_TEST_CASE(copy_assign_operator)
{
  zholobov::Queue< std::string > queue;
  queue.push("This");
  queue.push("is");
  queue.push("content");
  zholobov::Queue< std::string > new_queue = queue;
  BOOST_TEST(to_string(queue) == "This is content");
  BOOST_TEST(to_string(new_queue) == "This is content");
  queue.push("of");
  queue.push("a");
  queue.push("queue");
  new_queue = queue;
  BOOST_TEST(to_string(queue) == "This is content of a queue");
  BOOST_TEST(to_string(new_queue) == "This is content of a queue");
  new_queue = zholobov::Queue< std::string >();
  BOOST_TEST(to_string(new_queue) == "");
}

BOOST_AUTO_TEST_CASE(move_assign_operator)
{
  zholobov::Queue< std::string > queue;
  queue.push("This");
  queue.push("is");
  queue.push("content");
  zholobov::Queue< std::string > new_queue = std::move(queue);
  BOOST_TEST(to_string(queue) == "");
  BOOST_TEST(to_string(new_queue) == "This is content");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Queue_Opeartions)

BOOST_AUTO_TEST_CASE(push_copy)
{
  zholobov::Queue< std::string > queue;
  std::string val = "Something";
  queue.push(val);
  BOOST_TEST(to_string(queue) == "Something");
  BOOST_TEST(val == "Something");
}

BOOST_AUTO_TEST_CASE(push_move)
{
  zholobov::Queue< std::string > queue;
  std::string val = "Something";
  queue.push(std::move(val));
  BOOST_TEST(to_string(queue) == "Something");
  BOOST_TEST(val == "");
}

BOOST_AUTO_TEST_CASE(front_back_pop)
{
  zholobov::Queue< std::string > queue;
  queue.push("This");
  BOOST_TEST(queue.front() == "This");
  queue.push("is");
  BOOST_TEST(queue.front() == "This");
  queue.push("content");
  BOOST_TEST(queue.front() == "This");
  queue.pop();
  BOOST_TEST(queue.front() == "is");
  queue.pop();
  BOOST_TEST(queue.front() == "content");
}

BOOST_AUTO_TEST_CASE(empty)
{
  zholobov::Queue< std::string > queue;
  BOOST_TEST(queue.empty());
  queue.push("Something");
  BOOST_TEST(!queue.empty());
  queue.pop();
  BOOST_TEST(queue.empty());
}

BOOST_AUTO_TEST_CASE(clear)
{
  zholobov::Queue< std::string > stack;
  stack.push("One");
  BOOST_TEST(!stack.empty());
  stack.clear();
  BOOST_TEST(stack.empty());
  stack.push("Two");
  BOOST_TEST(!stack.empty());
  stack.clear();
  BOOST_TEST(stack.empty());
}

BOOST_AUTO_TEST_SUITE_END()
