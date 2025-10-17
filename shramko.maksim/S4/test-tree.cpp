#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <UBST/UBST.hpp>

using namespace shramko;

BOOST_AUTO_TEST_CASE(EmptyTreeTest)
{
  UBstTree< int, int > tree;
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
  BOOST_TEST(tree.cbegin() == tree.cend());
}

BOOST_AUTO_TEST_CASE(InsertAndFindTest)
{
  UBstTree< int, std::string > tree;
  tree[10] = "ten";
  tree[5] = "five";
  tree[15] = "fifteen";

  BOOST_TEST(!tree.empty());
  BOOST_TEST(tree.size() == 3);

  BOOST_TEST(tree.at(10) == "ten");
  BOOST_TEST(tree.at(5) == "five");
  BOOST_TEST(tree.at(15) == "fifteen");

  BOOST_CHECK_THROW(tree.at(20), std::out_of_range);

  auto it = tree.find(5);
  BOOST_TEST(it != tree.cend());
  BOOST_TEST(it->second == "five");

  auto it_notfound = tree.find(100);
  BOOST_TEST(it_notfound == tree.cend());
}

BOOST_AUTO_TEST_CASE(IteratorTraversalTest)
{
  UBstTree< int, int > tree;
  for (int i = 20; i >= 0; --i)
  {
    tree[i] = i * 10;
  }

  int expected_key = 0;
  for (auto it = tree.cbegin(); it != tree.cend(); ++it)
  {
    BOOST_TEST(it->first == expected_key);
    BOOST_TEST(it->second == expected_key * 10);
    ++expected_key;
  }
  BOOST_TEST(expected_key == 21);
}

BOOST_AUTO_TEST_CASE(UpdateValueTest)
{
  UBstTree< int, std::string > tree;
  tree[1] = "one";
  BOOST_TEST(tree[1] == "one");

  tree[1] = "uno";
  BOOST_TEST(tree[1] == "uno");
  BOOST_TEST(tree.size() == 1);
}

BOOST_AUTO_TEST_CASE(CopyConstructorAndAssignmentTest)
{
  UBstTree< int, std::string > tree1;
  tree1[1] = "one";

  UBstTree< int, std::string > tree2(tree1);
  BOOST_TEST(tree2.size() == 1);
  BOOST_TEST(tree2.find(1) != tree2.cend());

  UBstTree< int, std::string > tree3;
  tree3 = tree1;
  BOOST_TEST(tree3.size() == 1);
  BOOST_TEST(tree3.find(1) != tree3.cend());
}

BOOST_AUTO_TEST_CASE(MoveConstructorAndAssignmentTest)
{
  UBstTree< int, std::string > tree1;
  tree1[1] = "one";

  UBstTree< int, std::string > tree2(std::move(tree1));
  BOOST_TEST(tree2.size() == 1);
  BOOST_TEST(tree1.empty());

  UBstTree< int, std::string > tree3;
  tree3 = std::move(tree2);
  BOOST_TEST(tree3.size() == 1);
  BOOST_TEST(tree2.empty());
}

BOOST_AUTO_TEST_CASE(ClearTest)
{
  UBstTree< int, std::string > tree;
  tree[1] = "val1";
  tree[2] = "val2";
  tree[3] = "val3";
  BOOST_TEST(tree.size() == 3);

  tree.clear();

  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
  BOOST_TEST(tree.cbegin() == tree.cend());
}

namespace boost::test_tools::tt_detail
{
  template< typename Key, typename Value, typename Compare >
  struct print_log_value< shramko::ConstIterator< Key, Value, Compare > >
  {
    void operator()(std::ostream& os, shramko::ConstIterator< Key, Value, Compare > const& it)
    {
      if (it == shramko::ConstIterator< Key, Value, Compare >())
      {
        os << "end iterator";
      }
      else
      {
        os << "iterator to [" << it->first << ", " << it->second << "]";
      }
    }
  };
}
