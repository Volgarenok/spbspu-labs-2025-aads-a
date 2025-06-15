#include <boost/test/unit_test.hpp>

#include <tree/tree.hpp>

namespace {

  using TestTreeType = zholobov::Tree< int, std::string >;

  TestTreeType createTestTree()
  {
    TestTreeType tree;
    for (int i = 0; i < 10; ++i) {
      tree[i] = std::to_string(i);
    }
    return tree;
  }

  struct KeySeq {
    void operator()(const TestTreeType::value_type& val)
    {
      seq += (" " + val.second);
    }
    std::string seq;
  };

}

BOOST_AUTO_TEST_SUITE(Tree_Traversal)

BOOST_AUTO_TEST_CASE(traversal_lnr)
{
  {
    auto tree = createTestTree();
    KeySeq func;
    func = tree.traverse_lnr(func);
    BOOST_TEST(func.seq == " 0 1 2 3 4 5 6 7 8 9");
  }
  {
    const auto tree = createTestTree();
    KeySeq func;
    func = tree.traverse_lnr(func);
    BOOST_TEST(func.seq == " 0 1 2 3 4 5 6 7 8 9");
  }
}

BOOST_AUTO_TEST_CASE(traversal_rnl)
{
  {
    auto tree = createTestTree();
    KeySeq func;
    func = tree.traverse_rnl(func);
    BOOST_TEST(func.seq == " 9 8 7 6 5 4 3 2 1 0");
  }
  {
    const auto tree = createTestTree();
    KeySeq func;
    func = tree.traverse_rnl(func);
    BOOST_TEST(func.seq == " 9 8 7 6 5 4 3 2 1 0");
  }
}

BOOST_AUTO_TEST_CASE(traversal_breadth)
{
  {
    auto tree = createTestTree();
    KeySeq func;
    func = tree.traverse_breadth(func);
    BOOST_TEST(func.seq == " 3 1 7 0 2 5 8 4 6 9");
  }
  {
    const auto tree = createTestTree();
    KeySeq func;
    func = tree.traverse_breadth(func);
    BOOST_TEST(func.seq == " 3 1 7 0 2 5 8 4 6 9");
  }
}

BOOST_AUTO_TEST_SUITE_END()
