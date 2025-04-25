#include <boost/test/unit_test.hpp>
#include <sstream>
#include "../common/stack.hpp"

namespace
{
  template< class T >
  std::stringstream & out_stack(evstyunichev::Stack< T > &s, std::stringstream &ss)
  {
    ss << s.top();
    while (s.size() > 1)
    {
      s.pop();
      ss << ' ' << s.top();
    }
    return ss;
  }
}

BOOST_AUTO_TEST_SUITE(stack_tests)

BOOST_AUTO_TEST_CASE(construct_test)
{
  evstyunichev::Stack< int > s1{};
}

BOOST_AUTO_TEST_CASE(push_pop_tests)
{
  evstyunichev::Stack< int > s1{};
  s1.push(34);
  s1.push(52);
  s1.pop();
}

BOOST_AUTO_TEST_CASE(top_test)
{
  std::stringstream ss;
  evstyunichev::Stack< int > s1{};
  for (int i = 1; i <= 6; i++)
  {
    s1.push(i * 2 + 1);
    s1.push(i * 2);
    s1.pop();
  }
  BOOST_CHECK(s1.top() == 13);
  s1.push(52);
  BOOST_CHECK(s1.size() == 7);
  ss << s1.top();
  while (s1.size() > 1)
  {
    s1.pop();
    ss << ' ' << s1.top();
  }
  BOOST_CHECK(ss.str() == "52 13 11 9 7 5 3");
}

BOOST_AUTO_TEST_CASE(advanced_construct_test)
{
  std::stringstream ss;
  evstyunichev::Stack< int > s1{};
  s1.push(10);
  evstyunichev::Stack< int > s2{ s1 };
  s2.push(11);
  out_stack(s2, ss);
  BOOST_CHECK(ss.str() == "11 10");
}

BOOST_AUTO_TEST_CASE(advanced_assign_test)
{
  std::stringstream ss;
  evstyunichev::Stack< int > s1{}, s2{};
  s2.push(52);
  for (size_t i = 0; i < 6; i++)
  {
    s1.push(i);
  }
  s2 = s1;
  s1.push(52);
  s2.push(11);
  out_stack(s2, ss);
  BOOST_CHECK(ss.str() == "11 5 4 3 2 1 0");
}

BOOST_AUTO_TEST_CASE(stack_in_stack)
{
  using my_stack_i = evstyunichev::Stack< int >;
  using st_of_st = evstyunichev::Stack< my_stack_i >;
  std::stringstream ss;
  st_of_st s1{}, s4{};
  my_stack_i s2{};
  s2.push(52);
  s1.push(s2);
  s2.push(10);
  BOOST_CHECK(s1.top().top() == 52);
  s4.push(my_stack_i{});
}

BOOST_AUTO_TEST_CASE(advanced_stack_in_stack)
{
  using my_stack_i = evstyunichev::Stack< int >;
  using st_of_st = evstyunichev::Stack< my_stack_i >;
  std::stringstream ss;
  st_of_st s1{};
  my_stack_i s2{}, s3{};
  s2.push(3);
  s3 = s2;
  s2.push(10);
  s1.push({});
  s1.top().push(11);
  s1.push(s2);
  s1.push(s3);
  out_stack(s1.top(), ss);
  while (s1.size() > 1)
  {
    s1.pop();
    ss << ' ';
    out_stack(s1.top(), ss);
  }
  BOOST_CHECK(ss.str() == "3 10 3 11");
}

BOOST_AUTO_TEST_SUITE_END()
