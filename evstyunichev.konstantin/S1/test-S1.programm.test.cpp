#include <boost/test/unit_test.hpp>
#include <sstream>
#include "../common/list.hpp"

BOOST_AUTO_TEST_SUITE(main_tests)

BOOST_AUTO_TEST_CASE(construct_test)
{
  using list_ull = evstyunichev::List< size_t >;
  list_ull list1{}, list2{ list1 };
}

BOOST_AUTO_TEST_CASE(push_back_test)
{
  evstyunichev::List< int > list1{};
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  std::stringstream out;
  list1.out(out);
  BOOST_TEST(out.str() == "1 2 3");
}

BOOST_AUTO_TEST_CASE(front_back_test)
{
  evstyunichev::List< int > list2{};
  list2.push_back(2);
  list2.push_front(1);
  list2.push_front(3);
  BOOST_TEST(list2.front() == 3);
  BOOST_TEST(list2.back() == 2);
}

BOOST_AUTO_TEST_CASE(pop_test)
{
  evstyunichev::List< int > list3{};
  list3.push_back(2);
  list3.push_back(3);
  BOOST_TEST(list3.back() == 3);
  list3.pop_back();
  BOOST_TEST(list3.back() == 2);
  list3.push_front(4);
  list3.push_front(6);
  list3.pop_front();
  BOOST_TEST(list3.front() == 4);
}

BOOST_AUTO_TEST_CASE(splice_test_simple)
{
  evstyunichev::List< int > list1{}, list3{};
  list3.push_back(2);
  list3.push_back(3);
  list1.push_back(10);
  list1.splice(list1.begin(), list3);
  auto Citer1 = list1.cbegin();
  ++Citer1;
  BOOST_TEST(*Citer1 == 3);
  BOOST_TEST(list3.empty());
}

BOOST_AUTO_TEST_CASE(splice_test)
{
  std::stringstream out;
  evstyunichev::List< int > list1{ 5, 52 }, list2{ 7, 77 }, list3{}, list4{};
  list2.push_back(4);
  list2.push_front(2);
  list3.push_back(2);
  list3.push_back(3);
  list1.push_back(10);
  list1.splice(list1.begin(), list3);
  list2.out(out);
  std::cout << out.str() << '\n';
  auto iter1 = list1.begin(), iter2_b = ++(list2.begin()), iter2_e = ++(list2.end());
  ++iter1;
  BOOST_TEST(*iter1 == 3);
  ++iter1;
  ++iter1;
  BOOST_TEST(*iter1 == 52);
  BOOST_TEST(list3.empty());
}

BOOST_AUTO_TEST_SUITE_END()
