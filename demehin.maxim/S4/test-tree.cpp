#include <boost/test/unit_test.hpp>
#include <sstream>
#include <tree/tree.hpp>

namespace
{
  void printTreeValues(std::ostream& out, demehin::Tree< size_t, std::string > tree)
  {
    for (auto it = tree.begin(); it != tree.end(); it++)
    {
      out << (*it).second;
    }
  }
}

BOOST_AUTO_TEST_CASE(extra_test)
{
  demehin::Tree< int, std::string > tree;

  tree[1] = "1";
  tree.insert(std::make_pair(2, "2"));
  BOOST_TEST(tree[1] == "1");
  BOOST_TEST(tree[2] == "2");

  tree[2] = "4";
  BOOST_TEST(tree[2] == "4");
}


BOOST_AUTO_TEST_CASE(tree_print_test)
{
  demehin::Tree< size_t, std::string > t;
  demehin::Tree< size_t, std::string > tree(std::move(t));
  tree.insert(std::make_pair(1, "1"));
  tree.insert(std::make_pair(3, "4"));
  tree.insert(std::make_pair(10, "11"));
  tree.insert(std::make_pair(2, "3"));
  tree.insert(std::make_pair(4, "5"));

  std::ostringstream out;
  demehin::Tree< size_t, std::string > cpy_tree(tree);
  for (auto it = --cpy_tree.end(); it != cpy_tree.begin(); --it)
  {
    out << (*it).second;
  }
  BOOST_TEST(out.str() == "11543");

  std::ostringstream out2;
  printTreeValues(out2, tree);
  BOOST_TEST(out2.str() == "134511");
}


BOOST_AUTO_TEST_CASE(tree_construct_test)
{
  demehin::Tree< size_t, std::string > tree;
  tree.insert(std::make_pair(1, "1"));
  tree.insert(std::make_pair(10, "10"));
  tree.insert(std::make_pair(3, "3"));

  demehin::Tree< size_t, std::string > tree_cpy(tree);
  BOOST_TEST(tree_cpy.size() == 3);
  BOOST_TEST((*tree_cpy.begin()).second == "1");

  demehin::Tree< size_t, std::string > tree_mv(std::move(tree_cpy));

  demehin::Tree< size_t, std::string > range_tree(++tree_mv.begin(), tree_mv.end());
  std::ostringstream out;
  printTreeValues(out, range_tree);
  BOOST_TEST(out.str() == "310");

  demehin::Tree< size_t, std::string > ilist_tree{ { 1, "1" }, { 2, "2" }, { 0, "0" } };
  std::ostringstream out1;
  printTreeValues(out1, ilist_tree);
  BOOST_TEST(out1.str() == "012");
}

BOOST_AUTO_TEST_CASE(tree_modifiers_test)
{
  demehin::Tree< size_t, std::string > tree1;
  demehin::Tree< size_t, std::string > tree2;
  tree1.insert(std::make_pair(2, "2"));
  tree1.insert(std::make_pair(6, "6"));
  tree1.insert(std::make_pair(4, "4"));
  tree2.emplace(std::make_pair(10, "10"));
  tree2.emplace(std::make_pair(3, "3"));
  tree2.emplace(std::make_pair(5, "5"));
  tree2.emplace(std::make_pair(1, "1"));

  std::ostringstream out1;
  std::ostringstream out2;
  printTreeValues(out1, tree1);
  printTreeValues(out2, tree2);
  BOOST_TEST(out1.str() == "246");
  BOOST_TEST(out2.str() == "13510");

  tree2.insert(std::make_pair(7, "7"));
  tree2.insert(std::make_pair(11, "11"));
  tree2.erase(++tree2.begin());
  tree2.erase(10);
  std::ostringstream out3;
  printTreeValues(out3, tree2);
  BOOST_TEST(out3.str() == "15711");

  tree1.swap(tree2);
  std::ostringstream out4;
  std::ostringstream out5;
  printTreeValues(out4, tree1);
  printTreeValues(out5, tree2);
  BOOST_TEST(tree1.size() == 4);
  BOOST_TEST(tree2.size() == 3);
  BOOST_TEST(out4.str() == "15711");
  BOOST_TEST(out5.str() == "246");

  tree1.erase(++tree1.begin(), ++(++(++(tree1.begin()))));
  std::ostringstream out6;
  printTreeValues(out6, tree1);
  BOOST_TEST(out6.str() == "1711");

  tree2.insert(std::make_pair(3, "3"));
  tree1.insert(tree2.begin(), --tree2.end());
  std::ostringstream out7;
  printTreeValues(out7, tree1);
  BOOST_TEST(out7.str() == "1234711");
}

BOOST_AUTO_TEST_CASE(hint_insertion_test)
{
  demehin::Tree< size_t, std::string > tree;
  tree[1] = "1";
  tree[3] = "3";
  tree[5] = "5";
  auto hint1 = tree.lower_bound(4);
  auto res1 = tree.emplace_hint(hint1, std::make_pair(4, "4"));

  BOOST_TEST((*res1).first == 4);
  BOOST_TEST((*res1).second == "4");
  BOOST_TEST(tree.count(4) == 1);

  auto hint2 = tree.lower_bound(1);
  auto res2 = tree.insert(hint2, std::make_pair(7, "7"));

  BOOST_TEST((*res2).first == 7);
  BOOST_TEST((*res2).second == "7");
  BOOST_TEST(tree.count(7) == 1);
}

BOOST_AUTO_TEST_CASE(element_access_test)
{
  demehin::Tree< size_t, std::string > tree;
  std::ostringstream out;
  tree.insert(std::make_pair(100, "100"));
  try
  {
    std::string str = tree.at(1);
  }
  catch (const std::out_of_range&)
  {
    out << "exception";
  }
  BOOST_TEST(out.str() == "exception");

  tree[1] = "1";
  BOOST_TEST(tree.at(1) == "1");

  tree.insert(std::make_pair(9, "9"));
  tree.insert(std::make_pair(2, "2"));
  BOOST_TEST(tree[2] == "2");
  BOOST_TEST(tree.at(9) == "9");
}

BOOST_AUTO_TEST_CASE(operations_test)
{
  demehin::Tree< size_t, std::string > tree;
  tree.insert(std::make_pair(1, "1"));
  tree.insert(std::make_pair(5, "5"));
  tree.insert(std::make_pair(3, "3"));
  tree.insert(std::make_pair(7, "7"));

  auto ub1 = tree.upper_bound(3);
  auto lb1 = tree.lower_bound(4);
  auto lb2 = tree.lower_bound(7);

  BOOST_TEST((*ub1).second == "5");
  BOOST_TEST((*lb1).second == "5");
  BOOST_TEST((*lb2).second == "7");

  BOOST_TEST(tree.count(5) == 1);
  BOOST_TEST(tree.count(9) == 0);

  auto it = tree.find(5);
  BOOST_TEST((*it).second == "5");
}

BOOST_AUTO_TEST_CASE(capacity_test)
{
  demehin::Tree< size_t, std::string > tree;
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);

  for (size_t i = 0; i < 6; i++)
  {
    tree[i] = "data";
  }

  BOOST_TEST(!tree.empty());
  BOOST_TEST(tree.size() == 6);

  tree.erase(0);
  tree.erase(1);
  BOOST_TEST(tree.size() == 4);
  BOOST_TEST(!tree.empty());

  tree.clear();
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
}

BOOST_AUTO_TEST_CASE(operators_test)
{
  demehin::Tree< size_t, std::string > tree;
  tree[2] = "2";
  tree[1] = "1";
  tree[3] = "3";

  demehin::Tree< size_t, std::string > cpy_tree = tree;
  std::ostringstream out1;
  printTreeValues(out1, cpy_tree);
  BOOST_TEST(cpy_tree.size() == 3);
  BOOST_TEST(out1.str() == "123");

  cpy_tree.erase(2);
  demehin::Tree< size_t, std::string > mv_tree = std::move(cpy_tree);
  std::ostringstream out2;
  printTreeValues(out2, mv_tree);
  BOOST_TEST(mv_tree.size() == 2);
  BOOST_TEST(out2.str() == "13");
}
