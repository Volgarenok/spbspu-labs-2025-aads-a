#include <boost/test/unit_test.hpp>
#include "stack.hpp"

using aleksandrov::Stack;

namespace
{
  struct Bullet
  {
    Bullet(char p, double q):
      p(p),
      q(q)
    {}
    bool operator==(const Bullet& rhs) const
    {
      return p == rhs.p && q == rhs.q;
    }
    char p;
    double q;
  };

  std::ostream& operator<<(std::ostream& out, const Bullet& rhs)
  {
    return out << '[' << rhs.p << ':' << ' ' << rhs.q << ']';
  }
}

BOOST_AUTO_TEST_CASE(stack_construction)
{
  Stack< Bullet > rifle1;
  rifle1.push(Bullet('A', 2.0));
  rifle1.push(Bullet('B', 3.0));
  BOOST_TEST(rifle1.size() == 2);

  Stack< Bullet > rifle2(rifle1);
  BOOST_TEST(rifle2.size() == 2);
  BOOST_TEST(rifle2.top() == Bullet('B', 3.0));

  Stack< Bullet > rifle3(std::move(rifle1));
  BOOST_TEST(rifle3.size() == 2);
  BOOST_TEST(rifle3.top() == Bullet('B', 3.0));
}

BOOST_AUTO_TEST_CASE(stack_copy_move)
{
  Stack< Bullet > rifle1;
  Stack< Bullet > rifle2;
  Stack< Bullet > rifle3;
  rifle1.push(Bullet('A', 1.0));
  rifle1.push(Bullet('B', 2.0));

  rifle2 = rifle1;
  BOOST_TEST(rifle2.size() == 2);
  BOOST_TEST(rifle2.top() == Bullet('B', 2.0));

  rifle3 = std::move(rifle2);
  BOOST_TEST(rifle3.size() == 2);
  BOOST_TEST(rifle3.top() == Bullet('B', 2.0));
}

BOOST_AUTO_TEST_CASE(stack_top)
{
  Stack< int > shotgun;
  shotgun.push(1);
  BOOST_TEST(shotgun.top() == 1);

  shotgun.push(2);
  BOOST_TEST(shotgun.top() == 2);

  shotgun.pop();
  BOOST_TEST(shotgun.top() == 1);
}

BOOST_AUTO_TEST_CASE(stack_empty_size)
{
  Stack< char > cannon;
  BOOST_TEST(cannon.empty());
  BOOST_TEST(cannon.size() == 0);

  cannon.push('*');
  BOOST_TEST(!cannon.empty());
  BOOST_TEST(cannon.size() == 1);

  cannon.pop();
  BOOST_TEST(cannon.empty());
  BOOST_TEST(cannon.size() == 0);
}

BOOST_AUTO_TEST_CASE(stack_clear)
{
  Stack< Bullet > gun;
  gun.clear();
  BOOST_TEST(gun.empty());

  gun.push(Bullet('0', 0.0));
  gun.clear();
  BOOST_TEST(gun.empty());

  gun.push(Bullet('0', 0.0));
  gun.push(Bullet('0', 0.0));
  gun.clear();
  BOOST_TEST(gun.empty());
}

BOOST_AUTO_TEST_CASE(stack_push_emplace)
{
  Stack< Bullet > pistol;
  Bullet bullet('A', 1.0);

  pistol.push(bullet);
  BOOST_TEST(pistol.top() == bullet);

  pistol.pop();
  pistol.emplace('B', 2.0);
  BOOST_TEST(pistol.top() == Bullet('B', 2.0));

  pistol.push(Bullet('C', 3.0));
  BOOST_TEST(pistol.top() == Bullet('C', 3.0));

  pistol.emplace('D', 4.0);
  BOOST_TEST(pistol.top() == Bullet('D', 4.0));
}

BOOST_AUTO_TEST_CASE(stack_capacity_full)
{
  Stack< char > revolver;
  for (size_t i = 0; i < 65; ++i)
  {
    revolver.push(1);
  }
  BOOST_TEST(revolver.size() == 65);
  BOOST_TEST(revolver.capacity() >= 65);

  revolver.clear();
  BOOST_TEST(revolver.empty());
  BOOST_TEST(revolver.capacity() >= 65);

  Stack< char > other;
  BOOST_TEST(other.capacity() < revolver.capacity());
}

BOOST_AUTO_TEST_CASE(stack_pop)
{
  Stack< Bullet > gun;
  gun.push(Bullet('0', 0.0));
  gun.push(Bullet('0', 0.0));

  gun.pop();
  BOOST_TEST(gun.size() == 1);

  gun.pop();
  BOOST_TEST(gun.empty());
}

BOOST_AUTO_TEST_CASE(stack_swap)
{
  Stack< unsigned int > gun1;
  gun1.push(1);
  Stack< unsigned int > gun2;
  gun2.push(2);
  gun2.push(3);

  gun1.swap(gun2);
  BOOST_TEST(gun1.top() == 3);

  gun1.swap(gun1);
  BOOST_TEST(gun1.top() == 3);
}

