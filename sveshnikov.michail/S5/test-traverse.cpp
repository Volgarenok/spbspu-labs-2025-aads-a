#include <boost/test/unit_test.hpp>
#include <tree/tree.hpp>
#include "kv-aggregator.hpp"

namespace
{
  sveshnikov::AvlTree< int, std::string > fillTree();

  sveshnikov::AvlTree< int, std::string > fillTree()
  {
    using namespace sveshnikov;
    AvlTree< int, std::string > data;
    data[4] = "4";
    data[2] = "2";
    data[6] = "6";
    data[1] = "1";
    data[3] = "3";
    data[5] = "5";
    data[7] = "7";
    return data;
  }
}

BOOST_AUTO_TEST_CASE(traverse_lnr)
{
  using namespace sveshnikov;
  AvlTree< int, std::string > data;
  KeyValAggr f;
  BOOST_CHECK_THROW(data.traverse_lnr(f), std::logic_error);

  data = fillTree();
  f = data.traverse_lnr(f);
  BOOST_TEST(f.key_sum_ == 28);
  BOOST_TEST(f.values_ == "1 2 3 4 5 6 7");
}

BOOST_AUTO_TEST_CASE(traverse_rnl)
{
  using namespace sveshnikov;
  AvlTree< int, std::string > data;
  KeyValAggr f;
  BOOST_CHECK_THROW(data.traverse_rnl(f), std::logic_error);

  data = fillTree();
  f = data.traverse_rnl(f);
  BOOST_TEST(f.key_sum_ == 28);
  BOOST_TEST(f.values_ == "7 6 5 4 3 2 1");
}

BOOST_AUTO_TEST_CASE(traverse_breadth)
{
  using namespace sveshnikov;
  AvlTree< int, std::string > data;
  KeyValAggr f;
  BOOST_CHECK_THROW(data.traverse_breadth(f), std::logic_error);

  data = fillTree();
  f = data.traverse_breadth(f);
  BOOST_TEST(f.key_sum_ == 28);
  BOOST_TEST(f.values_ == "4 2 6 1 3 5 7");
}
