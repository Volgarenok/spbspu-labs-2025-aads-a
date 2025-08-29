#include <boost/test/unit_test.hpp>
#include "deque.hpp"

using aleksandrov::Deque;

namespace
{
  struct Point
  {
    int x;
    int y;

    Point() = delete;
    Point(int x, int y):
      x(x),
      y(y)
    {}

    bool operator==(const Point& rhs) const
    {
      return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const Point& rhs) const
    {
      return !operator==(rhs);
    }
    friend std::ostream& operator<<(std::ostream& out, const Point& rhs)
    {
      return out << '(' << rhs.x << ';' << rhs.y << ')';
    }
  };
}

BOOST_AUTO_TEST_CASE(deque_default_construction)
{
  Deque< Point > d;
  BOOST_TEST(d.empty());
  BOOST_TEST(d.size() == 0);
  BOOST_TEST(d.maxSize() >= 0);
  BOOST_TEST(d.capacity() == 0);
}

BOOST_AUTO_TEST_CASE(deque_copy_construction)
{
  Deque< Point > d1;
  Deque< Point > d1Copy(d1);
  BOOST_TEST(d1Copy.empty());
  BOOST_TEST(d1Copy.size() == 0);
  BOOST_TEST(d1Copy.capacity() == 0);

  Deque< Point > d2;
  d2.pushBack(Point(1, 1));
  d2.pushFront(Point(2, 2));
  Deque< Point > d2Copy(d2);
  BOOST_TEST(d2Copy.size() == 2);
  BOOST_TEST(d2Copy.capacity() == d2.capacity());
  BOOST_TEST(d2Copy.front() == Point(2, 2));
  BOOST_TEST(d2Copy.back() == Point(1, 1));
}

BOOST_AUTO_TEST_CASE(deque_move_construction)
{
  Deque< Point > d1;
  Deque< Point > d1Copy(std::move(d1));
  BOOST_TEST(d1Copy.empty());
  BOOST_TEST(d1Copy.size() == 0);
  BOOST_TEST(d1Copy.capacity() == 0);

  Deque< Point > d2;
  d2.pushBack(Point(1, 1));
  d2.pushFront(Point(2, 2));
  Deque< Point > d2Copy(std::move(d2));
  BOOST_TEST(d2Copy.size() == 2);
  BOOST_TEST(d2Copy.front() == Point(2, 2));
  BOOST_TEST(d2Copy.back() == Point(1, 1));
  BOOST_TEST(d2.empty());
  BOOST_TEST(d2.capacity() == 0);
}

BOOST_AUTO_TEST_CASE(deque_copy_assignment)
{
  Deque< Point > d1;
  Deque< Point > d2;
  d2 = d1;
  BOOST_TEST(d2.empty());
  BOOST_TEST(d2.capacity() == 0);

  d1.pushBack(Point(1, 1));
  d1.pushFront(Point(2, 2));
  d2 = d1;
  BOOST_TEST(d2.size() == 2);
  BOOST_TEST(d2.capacity() == d1.capacity());
  BOOST_TEST(d2.front() == Point(2, 2));
  BOOST_TEST(d2.back() == Point(1, 1));

  d1.clear();
  d2 = d1;
  BOOST_TEST(d2.empty());
  BOOST_TEST(d2.capacity() == d1.capacity());

  d1.pushBack(Point(1, 1));
  d1.pushFront(Point(2, 2));
  d2.pushBack(Point(3, 3));
  d2.pushFront(Point(4, 4));
  d2 = d1;
  BOOST_CHECK(d2 == d1);
}

BOOST_AUTO_TEST_CASE(deque_move_assignment)
{
  Deque< Point > d1;
  Deque< Point > d2;
  d2 = std::move(d1);
  BOOST_TEST(d2.empty());
  BOOST_TEST(d2.capacity() == 0);

  d1.pushBack(Point(1, 1));
  d1.pushFront(Point(2, 2));
  d2 = std::move(d1);
  BOOST_TEST(d2.size() == 2);
  BOOST_TEST(d2.front() == Point(2, 2));
  BOOST_TEST(d2.back() == Point(1, 1));
  BOOST_TEST(d1.empty());
  BOOST_TEST(d1.capacity() == 0);

  d2 = std::move(d1);
  BOOST_TEST(d2.empty());
  BOOST_TEST(d2.capacity() == 0);

  d2 = std::move(d1);
  BOOST_CHECK(d2 == d1);
}

BOOST_AUTO_TEST_CASE(deque_front_back)
{
  Deque< double > d;

  d.pushBack(1.0);
  BOOST_TEST(d.front() == 1.0);
  BOOST_TEST(d.back() == 1.0);

  d.pushBack(2.0);
  BOOST_TEST(d.front() == 1.0);
  BOOST_TEST(d.back() == 2.0);

  d.popBack();
  BOOST_TEST(d.front() == 1.0);
  BOOST_TEST(d.back() == 1.0);

  d.pushFront(2.0);
  BOOST_TEST(d.front() == 2.0);
  BOOST_TEST(d.back() == 1.0);

  d.popFront();
  BOOST_TEST(d.front() == 1.0);
  BOOST_TEST(d.back() == 1.0);
}

BOOST_AUTO_TEST_CASE(deque_empty_size_capacity)
{
  Deque< char > d;
  BOOST_TEST(d.empty());
  BOOST_TEST(d.size() == 0);
  BOOST_TEST(d.capacity() == 0);

  d.pushBack('a');
  BOOST_TEST(!d.empty());
  BOOST_TEST(d.size() == 1);
  BOOST_TEST(d.capacity() > 0);

  d.popBack();
  BOOST_TEST(d.empty());
  BOOST_TEST(d.size() != 1);
  BOOST_TEST(d.capacity() > 0);
}

BOOST_AUTO_TEST_CASE(deque_shrink_to_fit)
{
  Deque< Point > d;
  d.shrinkToFit();
  BOOST_TEST(d.capacity() == 0);

  d.pushFront(Point(0, 0));
  d.shrinkToFit();
  BOOST_TEST(d.capacity() == d.size());

  d.pushBack(Point(0, 0));
  d.pushFront(Point(0, 0));
  d.shrinkToFit();
  d.shrinkToFit();
  BOOST_TEST(d.capacity() == d.size());
}

BOOST_AUTO_TEST_CASE(deque_clear)
{
  Deque< Point > d;
  d.clear();
  BOOST_TEST(d.empty());
  BOOST_TEST(d.capacity() == 0);

  d.pushBack(Point(0, 0));
  d.clear();
  BOOST_TEST(d.empty());
  BOOST_TEST(d.capacity() > 0);

  d.pushFront(Point(0, 0));
  d.clear();
  BOOST_TEST(d.empty());
  BOOST_TEST(d.capacity() > 0);

  d.pushBack(Point(0, 0));
  d.pushFront(Point(0, 0));
  d.clear();
  d.clear();
  BOOST_TEST(d.empty());
}

BOOST_AUTO_TEST_CASE(deque_push)
{
  Deque< Point > polygon;
  Point point(0, 3);

  polygon.pushBack(point);
  BOOST_TEST(polygon.front() == point);
  BOOST_TEST(polygon.back() == point);

  polygon.clear();
  polygon.pushFront(point);
  BOOST_TEST(polygon.front() == point);
  BOOST_TEST(polygon.back() == point);

  polygon.pushFront(Point(-2, 2));
  BOOST_TEST(polygon.front() == Point(-2, 2));
  BOOST_TEST(polygon.back() == point);

  polygon.pushBack(Point(5, -1));
  BOOST_TEST(polygon.front() == Point(-2, 2));
  BOOST_TEST(polygon.back() == Point(5, -1));
}

BOOST_AUTO_TEST_CASE(deque_emplace)
{
  Deque< Point > polygon;
  Point point(-1, -2);

  polygon.emplaceBack(point);
  BOOST_TEST(polygon.front() == point);
  BOOST_TEST(polygon.back() == point);

  polygon.clear();
  polygon.emplaceFront(point);
  BOOST_TEST(polygon.front() == point);
  BOOST_TEST(polygon.back() == point);

  polygon.emplaceBack(Point(-3, -4));
  BOOST_TEST(polygon.front() == point);
  BOOST_TEST(polygon.back() == Point(-3, -4));

  polygon.emplaceFront(0, 6);
  BOOST_TEST(polygon.front() == Point(0, 6));
  BOOST_TEST(polygon.back() == Point(-3, -4));
}

BOOST_AUTO_TEST_CASE(deque_capacity_full)
{
  using aleksandrov::minDequeCapacity;

  Deque< char > d;
  for (size_t i = 0; i < minDequeCapacity; ++i)
  {
    d.pushBack(0);
  }
  BOOST_TEST(d.size() == d.capacity());

  d.pushBack(1);
  BOOST_TEST(d.size() == minDequeCapacity + 1);
  BOOST_TEST(d.capacity() >= d.size());

  d.popBack();
  BOOST_TEST(d.size() != d.capacity());
  BOOST_TEST(d.capacity() > minDequeCapacity);

  d.clear();
  BOOST_TEST(d.empty());
  BOOST_TEST(d.capacity() > minDequeCapacity);

  Deque< char > other;
  BOOST_TEST(other.capacity() < d.capacity());
}

BOOST_AUTO_TEST_CASE(deque_pop)
{
  using aleksandrov::minDequeCapacity;

  Deque< Point > d;
  d.pushBack(Point(0, 0));
  d.popBack();
  BOOST_TEST(d.empty());
  BOOST_TEST(d.capacity() > 0);

  d.pushBack(Point(0, 0));
  d.popFront();
  BOOST_TEST(d.empty());
  BOOST_TEST(d.capacity() > 0);

  d.pushFront(Point(0, 0));
  d.popBack();
  BOOST_TEST(d.empty());
  BOOST_TEST(d.capacity() > 0);

  d.pushFront(Point(0, 0));
  d.popFront();
  BOOST_TEST(d.empty());
  BOOST_TEST(d.capacity() > 0);

  d.pushBack(Point(1, 1));
  d.pushFront(Point(2, 2));
  d.popBack();
  BOOST_TEST(d.size() == 1);
  BOOST_TEST(d.back() == Point(2, 2));

  d.popFront();
  BOOST_TEST(d.empty());

  d.pushBack(Point(1, 1));
  d.pushFront(Point(2, 2));
  d.popFront();
  BOOST_TEST(d.size() == 1);
  BOOST_TEST(d.front() == Point(1, 1));

  d.popBack();
  BOOST_TEST(d.empty());

  d.pushFront(Point(1, 1));
  d.pushBack(Point(2, 2));
  d.popFront();
  BOOST_TEST(d.size() == 1);
  BOOST_TEST(d.front() == Point(2, 2));

  d.popBack();
  BOOST_TEST(d.empty());

  d.pushFront(Point(1, 1));
  d.pushBack(Point(2, 2));
  d.popBack();
  BOOST_TEST(d.size() == 1);
  BOOST_TEST(d.back() == Point(1, 1));

  d.popFront();
  BOOST_TEST(d.empty());
}

BOOST_AUTO_TEST_CASE(deque_swap)
{
  Deque< float > d1;
  d1.pushBack(1.0f);
  Deque< float > d2;
  d2.pushBack(2.0f);
  d2.pushBack(3.0f);

  d1.swap(d2);
  BOOST_TEST(d1.front() == 2.0f);

  d1.swap(d1);
  BOOST_TEST(d1.front() == 2.0f);
}

BOOST_AUTO_TEST_CASE(deque_equal_operators)
{
  Deque< Point > d1;
  Deque< Point > d2;
  BOOST_CHECK(d1 == d2);

  d1.pushBack(Point(1, 1));
  d1.popFront();
  BOOST_CHECK(d1 == d2);

  d1.pushFront(Point(1, 1));
  BOOST_CHECK(d1 != d2);

  d2.pushBack(Point(1, 1));
  BOOST_CHECK(d2 == d1);

  d1.pushFront(Point(2, 2));
  d2.pushBack(Point(2, 2));
  BOOST_CHECK(d1 != d2);
}

