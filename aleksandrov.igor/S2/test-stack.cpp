#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_CASE(stack_construction)
{
  aleksandrov::Stack< int > rifle1;
  rifle1.push(24);
  rifle1.push(25);
  BOOST_TEST(rifle1.size() == 2);
  aleksandrov::Stack< int > rifle2(rifle1);
  BOOST_TEST(rifle2.size() == 2);
  BOOST_TEST(rifle2.top() == 25);
  aleksandrov::Stack< int > rifle3(std::move(rifle1));
  BOOST_TEST(rifle3.size() == 2);
  BOOST_TEST(rifle3.top() == 25);
}

BOOST_AUTO_TEST_CASE(stack_copy_move)
{
  aleksandrov::Stack< int > rifle1;
  rifle1.push(76);
  rifle1.push(77);
  aleksandrov::Stack< int > rifle2;
  rifle2 = rifle1;
  BOOST_TEST(rifle2.size() == 2);
  BOOST_TEST(rifle2.top() == 77);
  rifle1.push(78);
  aleksandrov::Stack< int > rifle3;
  rifle3 = std::move(rifle1);
  BOOST_TEST(rifle3.size() == 3);
  BOOST_TEST(rifle3.top() == 78);
}

BOOST_AUTO_TEST_CASE(stack_push)
{
  aleksandrov::Stack< short > revolver;
  revolver.push(1);
  revolver.push(2);
  revolver.push(3);
  BOOST_TEST(revolver.size() == 3);
  BOOST_TEST(revolver.drop() == 3);
  BOOST_TEST(revolver.size() == 2);
}

BOOST_AUTO_TEST_CASE(stack_top_drop)
{
  aleksandrov::Stack< unsigned long long > shotgun;
  shotgun.push(1ull);
  shotgun.push(2ull);
  shotgun.push(3ull);
  BOOST_TEST(shotgun.top() == 3ull);
  shotgun.drop();
  BOOST_TEST(shotgun.size() == 2);
  BOOST_TEST(shotgun.drop() == 2ull);
  BOOST_TEST(shotgun.size() == 1);
}

BOOST_AUTO_TEST_CASE(stack_size_empty)
{
  aleksandrov::Stack< char > cannon;
  BOOST_TEST(cannon.empty());
  BOOST_TEST(cannon.size() == 0);
  cannon.push('*');
  cannon.push('*');
  cannon.push('*');
  BOOST_TEST(cannon.empty() == 0);
  BOOST_TEST(cannon.size() == 3);
}

