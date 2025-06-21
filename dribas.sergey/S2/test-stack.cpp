#include <boost/test/unit_test.hpp>
#include <stack.hpp>

BOOST_AUTO_TEST_SUITE(StackTests)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  dribas::Stack< int > stack;
  BOOST_CHECK(stack.size() == 0);
}

BOOST_AUTO_TEST_CASE(PushAndTop)
{
  dribas::Stack< int > stack;
  stack.push(10);
  BOOST_CHECK(stack.size() == 1);
  BOOST_CHECK(stack.top() == 10);

  stack.push(20);
  BOOST_CHECK(stack.size() == 2);
  BOOST_CHECK(stack.top() == 20);
}

BOOST_AUTO_TEST_CASE(Pop)
{
  dribas::Stack< int > stack;
  stack.push(10);
  stack.push(20);
  stack.pop();
  BOOST_CHECK(stack.size() == 1);
  BOOST_CHECK(stack.top() == 10);
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  dribas::Stack< int > stack1;
  stack1.push(10);
  dribas::Stack< int > stack2(stack1);
  BOOST_CHECK(stack2.size() == 1);
  BOOST_CHECK(stack2.top() == 10);
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
  dribas::Stack< int > stack1;
  stack1.push(10);
  dribas::Stack< int > stack2;
  stack2 = stack1;
  BOOST_CHECK(stack2.size() == 1);
  BOOST_CHECK(stack2.top() == 10);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  dribas::Stack< int > stack1;
  stack1.push(10);
  dribas::Stack< int > stack2(std::move(stack1));
  BOOST_CHECK(stack2.size() == 1);
  BOOST_CHECK(stack2.top() == 10);
  BOOST_CHECK(stack1.size() == 0);
}

BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  dribas::Stack< int > stack1;
  stack1.push(10);
  dribas::Stack< int > stack2;
  stack2 = std::move(stack1);
  BOOST_CHECK(stack2.size() == 1);
  BOOST_CHECK(stack2.top() == 10);
  BOOST_CHECK(stack1.size() == 0);
}

BOOST_AUTO_TEST_CASE(Swap)
{
  dribas::Stack< int > stack1;
  stack1.push(10);
  stack1.push(20);
  dribas::Stack< int > stack2;
  stack2.push(30);
  stack1.swap(stack2);
  BOOST_CHECK(stack1.size() == 1);
  BOOST_CHECK(stack1.top() == 30);
  BOOST_CHECK(stack2.size() == 2);
  BOOST_CHECK(stack2.top() == 20);
}

BOOST_AUTO_TEST_SUITE_END()
