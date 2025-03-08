#include <boost/test/unit_test.hpp>
#include "list.hpp"

using list_t = averenkov::List< int >;

BOOST_AUTO_TEST_SUITE(ListTest)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
    list_t list;
    BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
    list_t list;
    list_t copyList1(list);
    BOOST_TEST(copyList1.empty());

    list.push_back(1);
    list.push_back(2);
    list_t copyList(list);
    BOOST_TEST(copyList.size() == 2);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
    list_t list;
    list.push_back(1);
    list.push_back(2);
    list_t movedList(std::move(list));
    BOOST_TEST(movedList.size() == 2);
}

BOOST_AUTO_TEST_CASE(FillConstructor)
{
    list_t emplist(0, 1);
    BOOST_TEST(emplist.empty());
    list_t list(3, 5);
    BOOST_TEST(list.size() == 3);
}

BOOST_AUTO_TEST_CASE(BeginEnd)
{
    list_t list;
    list.push_back(1);
    list.push_back(2);

    auto it = list.begin();
    BOOST_TEST(*it == 1);
    ++it;
    BOOST_TEST(*it == 2);
    ++it;
    auto it2 = list.end();
    BOOST_TEST(*it == *it2);
}

BOOST_AUTO_TEST_CASE(CBeginCEnd)
{
    list_t list;
    list.push_back(1);
    list.push_back(2);

    auto it = list.cbegin();
    BOOST_TEST(*it == 1);
    ++it;
    BOOST_TEST(*it == 2);
    ++it;
    auto it2 = list.cend();
    BOOST_TEST(*it == *it2);
}


BOOST_AUTO_TEST_CASE(FrontBack)
{
    list_t list;
    list.push_back(1);
    list.push_back(2);
    BOOST_TEST(list.front() == 1);
    BOOST_TEST(list.back() == 2);
}

BOOST_AUTO_TEST_CASE(PushFront)
{
    list_t list;
    list.push_front(1);
    list.push_front(2);
    BOOST_TEST(list.front() == 2);
    BOOST_TEST(list.back() == 1);
}

BOOST_AUTO_TEST_CASE(PushBack)
{
    list_t list;
    list.push_back(1);
    BOOST_TEST(list.back() == 1);

    list.clear();
    list.push_back(2);
    list.push_back(3);
    BOOST_TEST(list.back() == 3);
}

BOOST_AUTO_TEST_CASE(PopFront)
{
    list_t list;
    list.push_back(1);
    list.push_back(2);

    list.pop_front();
    BOOST_TEST(list.front() == 2);
}

BOOST_AUTO_TEST_CASE(PopBack)
{
    list_t list;
    list.push_back(1);
    list.push_back(2);

    list.pop_back();
    BOOST_TEST(list.back() == 1);
}

BOOST_AUTO_TEST_CASE(Clear)
{
    list_t list;
    list.clear();
    BOOST_TEST(list.size() == 0);

    list.push_back(2);
    list.push_back(3);
    list.clear();
    BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(Swap)
{
    list_t list1;
    list1.push_back(1);
    list1.push_back(2);

    list_t list2;
    list2.push_back(3);
    list2.push_back(4);

    list1.swap(list2);
    BOOST_TEST(list1.front() == 3);
}

BOOST_AUTO_TEST_CASE(Remove)
{
    list_t list1;
    list1.push_back(1);
    list1.remove(1);
    BOOST_TEST(list1.empty());

    list_t list2;
    list1.push_back(1);
    list1.remove(2);
    BOOST_TEST(list1.size() == 1);

    list_t list3;
    list3.push_back(1);
    list3.push_back(2);
    list3.push_back(2);
    list3.push_back(3);
    list3.remove(2);
    BOOST_TEST(list3.size() == 2);
}

BOOST_AUTO_TEST_CASE(RemoveIf)
{
    list_t list1;
    list1.push_back(2);
    list1.removeIf([](int value)
    {
      return value % 2 == 0;
    });
    BOOST_TEST(list1.empty());

    list_t list2;
    list2.push_back(1);
    list2.removeIf([](int value)
    {
      return value % 2 == 0;
    });
    BOOST_TEST(list2.size() == 1);

    list_t list3;
    list3.push_back(1);
    list3.push_back(2);
    list3.push_back(3);
    list3.removeIf([](int value)
    {
      return value % 2 == 0;
    });
    BOOST_TEST(list3.size() == 2);
}

BOOST_AUTO_TEST_CASE(Assign)
{
    list_t list;
    list.assign(0, 0);
    BOOST_TEST(list.empty());
    list.assign(3, 5);
    BOOST_TEST(list.size() == 3);
}

BOOST_AUTO_TEST_SUITE_END()
