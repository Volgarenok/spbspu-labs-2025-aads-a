#include <boost/test/unit_test.hpp>
#include "tree.hpp"

using namespace kushekbaev;
using dataset_t = Tree< size_t, std::string >;

namespace
{
  std::string printTree(const dataset_t& tree)
  {
    if (tree.empty())
    {
      return "";
    }
    std::ostringstream out;
    for (auto it = tree.begin(); it != tree.end(); ++it)
    {
      out << it->second;
    }
    return out.str();
  }
}


BOOST_AUTO_TEST_SUITE(constructors)

BOOST_AUTO_TEST_CASE(defaultConstructor)
{
  dataset_t tree;
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
}

BOOST_AUTO_TEST_CASE(copyConstructor)
{
  dataset_t src_tree;
  src_tree.insert(std::make_pair(1, "1"));
  dataset_t copy_tree(src_tree);
  BOOST_TEST(printTree(src_tree) == printTree(copy_tree));
  src_tree.insert(std::make_pair(2, "2"));
  BOOST_TEST(printTree(src_tree) == "12");
  BOOST_TEST(printTree(copy_tree) == "1");
}

BOOST_AUTO_TEST_CASE(moveConstructor)
{
  dataset_t src_tree;
  src_tree.insert(std::make_pair(1, "1"));
  src_tree.insert(std::make_pair(2, "2"));
  auto src_tree_before = src_tree;
  dataset_t move_tree(std::move(src_tree));
  BOOST_TEST(printTree(move_tree) == "12");
  BOOST_TEST(move_tree.size() == 2);
  BOOST_TEST(src_tree.empty());
  BOOST_TEST(printTree(src_tree) == "");
  BOOST_TEST(src_tree.size() == 0);
  move_tree.insert(std::make_pair(3, "3"));
  BOOST_TEST(printTree(move_tree) == "123");
}

BOOST_AUTO_TEST_CASE(ilConstructor)
{
  dataset_t il_tree{{ 1, "1"}, { 2, "2" }};
  BOOST_TEST(printTree(il_tree) == "12");
  il_tree.insert(std::make_pair(3, "3"));
  BOOST_TEST(printTree(il_tree) == "123");
}

BOOST_AUTO_TEST_CASE(inputIterator)
{
  dataset_t src_tree;
  src_tree.insert(std::make_pair(1, "1"));
  src_tree.insert(std::make_pair(2, "2"));
  src_tree.insert(std::make_pair(3, "3"));
  src_tree.insert(std::make_pair(4, "4"));
  src_tree.insert(std::make_pair(5, "5"));
  src_tree.insert(std::make_pair(6, "6"));
  dataset_t ii_tree(src_tree.begin(), src_tree.end());
  BOOST_TEST(printTree(ii_tree) == "123456");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(operators)

BOOST_AUTO_TEST_CASE(copyAssignment)
{
  dataset_t first_tree;
  first_tree.insert(std::make_pair(1, "1"));
  first_tree.insert(std::make_pair(2, "2"));
  first_tree.insert(std::make_pair(3, "3"));
  dataset_t second_tree;
  second_tree = first_tree;
  BOOST_TEST(printTree(first_tree) == printTree(second_tree));
}

BOOST_AUTO_TEST_CASE(moveAssignment)
{
  dataset_t first_tree;
  first_tree.insert(std::make_pair(1, "1"));
  first_tree.insert(std::make_pair(2, "2"));
  first_tree.insert(std::make_pair(3, "3"));
  dataset_t second_tree;
  second_tree = std::move(first_tree);
  BOOST_TEST(printTree(first_tree) == "");
  BOOST_TEST(printTree(second_tree) == "123");
}

BOOST_AUTO_TEST_CASE(equalNotEqual)
{
  dataset_t first_tree;
  first_tree.insert(std::make_pair(1, "1"));
  first_tree.insert(std::make_pair(2, "2"));
  first_tree.insert(std::make_pair(3, "3"));
  dataset_t second_tree;
  second_tree.insert(std::make_pair(1, "1"));
  second_tree.insert(std::make_pair(2, "2"));
  second_tree.insert(std::make_pair(3, "3"));
  BOOST_TEST(first_tree == second_tree);
  first_tree.insert(std::make_pair(4, "4"));
  BOOST_TEST(first_tree != second_tree);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(beginEnd)
{
  dataset_t first_tree;
  first_tree.insert(std::make_pair(1, "1"));
  first_tree.insert(std::make_pair(2, "2"));
  first_tree.insert(std::make_pair(3, "3"));
  auto it_begin = first_tree.begin();
  auto it_end = first_tree.end();
  BOOST_TEST((*it_begin).first == 1);
  ++it_begin;
  BOOST_TEST((*it_begin).first == 2);
  ++it_begin;
  BOOST_TEST((*it_begin).first == 3);
  --it_end;
  BOOST_TEST((*it_begin).first == (*it_end).first);
  --it_begin;
  BOOST_TEST((*it_begin).first == 2);
}

BOOST_AUTO_TEST_CASE(cbeginCend)
{
  dataset_t first_tree;
  first_tree.insert(std::make_pair(1, "1"));
  first_tree.insert(std::make_pair(2, "2"));
  first_tree.insert(std::make_pair(3, "3"));
  auto it_begin = first_tree.cbegin();
  auto it_end = first_tree.cend();
  BOOST_TEST((*it_begin).first == 1);
  ++it_begin;
  BOOST_TEST((*it_begin).first == 2);
  ++it_begin;
  BOOST_TEST((*it_begin).first == 3);
  --it_end;
  BOOST_TEST((*it_begin).first == (*it_end).first);
  --it_begin;
  BOOST_TEST((*it_begin).first == 2);
}

BOOST_AUTO_TEST_CASE(sizeEmpty)
{
  dataset_t first_tree;
  BOOST_TEST(first_tree.size() == 0);
  BOOST_TEST(first_tree.empty());
  first_tree.insert(std::make_pair(1, "1"));
  first_tree.insert(std::make_pair(2, "2"));
  first_tree.insert(std::make_pair(3, "3"));
  BOOST_TEST(first_tree.size() == 3);
  BOOST_TEST(!(first_tree.empty()));
}

BOOST_AUTO_TEST_CASE(atOp)
{
  dataset_t first_tree;
  first_tree.insert(std::make_pair(1, "1"));
  first_tree.insert(std::make_pair(2, "2"));
  first_tree.insert(std::make_pair(3, "3"));
  auto tmp = first_tree.at(1);
  BOOST_TEST(tmp == "1");
  BOOST_TEST(first_tree[2] == "2");
}

BOOST_AUTO_TEST_CASE(insert)
{
  dataset_t first_tree;
  first_tree.insert({ 1, "1" });
  BOOST_TEST(printTree(first_tree) == "1");
  first_tree.insert({ 2, "2" });
  BOOST_TEST(printTree(first_tree) == "12");
  first_tree.insert(std::make_pair(3, "3"));
  BOOST_TEST(printTree(first_tree) == "123");
  dataset_t second_tree;
  second_tree.insert(first_tree.begin(), first_tree.end());
  BOOST_TEST(printTree(second_tree) == "123");
  second_tree.insert({ 10, "10" });
  auto hint = second_tree.lower_bound(4);
  auto res = second_tree.insert(hint, std::make_pair(4, "4"));
  BOOST_TEST(res->first == 4);
  BOOST_TEST(res->second == "4");
  BOOST_TEST(printTree(second_tree) == "123410");
}

BOOST_AUTO_TEST_CASE(erase)
{
  dataset_t first_tree;
  first_tree.insert(std::make_pair(1, "1"));
  first_tree.insert(std::make_pair(2, "2"));
  first_tree.insert(std::make_pair(3, "3"));
  first_tree.erase(--first_tree.end());
  BOOST_TEST(printTree(first_tree) == "12");
  first_tree.insert(std::make_pair(3, "3"));
  first_tree.insert(std::make_pair(4, "4"));
  first_tree.insert(std::make_pair(5, "5"));
  first_tree.erase(first_tree.begin(), first_tree.end());
  BOOST_TEST(first_tree.empty());
}

BOOST_AUTO_TEST_CASE(findCount)
{
  dataset_t first_tree;
  first_tree.insert(std::make_pair(1, "1"));
  first_tree.insert(std::make_pair(2, "2"));
  first_tree.insert(std::make_pair(3, "3"));
  BOOST_CHECK(first_tree.find(4) == first_tree.end());
  BOOST_CHECK(first_tree.find(1) == first_tree.begin());
  BOOST_TEST(first_tree.count(2) == 1);
}

BOOST_AUTO_TEST_CASE(equalRangeAndBounds)
{
  dataset_t first_tree;
  first_tree.insert(std::make_pair(1, "1"));
  first_tree.insert(std::make_pair(2, "2"));
  first_tree.insert(std::make_pair(3, "3"));
  first_tree.insert(std::make_pair(4, "4"));
  auto range = first_tree.equal_range(3);
  BOOST_CHECK(range.first != first_tree.end());
  BOOST_CHECK_EQUAL(range.first->first, 2);
}
