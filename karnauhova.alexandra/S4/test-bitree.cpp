#include <boost/test/unit_test.hpp>
#include "bitree.hpp"

BOOST_AUTO_TEST_CASE(test_element_access)
{
  karnauhova::BiTree< size_t, int > tree;
  BOOST_TEST(tree.empty() == 1);
  tree.push(1, 1);
  BOOST_TEST(tree.empty() == 0);
}
