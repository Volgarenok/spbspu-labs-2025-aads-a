#include <boost/test/unit_test.hpp>
#include <tree/tree.hpp>

BOOST_AUTO_TEST_CASE(construction)
{
  using namespace sveshnikov;
  AvlTree< int, std::string > tree1;
  bool is_equal = (tree1.begin() == tree1.end());
  BOOST_TEST(is_equal);
  BOOST_TEST(tree1.empty());

  tree1[1] = "a";
  tree1[2] = "b";
  AvlTree< int, std::string > tree2 = tree1;
  BOOST_TEST(tree1.size() == 2);
  BOOST_TEST(tree1[1] == "a");
  BOOST_TEST(tree2.size() == 2);
  BOOST_TEST(tree2[1] == "a");

  AvlTree< int, std::string > tree3 = std::move(tree1);
  BOOST_TEST(tree1.empty());
  BOOST_TEST(tree3.size() == 2);
  BOOST_TEST(tree3[1] == "a");

  tree1 = std::move(tree2);
  BOOST_TEST(tree2.empty());
  BOOST_TEST(tree1.size() == 2);
  BOOST_TEST(tree1[1] == "a");

  tree2 = tree3;
  BOOST_TEST(tree2.size() == 2);
  BOOST_TEST(tree2[2] == "b");
  BOOST_TEST(tree3.size() == 2);
  BOOST_TEST(tree3[2] == "b");
}

BOOST_AUTO_TEST_CASE(elsement_access)
{
  sveshnikov::AvlTree< int, std::string > tree;
  tree[1] = "a";
  tree[2] = "b";

  BOOST_TEST(tree.size() == 2);
  BOOST_TEST(tree[2] == "b");
  BOOST_TEST(tree.begin()->second == "a");
  BOOST_TEST((--tree.end())->second == "b");
  BOOST_TEST(tree.begin()->first == 1);
  tree.at(2) = "r";
  BOOST_TEST((++tree.begin())->second == "r");
  BOOST_TEST(tree.at(2) == "r");
}

BOOST_AUTO_TEST_CASE(swap_and_clear)
{
  sveshnikov::AvlTree< int, std::string > tree1;
  tree1[1] = "a";
  tree1[2] = "b";
  sveshnikov::AvlTree< int, std::string > tree2;
  tree2[1] = "c";
  tree2[2] = "b";
  tree2[3] = "r";
  tree1.swap(tree2);
  BOOST_TEST(tree1.size() == 3);
  BOOST_TEST(tree1[1] == "c");
  BOOST_TEST(tree2.size() == 2);
  BOOST_TEST(tree2[1] == "a");

  tree1.clear();
  BOOST_TEST(tree1.empty());
  bool is_equal = (tree1.begin() == tree1.end());
  BOOST_TEST(is_equal);
}

BOOST_AUTO_TEST_CASE(insert)
{
  sveshnikov::AvlTree< int, std::string > tree;
  auto it = tree.insert({1, "a"}).first;
  int arr[5] = {2, 5, 4, 6, 3};
  for (size_t i = 0; i < 5; i++)
  {
    tree.insert({arr[i], "a"});
  }
  BOOST_TEST(tree.size() == 6);
  bool is_equal = (it == tree.begin());
  BOOST_TEST(is_equal);
  bool is_insert = tree.insert({3, "a"}).second;
  BOOST_TEST(!is_insert);
}

BOOST_AUTO_TEST_CASE(erase)
{
  sveshnikov::AvlTree< int, std::string > tree;
  int arr1[7] = {2, 6, 4, 5, 3, 1, 0};
  for (size_t i = 0; i < 7; i++)
  {
    tree.insert({arr1[i], "a"});
  }
  auto it = tree.insert({7, "a"}).first;

  tree.erase(it);
  BOOST_TEST(tree.size() == 7);
  int is_erase = tree.erase(8);
  BOOST_TEST(is_erase == 0);
  int arr2[6] = {2, 4, 5, 0, 3, 1};
  size_t size = 7;
  for (size_t i = 0; i < 6; i++)
  {
    size--;
    is_erase = tree.erase(arr2[i]);
    BOOST_TEST(is_erase == 1);
    BOOST_TEST((--tree.end())->first == 6);
    BOOST_TEST(tree.size() == size);
  }

  BOOST_TEST(tree.begin()->first == 6);
  tree.erase(6);
  BOOST_TEST(is_erase == 1);
  BOOST_TEST(tree.empty());
  is_erase = tree.erase(11);
  BOOST_TEST(is_erase == 0);
}

BOOST_AUTO_TEST_CASE(operations)
{
  sveshnikov::AvlTree< int, std::string > tree;
  bool is_equal1 = (tree.find(7) == tree.end());
  BOOST_TEST(is_equal1);

  int arr[7] = {2, 6, 4, 5, 3, 1, 7};
  for (size_t i = 0; i < 7; i++)
  {
    tree.insert({arr[i], "a"});
  }
  bool is_equal2 = (tree.find(7) == --(tree.end()));
  BOOST_TEST(is_equal2);
  bool is_equal3 = (tree.find(1) == tree.begin());
  BOOST_TEST(is_equal3);

  auto it_pair = tree.equal_range(0);
  bool is_equal4 = (it_pair.first == it_pair.second);
  BOOST_TEST(is_equal4);
  bool is_equal5 = (it_pair.first == tree.find(1));
  BOOST_TEST(is_equal5);

  BOOST_TEST(tree.count(5) == 1);
  BOOST_TEST(tree.count(9) == 0);
}
