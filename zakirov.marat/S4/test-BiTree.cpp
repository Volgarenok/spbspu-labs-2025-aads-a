#include <boost/test/unit_test.hpp>
#include "BiTree.hpp"

BOOST_AUTO_TEST_CASE(default_constructor)
{
  zakirov::BiTree< int, char > test;
  BOOST_TEST(test.size() == 0);
  BOOST_TEST(test.empty() == true);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(insert)
{
  zakirov::BiTree< int, char > test;
  test.insert({5, 'a'});
  test.insert({3, 'b'});
  test.insert({8, 'l'});
  BOOST_TEST(test.size() == 3);
}
