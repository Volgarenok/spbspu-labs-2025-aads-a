#include <boost/test/unit_test.hpp>
#include "deque.hpp"

using aleksandrov::Deque;

namespace
{
  struct Point
  {
    Point(int x, int y):
      x(x),
      y(y)
    {}
    bool operator==(const Point& rhs) const
    {
      return x == rhs.x && y == rhs.y;
    }
    int x;
    int y;
  };

  std::ostream& operator<<(std::ostream& out, const Point& rhs)
  {
    return out << '(' << rhs.x << ';' << rhs.y << ')';
  }
}

BOOST_AUTO_TEST_CASE(deque_construction)
{
  Deque< Point > d1;
  BOOST_TEST(d1.empty());
  BOOST_TEST(d1.size() == 0);

  Deque< Point > d2(d1);
  BOOST_TEST(d2.empty());
  BOOST_TEST(d2.size() == 0);

  d2.pushBack(Point(1, 2));
  d2.pushBack(Point(3, 4));

  Deque< Point > d3(d2);
  BOOST_TEST(d3.size() == 2);
  BOOST_TEST(d3.front() == Point(1, 2));

  Deque< Point > d4(std::move(d3));
  BOOST_TEST(d4.size() == 2);
  BOOST_TEST(d4.front() == Point(1, 2));
  BOOST_TEST(d3.empty());
}

BOOST_AUTO_TEST_CASE(deque_copy_move)
{
  Deque< Point > d1;
  Deque< Point > d2;
  Deque< Point > d3;
  d1.pushBack(Point(2, 4));
  d1.pushBack(Point(6, 8));

  d2 = d1;
  BOOST_TEST(d2.size() == 2);
  BOOST_TEST(d2.front() == Point(2, 4));

  d3 = std::move(d2);
  BOOST_TEST(d3.size() == 2);
  BOOST_TEST(d3.front() == Point(2, 4));
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

BOOST_AUTO_TEST_CASE(deque_empty_size)
{
  Deque< char > d;
  BOOST_TEST(d.empty());
  BOOST_TEST(d.size() == 0);

  d.pushBack('a');
  BOOST_TEST(!d.empty());
  BOOST_TEST(d.size() == 1);

  d.popBack();
  BOOST_TEST(d.empty());
  BOOST_TEST(d.size() == 0);

  d.clear();
  BOOST_TEST(d.empty());
  BOOST_TEST(d.size() == 0);
}

BOOST_AUTO_TEST_CASE(deque_clear)
{
  Deque< Point > d;
  d.clear();
  d.pushBack(Point(0, 0));

  d.clear();
  d.clear();
  BOOST_TEST(d.empty());

  d.pushBack(Point(0, 0));
  d.pushBack(Point(0, 0));

  d.clear();
  BOOST_TEST(d.empty());
}

BOOST_AUTO_TEST_CASE(deque_push_emplace)
{
  Deque< Point > polygon;
  Point a(-1.5, -2.0);
  Point b(1.0, 3.5);
  Point c(-2.5, 5.0);

  polygon.pushBack(a);
  BOOST_TEST(polygon.back() == a);
  BOOST_TEST(polygon.size() == 1);

  polygon.pushFront(b);
  BOOST_TEST(polygon.front() == b);
  BOOST_TEST(polygon.back() == a);
  BOOST_TEST(polygon.size() == 2);

  polygon.emplaceBack(-1.5, -2.0);
  BOOST_TEST(polygon.back() == a);
  BOOST_TEST(polygon.front() == b);
  BOOST_TEST(polygon.size() == 3);

  polygon.emplaceFront(-2.5, 5.0);
  BOOST_TEST(polygon.front() == c);
  BOOST_TEST(polygon.back() == a);
  BOOST_TEST(polygon.size() == 4);

  polygon.pushBack(Point(1.5, -1.5));
  BOOST_TEST(polygon.front() == c);

  polygon.pushFront(Point(-1.5, -2.0));
  BOOST_TEST(polygon.front() == a);

  polygon.emplaceBack(-0.5, 4.5);
  BOOST_TEST(polygon.front() == a);

  polygon.emplaceFront(1.0, 3.5);
  BOOST_TEST(polygon.front() == b);
}

BOOST_AUTO_TEST_CASE(deque_capacity_full)
{
  using aleksandrov::minDequeCapacity;

  Deque< char > d;
  for (size_t i = 0; i < minDequeCapacity + 1; ++i)
  {
    d.pushBack(1);
  }
  BOOST_TEST(d.size() == minDequeCapacity + 1);
  BOOST_TEST(d.capacity() >= minDequeCapacity + 1);

  d.clear();
  BOOST_TEST(d.empty());
  BOOST_TEST(d.capacity() >= minDequeCapacity + 1);

  Deque< char > h;
  BOOST_TEST(h.capacity() < d.capacity());

  Deque< size_t > q;
  for (size_t i = 0; i < minDequeCapacity - 1; ++i)
  {
    q.pushBack(i);
  }
  for (size_t i = 0; i < 5; ++i)
  {
    q.popFront();
  }
  for (size_t i = 70; i < 75; ++i)
  {
    q.pushFront(i);
  }
  q.pushBack(76);
  BOOST_TEST(q.size() == minDequeCapacity);
  BOOST_TEST(q.capacity() >= minDequeCapacity);
  BOOST_TEST(q.front() == 74);
  BOOST_TEST(q.back() == 76);
}

BOOST_AUTO_TEST_CASE(deque_pop)
{
  using aleksandrov::minDequeCapacity;

  Deque< Point > d;
  d.pushBack(Point(1, 2));
  d.pushBack(Point(3, 4));

  d.popBack();
  BOOST_TEST(d.size() == 1);
  BOOST_TEST(d.back() == Point(1, 2));

  d.clear();
  d.pushBack(Point(1, 2));
  d.pushBack(Point(3, 4));

  d.popFront();
  BOOST_TEST(d.size() == 1);
  BOOST_TEST(d.front() == Point(3, 4));

  d.pushBack(Point(1, 2));
  d.popFront();
  BOOST_TEST(d.size() == 1);
  BOOST_TEST(d.back() == Point(1, 2));

  Deque< size_t > q;
  for (size_t i = 0; i < minDequeCapacity - 1; ++i)
  {
    q.pushBack(i);
  }
  for (size_t i = 0; i < 5; ++i)
  {
    q.popFront();
  }
  for (size_t i = 70; i < 75; ++i)
  {
    q.pushFront(i);
  }
  q.pushBack(76);
  BOOST_TEST(q.front() == 74);
  BOOST_TEST(q.back() == 76);
  BOOST_TEST(q.size() == minDequeCapacity);
  BOOST_TEST(q.capacity() >= minDequeCapacity);
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

