#include <boost/test/unit_test.hpp>
#include <string>
#include <queue.hpp>

BOOST_AUTO_TEST_SUITE(special_methods_queue)
BOOST_AUTO_TEST_CASE(constructors)
{
  sveshnikov::Queue< char > queue;
  BOOST_TEST(queue.empty());

  queue.push('1');
  queue.push('2');
  sveshnikov::Queue< char > queue2(queue);
  BOOST_TEST(queue2.getSize() == 2);
  BOOST_TEST(queue2.front() == '1');

  sveshnikov::Queue< char > queue3(std::move(queue));
  BOOST_TEST(queue.empty());
  BOOST_TEST(queue3.getSize() == 2);
  BOOST_TEST(queue3.front() == '1');
}

BOOST_AUTO_TEST_CASE(operators)
{
  sveshnikov::Queue< int > queue1;
  queue1.push(1);
  queue1.push(2);
  sveshnikov::Queue< int > queue2;
  queue2 = queue1;
  BOOST_TEST(!queue1.empty());
  BOOST_TEST(queue2.getSize() == 2);
  BOOST_TEST(queue2.front() == 1);

  sveshnikov::Queue< int > queue3;
  queue3 = std::move(queue1);
  BOOST_TEST(queue1.empty());
  BOOST_TEST(queue3.getSize() == 2);
  BOOST_TEST(queue2.front() == 1);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(other_methods_queue)
BOOST_AUTO_TEST_CASE(push)
{
  sveshnikov::Queue< std::string > queue;
  queue.push("pobeda");
  BOOST_TEST(queue.front() == "pobeda");
  for (size_t i = 0; i < 100; i++)
  {
    queue.push(" URA!!!");
  }
  std::string str = "ura!";
  queue.push(str);
  BOOST_TEST(queue.getSize() == 102);
}

BOOST_AUTO_TEST_CASE(pop)
{
  sveshnikov::Queue< std::string > queue;
  queue.push("slovo1");
  queue.push("slovo2");
  queue.pop();
  BOOST_TEST(queue.front() == "slovo2");
  BOOST_TEST(queue.getSize() == 1);
  queue.pop();
  BOOST_TEST(queue.empty());
}

BOOST_AUTO_TEST_CASE(front)
{
  sveshnikov::Queue< int > queue;
  int x = 0;
  for (size_t i = 0; i < 100; i++)
  {
    queue.push(x + i);
  }
  for (size_t i = 0; i < 99; i++)
  {
    queue.pop();
  }
  BOOST_TEST(queue.front() == 99);
  x = 100;
  for (size_t i = 0; i < 110; i++)
  {
    queue.push(x + i);
  }
  for (size_t i = 0; i < 109; i++)
  {
    queue.pop();
  }
  BOOST_TEST(queue.front() == 208);
}

BOOST_AUTO_TEST_CASE(swap)
{
  sveshnikov::Queue< std::string > queue1;
  queue1.push("slovo1");
  queue1.push("slovo2");
  sveshnikov::Queue< std::string > queue2;
  queue2.push("drugoe_slovo1");
  queue1.swap(queue2);
  BOOST_TEST(queue1.getSize() == 1);
  BOOST_TEST(queue2.getSize() == 2);
  BOOST_TEST(queue1.front() == "drugoe_slovo1");
  BOOST_TEST(queue2.front() == "slovo1");
}
BOOST_AUTO_TEST_SUITE_END()
