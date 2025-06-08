#include <boost/test/unit_test.hpp>
#include <list/list.hpp>
#include <list/iterators.hpp>
#include <list/listNode.hpp>

BOOST_AUTO_TEST_CASE(test_constructors_operators)
{
  using list_t = alymova::List< int >;

  list_t list1;
  BOOST_TEST(list1.empty());
  list_t list2(list1);
  BOOST_TEST(list1 == list2);

  list_t list3(list_t(3ull, 1));
  BOOST_TEST(list3.size() == 3);
  BOOST_TEST(list3.front() == 1);

  list_t list4(list3.begin(), list3.end());
  BOOST_TEST(list4 == list3);

  list_t list5{1, 1, 1};
  BOOST_TEST(list5 == list3);

  list3 = list4;
  BOOST_TEST(list3 == list4);

  list4 = list_t(3ull, 1);
  BOOST_TEST(list4 == list5);

  list4 = {2, 2, 2};
  list_t list6{2, 2, 2};
  BOOST_TEST(list4 == list6);
}
BOOST_AUTO_TEST_CASE(test_list_iterators)
{
  using list_t = alymova::List< int >;

  list_t list(3ull, 1);
  auto iter_b = list.begin();
  auto iter_e = list.end();
  BOOST_TEST(*iter_b == 1);
  BOOST_TEST(*iter_e == 0);

  const list_t list1(2ull, 3);
  auto citer_b = list1.cbegin();
  auto citer_e = list1.cend();
  BOOST_TEST(*citer_b == 3);
  BOOST_TEST(*citer_e == 0);

  citer_b = list1.begin();
  citer_e = list1.end();
  BOOST_TEST(*citer_b == 3);
  BOOST_TEST(*citer_e == 0);
}
BOOST_AUTO_TEST_CASE(test_size)
{
  using list_t = alymova::List< int >;

  list_t list;
  BOOST_TEST(list.size() == 0);

  list.push_back(1);
  BOOST_TEST(list.size() == 1);

  list.pop_back();
  BOOST_TEST(list.empty());
}
BOOST_AUTO_TEST_CASE(test_base_interface)
{
  using list_t = alymova::List< int >;

  list_t list;
  BOOST_TEST(list.size() == 0);
  BOOST_TEST(list.empty());

  list.push_front(1);
  BOOST_TEST(list.front() == 1);
  BOOST_TEST(list.size() == 1);

  list.push_front(2);
  BOOST_TEST(list.back() == 1);

  list.pop_front();
  BOOST_TEST(list.front() == 1);

  list.pop_back();
  BOOST_TEST(list.empty());

  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  BOOST_TEST(list.front() == 10);
  BOOST_TEST(list.back() == 30);
  BOOST_TEST(*(++list.begin()) == 20);
  BOOST_TEST(list.size() == 3);

  list.pop_back();
  BOOST_TEST(list.back() == 20);

  const list_t list1(3ull, 1);
  BOOST_TEST(list1.front() == 1);
  BOOST_TEST(list1.back() == 1);
}
BOOST_AUTO_TEST_CASE(test_swap)
{
  using list_t = alymova::List< int >;

  list_t list1{2, 5};
  list_t list2{5, 2};
  list_t list3(list2);
  list1.swap(list2);
  BOOST_TEST(list1 == list3);
}
template< typename T >
bool is_divided_10(const T& value)
{
  return value % 10 == 0;
}
BOOST_AUTO_TEST_CASE(test_remove)
{
  using list_t = alymova::List< int >;
  using list_str_t = alymova::List< std::string >;

  list_str_t list1(1, "cat");
  list1.remove("cat");
  BOOST_TEST(list1.size() == 0);

  list1.push_back("dog");
  list1.push_back("student");
  list1.remove("student");
  BOOST_TEST(list1.back() == "dog");

  list_str_t list2(10, "cat");
  list2.push_back("fox");
  list2.push_front("dolphin");
  list2.remove("cat");
  BOOST_TEST(list2.size() == 2);

  list_t list3(5ull, 10);
  list3.push_back(1);
  list3.remove_if(is_divided_10< int >);
  BOOST_TEST(list3.size() == 1);
  BOOST_TEST(list3.front() == 1);
}
BOOST_AUTO_TEST_CASE(test_assign)
{
  using list_t = alymova::List< int >;

  list_t list1;
  list1.assign(0ull, 1);
  BOOST_TEST(list1.empty());

  list1.assign(5ull, 1);
  list_t list_comp(5ull, 1);
  BOOST_TEST(list1 == list_comp);

  list_t list2{1, 2, 3};
  list2.assign(list1.begin(), list1.begin());
  BOOST_TEST(list2.empty());

  list2.assign(list1.begin(), list1.end());
  BOOST_TEST(list2 == list1);

  list2.assign({});
  BOOST_TEST(list2.empty());

  list2.assign({1, 2, 3});
  list_comp = {1, 2, 3};
  BOOST_TEST(list2 == list_comp);
}
BOOST_AUTO_TEST_CASE(test_splice)
{
  using list_t = alymova::List< int >;

  list_t list1, list2, list_comp1, list_comp2;
  for (size_t i = 1; i <= 4; i++)
  {
    list1.push_back(i);
  }
  for (size_t i = 1; i <= 3; i++)
  {
    list2.push_back(i * 10);
  }
  auto iter1 = list2.begin();
  list1.splice(list1.begin(), list2, iter1);
  list_comp1 = {10, 1, 2, 3, 4};
  list_comp2 = {20, 30};
  BOOST_TEST(list1 == list_comp1);
  BOOST_TEST(list2 == list_comp2);
  BOOST_TEST(*iter1 == 10);

  auto iter2 = --list2.end();
  list1.splice(list1.begin(), list2, iter2);
  list_comp1.push_front(30);
  list_comp2.pop_back();
  BOOST_TEST(list1 == list_comp1);
  BOOST_TEST(list2 == list_comp2);
  BOOST_TEST(*iter2 == 30);

  auto iter3 = list2.begin();
  list1.splice(--list1.end(), list2, iter3);
  list_comp1 = {30, 10, 1, 2, 3, 20, 4};
  BOOST_TEST(list1 == list_comp1);
  BOOST_TEST(list2.empty());
  BOOST_TEST(*iter3 == 20);

  for (size_t i = 1; i <= 3; i++)
  {
    list2.push_back(i * 100);
  }
  iter3 = --list1.end();
  list2.splice(--list2.end(), list1, iter3);
  list_comp1.pop_back();
  list_comp2 = {100, 200, 4, 300};
  BOOST_TEST(list1 == list_comp1);
  BOOST_TEST(list2 == list_comp2);
  BOOST_TEST(*(++iter3) == 300);

  list_t list3;
  list_t list2_copy = list2;
  list2.splice(list2.begin(), list3);
  BOOST_TEST(list2 == list2_copy);

  list2.splice(list2.begin(), list3, list3.begin(), list3.end());
  BOOST_TEST(list2 == list2_copy);
}
BOOST_AUTO_TEST_CASE(test_insert)
{
  using list_t = alymova::List< int >;

  list_t list1{1, 2, 3};
  alymova::Iterator< int > it_res = list1.insert(list1.begin(), 10);
  list_t list_comp{10, 1, 2, 3};
  BOOST_TEST(*it_res = 10);
  BOOST_TEST(list1 == list_comp);

  it_res = list1.insert(list1.end(), 20);
  list_comp.push_back(20);
  BOOST_TEST(list1 == list_comp);
  BOOST_TEST(*it_res = 20);

  it_res = list1.insert(++(list1.begin()), 30);
  list_comp = {10, 30, 1, 2, 3, 20};
  BOOST_TEST(list1 == list_comp);
  BOOST_TEST(*it_res == 30);

  list1 = {1, 2, 3};
  list_comp = {4, 4, 4, 1, 2, 3};
  it_res = list1.insert(list1.begin(), (size_t)3, 4);
  BOOST_TEST(list1 == list_comp);
  BOOST_TEST(*it_res == 4);

  list_t list2;
  it_res = list2.insert(list2.begin(), list1.begin(), list1.end());
  BOOST_TEST(list2 == list_comp);
  BOOST_TEST(*it_res == 4);

  list2.clear();
  it_res = list2.insert(list2.begin(), {4, 4, 4, 1, 2, 3});
  BOOST_TEST(list2 == list_comp);
  bool b = (it_res == list2.begin());
  BOOST_TEST(b);
}
BOOST_AUTO_TEST_CASE(test_erase)
{
  using list_t = alymova::List< int >;

  list_t list1{1, 2, 3, 4, 5};
  auto it_res = list1.erase(list1.begin());
  BOOST_TEST(list1.front() == 2);
  BOOST_TEST(*it_res == 2);

  it_res = list1.erase(--(list1.end()));
  BOOST_TEST(list1.back() == 4);
  bool b = (it_res == list1.end());
  BOOST_TEST(b);

  it_res = list1.erase(++(list1.begin()));
  list_t list_comp{2, 4};
  BOOST_TEST(list1 == list_comp);
  BOOST_TEST(*it_res == 4);

  it_res = list1.erase(list1.begin(), list1.end());
  BOOST_TEST(list1.empty());
  b = (it_res == list1.end());
  BOOST_TEST(b);
}
BOOST_AUTO_TEST_CASE(test_ralational_operators)
{
  using list_t = alymova::List< int >;

  list_t list1;
  list_t list2;
  BOOST_TEST(list1 == list2);
  BOOST_TEST(list1 >= list2);
  BOOST_TEST(list1 <= list2);

  list1 = {1, 2, 3};
  list2 = {1, 2, 3};
  BOOST_TEST(list1 == list2);
  BOOST_TEST(list1 <= list2);
  BOOST_TEST(list1 >= list2);

  list2.pop_back();
  BOOST_TEST(list1 != list2);
  BOOST_TEST(list1 > list2);
  BOOST_TEST(list2 < list1);

  list2.push_back(4);
  BOOST_TEST(list1 != list2);
  BOOST_TEST(list1 < list2);
  BOOST_TEST(list2 > list1);
}
BOOST_AUTO_TEST_CASE(test_reverse)
{
  using list_t = alymova::List< int >;

  list_t list1;
  list_t list_comp = list1;
  list1.reverse();
  BOOST_TEST(list1 == list_comp);

  list1 = {1, 2};
  list_comp = {2, 1};
  list1.reverse();
  BOOST_TEST(list1 == list_comp);

  list1 = {1, 2, 3, 4, 5};
  list_comp = {5, 4, 3, 2, 1};
  list1.reverse();
  BOOST_TEST(list1 == list_comp);
}
template< typename T >
struct forEmplace
{
  T one;
  T two;
  bool operator==(const forEmplace& other) const
  {
    return (one == other.one) && (two == other.two);
  }
  bool operator!=(const forEmplace& other) const
  {
    return !(*this == other);
  }
};
BOOST_AUTO_TEST_CASE(test_emplace)
{
  using list_t = alymova::List< forEmplace< int > >;

  list_t list1;
  forEmplace< int > object{1, 2};
  list_t list_comp{1, object};
  alymova::Iterator< forEmplace< int > > it_res = list1.emplace_front(1, 2);
  BOOST_TEST(list1 == list_comp);
  bool b = (*it_res == object);
  BOOST_TEST(b);

  object = {3, 4};
  list_comp.push_back(object);
  it_res = list1.emplace_back(3, 4);
  BOOST_TEST(list1 == list_comp);
  b = (*it_res == object);
  BOOST_TEST(b);

  object = {5, 6};
  list_comp.insert(++(list_comp.begin()), object);
  it_res = list1.emplace(++(list1.begin()), 5, 6);
  BOOST_TEST(list1 == list_comp);
  b = (*it_res == object);
  BOOST_TEST(b);
}
template< typename T >
bool is_divided_other(const T& one, const T& two)
{
  return (one % two == 0);
}
BOOST_AUTO_TEST_CASE(test_unique)
{
  using list_t = alymova::List< int >;

  list_t list1;
  list1.unique();
  BOOST_TEST(list1.empty());

  list1 = {1, 1, 2, 2, 3, 4, 4, 4, 5};
  list_t list_comp{1, 2, 3, 4, 5};
  list1.unique();
  BOOST_TEST(list1 == list_comp);

  list1.unique();
  BOOST_TEST(list1 == list_comp);

  list_comp.erase(++(list_comp.begin()), list_comp.end());
  list1.unique(is_divided_other< int >);
  BOOST_TEST(list1 == list_comp);
}
template< typename T >
bool is_greater(const T& one, const T& two)
{
  return one > two;
}
BOOST_AUTO_TEST_CASE(test_sort)
{
  using list_t = alymova::List< int >;

  list_t list1;
  list1.sort();
  BOOST_TEST(list1.empty());

  list1.push_back(1);
  list1.sort();
  BOOST_TEST(list1.front() == 1);
  BOOST_TEST(list1.size() == 1);

  list1 = {1, 2, 2, 3};
  list_t list_comp{1, 2, 2, 3};
  list1.sort();
  BOOST_TEST(list1 == list_comp);

  list1 = {2, 3, 1, 2};
  list1.sort();
  BOOST_TEST(list1 == list_comp);

  list_comp = {3, 2, 2, 1};
  list1.sort(is_greater< int >);
  BOOST_TEST(list1 == list_comp);
}
BOOST_AUTO_TEST_CASE(test_merge)
{
  using list_t = alymova::List< int >;

  list_t list1{1, 2, 3, 4};
  list_t list_comp = list1;
  list1.merge(list1);
  BOOST_TEST(list1 == list_comp);

  list_t list2{3, 2, 1};
  try
  {
    list1.merge(list2);
  }
  catch (const std::exception& e)
  {
    BOOST_TEST(list1 == list_comp);
  }

  list2.clear();
  list1.merge(list2);
  BOOST_TEST(list1 == list_comp);

  list2 = {0, 0, 0};
  list_comp = {0, 0, 0, 1, 2, 3, 4};
  list1.merge(list2);
  BOOST_TEST(list1 == list_comp);

  list2 = {2, 2, 4, 5};
  list_comp = {0, 0, 0, 1, 2, 2, 2, 3, 4, 4, 5};
  list1.merge(list2);
  BOOST_TEST(list1 == list_comp);

  list1.clear();
  list2.sort(is_greater< int >);
  list1.merge(list2, is_greater< int >);
}
