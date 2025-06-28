#include <boost/test/unit_test.hpp>
#include <sstream>
#include <avl_tree.hpp>

BOOST_AUTO_TEST_SUITE(empty_method)

BOOST_AUTO_TEST_CASE(empty_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  out << tree.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(not_empty_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 1, "hi" });
  out << tree.empty();
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(size_method)

BOOST_AUTO_TEST_CASE(no_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  out << tree.size();
  BOOST_TEST(out.str() == "0");
}

BOOST_AUTO_TEST_CASE(one_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 1, "hi" });
  out << tree.size();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(ya_one_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 1, "hi" });
  tree.insert({ 1, "my" });
  tree.insert({ 1, "best" });
  tree.insert({ 1, "friend" });
  out << tree.size();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(three_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 1, "hi" });
  tree.insert({ 2, "my" });
  tree.insert({ 3, "friend" });
  out << tree.size();
  BOOST_TEST(out.str() == "3");
}

BOOST_AUTO_TEST_CASE(ya_three_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 1, "hi" });
  tree.insert({ 2, "my" });
  tree.insert({ 2, "best" });
  tree.insert({ 3, "friend" });
  out << tree.size();
  BOOST_TEST(out.str() == "3");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(clear_method)

BOOST_AUTO_TEST_CASE(clear_empty_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.clear();
  out << tree.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(clear_one_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 1, "hi" });
  tree.clear();
  out << tree.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(clear_three_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 1, "hi" });
  tree.insert({ 2, "my" });
  tree.insert({ 3, "friend" });
  tree.clear();
  out << tree.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(clear_five_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 1, "I" });
  tree.insert({ 2, "like" });
  tree.insert({ 3, "eating" });
  tree.insert({ 4, "fresh"});
  tree.insert({ 5, "cats"});
  tree.clear();
  out << tree.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(insert_method)

BOOST_AUTO_TEST_CASE(insert_one_element)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 1, "hi" });
  out << tree[1];
  BOOST_TEST(out.str() == "hi");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(going_through_tree)

BOOST_AUTO_TEST_CASE(one_element_avl_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 1, "hi" });
  for (auto it = tree.cbegin(); it != tree.cend(); ++it)
  {
    out << it->second;
  }
  BOOST_TEST(out.str() == "hi");
}

BOOST_AUTO_TEST_CASE(two_element_avl_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 1, "hi" });
  tree.insert({ 2, "cute" });
  for (auto it = tree.cbegin(); it != tree.cend(); ++it)
  {
    out << it->second;
  }
  BOOST_TEST(out.str() == "hicute");
}

BOOST_AUTO_TEST_CASE(ya_two_element_avl_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 2, "cute" });
  tree.insert({ 1, "hi" });
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    out << it->second;
  }
  BOOST_TEST(out.str() == "hicute");
}

BOOST_AUTO_TEST_CASE(three_element_avl_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 2, "cute" });
  tree.insert({ 1, "hi" });
  tree.insert({ 3, "cat" });
  for (auto it = tree.cbegin(); it != tree.cend(); ++it)
  {
    out << it->second;
  }
  BOOST_TEST(out.str() == "hicutecat");
}

BOOST_AUTO_TEST_CASE(five_element_avl_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 3, "very" });
  tree.insert({ 1, "hi" });
  tree.insert({ 2, "cute" });
  tree.insert({ 4, "tasty" });
  tree.insert({ 5, "cat" });
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    out << it->second;
  }
  BOOST_TEST(out.str() == "hicuteverytastycat");
}

BOOST_AUTO_TEST_CASE(ya_five_element_avl_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 1, "very" });
  tree.insert({ 2, "hi" });
  tree.insert({ 3, "cute" });
  tree.insert({ 4, "tasty" });
  tree.insert({ 5, "cat" });
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    out << it->second;
  }
  BOOST_TEST(out.str() == "veryhicutetastycat");
}

BOOST_AUTO_TEST_CASE(eleven_element_avl_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, int > tree;
  tree.insert({ 6, 6 });
  tree.insert({ 2, 2 });
  tree.insert({ 1, 1 });
  tree.insert({ 4, 4 });
  tree.insert({ 3, 3 });
  tree.insert({ 5, 5 });
  tree.insert({ 9, 9 });
  tree.insert({ 7, 7 });
  tree.insert({ 8, 8 });
  tree.insert({ 11, 11 });
  tree.insert({ 10, 10 });
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    out << it->second;
  }
  BOOST_TEST(out.str() == "1234567891011");
}

BOOST_AUTO_TEST_CASE(stress_test)
{
  std::ostringstream out;
  petrov::AVLTree< int, int > tree;
  for (size_t i = 0; i < 1000000; ++i)
  {
    tree.insert({ i, i });
  }
  tree.clear();
  out << tree.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(find_method)

BOOST_AUTO_TEST_CASE(find_one_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, int > tree;
  tree.insert({ 1, 1 });
  auto it = tree.find(1);
  out << it->second;
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(find_eleven_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, int > tree;
  for (size_t i = 1; i <= 11; ++i)
  {
    tree.insert({ i, i });
  }
  auto it_1 = tree.find(11);
  auto it_2 = tree.find(7);
  out << it_1->second << " " << it_2->second;
  BOOST_TEST(out.str() == "11 7");
}

BOOST_AUTO_TEST_CASE(find_five_element_tree)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 1, "very" });
  tree.insert({ 2, "hi" });
  tree.insert({ 3, "cute" });
  tree.insert({ 4, "tasty" });
  tree.insert({ 5, "cat" });
  auto it_1 = tree.find(2);
  auto it_2 = tree.find(5);
  out << it_1->second << " " << it_2->second;
  BOOST_TEST(out.str() == "hi cat");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(erase_method)

BOOST_AUTO_TEST_CASE(erase_one_element)
{
  std::ostringstream out;
  petrov::AVLTree< int, int > tree;
  tree.insert({ 1, 1 });
  tree.erase(1);
  out << tree.empty();
  BOOST_TEST(out.str() == "1");
}

BOOST_AUTO_TEST_CASE(erase_three_elements)
{
  std::ostringstream out;
  petrov::AVLTree< int, std::string > tree;
  tree.insert({ 1, "very" });
  tree.insert({ 2, "hi" });
  tree.insert({ 3, "cute" });
  tree.insert({ 4, "tasty" });
  tree.insert({ 5, "cat" });
  tree.erase(2);
  tree.erase(3);
  tree.erase(5);
  for (auto it = tree.cbegin(); it != tree.cend(); ++it)
  {
    out << it->second;
  }
  BOOST_TEST(out.str() == "verytasty");
}

BOOST_AUTO_TEST_CASE(erase_five_elements)
{
  std::ostringstream out;
  petrov::AVLTree< int, int > tree;
  tree.insert({ 6, 6 });
  tree.insert({ 2, 2 });
  tree.insert({ 1, 1 });
  tree.insert({ 4, 4 });
  tree.insert({ 3, 3 });
  tree.insert({ 5, 5 });
  tree.insert({ 9, 9 });
  tree.insert({ 7, 7 });
  tree.insert({ 8, 8 });
  tree.insert({ 11, 11 });
  tree.insert({ 10, 10 });
  tree.erase(5);
  tree.erase(8);
  tree.erase(11);
  tree.erase(6);
  tree.erase(1);
  for (auto it = tree.cbegin(); it != tree.cend(); ++it)
  {
    out << it->second;
  }
  BOOST_TEST(out.str() == "2347910");
}

BOOST_AUTO_TEST_SUITE_END()
