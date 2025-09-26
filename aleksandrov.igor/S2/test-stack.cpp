#include <boost/test/unit_test.hpp>
#include "stack.hpp"

using aleksandrov::Stack;

namespace
{
  struct Bullet
  {
    char p;
    double q;

    Bullet() = delete;
    Bullet(char p, double q):
      p(p),
      q(q)
    {}

    bool operator==(const Bullet& rhs) const
    {
      return p == rhs.p && q == rhs.q;
    }
    bool operator!=(const Bullet& rhs) const
    {
      return !operator==(rhs);
    }
    friend std::ostream& operator<<(std::ostream& out, const Bullet& rhs)
    {
      return out << '[' << rhs.p << ':' << ' ' << rhs.q << ']';
    }
  };
}

BOOST_AUTO_TEST_CASE(stack_default_construction)
{
  using aleksandrov::minDequeCapacity;

  Stack< Bullet > rifle;
  BOOST_TEST(rifle.empty());
  BOOST_TEST(rifle.size() == 0);
  BOOST_TEST(rifle.maxSize() >= 0);
  BOOST_TEST(rifle.capacity() == 0);
}
BOOST_AUTO_TEST_CASE(copy_construction)
{
  Stack< Bullet > rifle;
  Stack< Bullet > rifleCopy(rifle);
  BOOST_TEST(rifleCopy.empty());
  BOOST_TEST(rifleCopy.size() == 0);
  BOOST_TEST(rifleCopy.capacity() == 0);

  Stack< Bullet > shotgun;
  shotgun.push(Bullet('A', 1.0));
  shotgun.push(Bullet('B', 2.0));
  Stack< Bullet > shotgunCopy(shotgun);
  BOOST_TEST(shotgunCopy.size() == 2);
  BOOST_TEST(shotgunCopy.capacity() == shotgun.capacity());
  BOOST_TEST(shotgunCopy.top() == Bullet('B', 2.0));
}

BOOST_AUTO_TEST_CASE(move_construction)
{
  Stack< Bullet > rifle;
  Stack< Bullet > rifleCopy(std::move(rifle));
  BOOST_TEST(rifleCopy.empty());
  BOOST_TEST(rifleCopy.size() == 0);
  BOOST_TEST(rifleCopy.capacity() == 0);

  Stack< Bullet > shotgun;
  shotgun.push(Bullet('A', 1.0));
  shotgun.push(Bullet('B', 2.0));
  Stack< Bullet > shotgunCopy(std::move(shotgun));
  BOOST_TEST(shotgunCopy.size() == 2);
  BOOST_TEST(shotgunCopy.top() == Bullet('B', 2.0));
  BOOST_TEST(shotgun.empty());
  BOOST_TEST(shotgun.capacity() == 0);
}

BOOST_AUTO_TEST_CASE(stack_copy_assignment)
{
  Stack< Bullet > rifle1;
  Stack< Bullet > rifle2;
  rifle2 = rifle1;
  BOOST_TEST(rifle2.empty());
  BOOST_TEST(rifle2.capacity() == 0);

  rifle1.push(Bullet('A', 1.0));
  rifle1.push(Bullet('B', 2.0));
  rifle2 = rifle1;
  BOOST_TEST(rifle2.size() == 2);
  BOOST_TEST(rifle2.capacity() == rifle1.capacity());
  BOOST_TEST(rifle2.top() == Bullet('B', 2.0));

  rifle1.clear();
  rifle2 = rifle1;
  BOOST_TEST(rifle2.empty());
  BOOST_TEST(rifle2.capacity() == rifle1.capacity());

  rifle1.push(Bullet('A', 1.0));
  rifle1.push(Bullet('B', 2.0));
  rifle2.push(Bullet('C', 3.0));
  rifle2.push(Bullet('D', 4.0));
  rifle2 = rifle1;
  BOOST_CHECK(rifle2 == rifle1);
}

BOOST_AUTO_TEST_CASE(stack_move_assignment)
{
  Stack< Bullet > rifle1;
  Stack< Bullet > rifle2;
  rifle2 = std::move(rifle1);
  BOOST_TEST(rifle2.empty());
  BOOST_TEST(rifle2.capacity() == 0);

  rifle1.push(Bullet('A', 1.0));
  rifle1.push(Bullet('B', 2.0));
  rifle2 = std::move(rifle1);
  BOOST_TEST(rifle2.size() == 2);
  BOOST_TEST(rifle2.top() == Bullet('B', 2.0));
  BOOST_TEST(rifle1.empty());
  BOOST_TEST(rifle1.capacity() == 0);

  rifle2 = std::move(rifle1);
  BOOST_TEST(rifle2.empty());
  BOOST_TEST(rifle2.capacity() == 0);

  rifle2 = std::move(rifle1);
  BOOST_CHECK(rifle2 == rifle1);
}

BOOST_AUTO_TEST_CASE(stack_top)
{
  Stack< int > rifle;
  rifle.push(1);
  BOOST_TEST(rifle.top() == 1);

  rifle.push(2);
  BOOST_TEST(rifle.top() == 2);

  rifle.pop();
  BOOST_TEST(rifle.top() == 1);

  rifle.top() = 2;
  BOOST_TEST(rifle.top() == 2);
}

BOOST_AUTO_TEST_CASE(stack_empty_size_capacity)
{
  Stack< char > cannon;
  BOOST_TEST(cannon.empty());
  BOOST_TEST(cannon.size() == 0);
  BOOST_TEST(cannon.capacity() == 0);

  cannon.push('*');
  BOOST_TEST(!cannon.empty());
  BOOST_TEST(cannon.size() == 1);
  BOOST_TEST(cannon.capacity() > 0);

  cannon.push('*');
  BOOST_TEST(cannon.size() == 2);

  cannon.clear();
  BOOST_TEST(cannon.empty());
  BOOST_TEST(cannon.size() == 0);
  BOOST_TEST(cannon.capacity() > 0);
}

BOOST_AUTO_TEST_CASE(stack_shrink_to_fit)
{
  Stack< Bullet > shotgun;
  shotgun.shrinkToFit();
  BOOST_TEST(shotgun.capacity() == 0);

  shotgun.push(Bullet('0', 0.0));
  shotgun.shrinkToFit();
  BOOST_TEST(shotgun.capacity() == shotgun.size());

  shotgun.push(Bullet('0', 0.0));
  shotgun.pop();
  shotgun.push(Bullet('0', 0.0));
  shotgun.shrinkToFit();
  shotgun.shrinkToFit();
  BOOST_TEST(shotgun.capacity() == shotgun.size());
}

BOOST_AUTO_TEST_CASE(stack_clear)
{
  Stack< Bullet > gun;
  gun.clear();
  BOOST_TEST(gun.empty());
  BOOST_TEST(gun.capacity() == 0);

  gun.push(Bullet('0', 0.0));
  gun.clear();
  BOOST_TEST(gun.empty());
  BOOST_TEST(gun.capacity() > 0);

  gun.push(Bullet('0', 0.0));
  gun.push(Bullet('0', 0.0));
  gun.clear();
  gun.clear();
  BOOST_TEST(gun.empty());
}

BOOST_AUTO_TEST_CASE(stack_push)
{
  Stack< Bullet > pistol;
  Bullet bullet('A', 1.0);

  pistol.push(bullet);
  BOOST_TEST(pistol.top() == bullet);

  pistol.push(Bullet('C', 3.0));
  BOOST_TEST(pistol.top() == Bullet('C', 3.0));
}

BOOST_AUTO_TEST_CASE(stack_emplace)
{
  Stack< Bullet > pistol;
  Bullet bullet('B', 7.0);

  pistol.emplace(bullet);
  BOOST_TEST(pistol.top() == bullet);

  pistol.emplace(Bullet('C', -3.0));
  BOOST_TEST(pistol.top() == Bullet('C', -3.0));

  pistol.emplace('D', 4.0);
  BOOST_TEST(pistol.top() == Bullet('D', 4.0));
}

BOOST_AUTO_TEST_CASE(stack_capacity_full)
{
  using aleksandrov::minDequeCapacity;

  Stack< int > revolver;
  for (size_t i = 0; i < minDequeCapacity; ++i)
  {
    revolver.push(0);
  }
  BOOST_TEST(revolver.size() == revolver.capacity());

  revolver.push(1);
  BOOST_TEST(revolver.size() == minDequeCapacity + 1);
  BOOST_TEST(revolver.capacity() >= revolver.size());

  revolver.pop();
  BOOST_TEST(revolver.size() != revolver.capacity());
  BOOST_TEST(revolver.capacity() > minDequeCapacity);

  revolver.clear();
  BOOST_TEST(revolver.empty());
  BOOST_TEST(revolver.capacity() > minDequeCapacity);

  Stack< char > other;
  BOOST_TEST(other.capacity() < revolver.capacity());
}

BOOST_AUTO_TEST_CASE(stack_pop)
{
  Stack< Bullet > gun;
  gun.push(Bullet('A', 1.0));
  gun.pop();
  BOOST_TEST(gun.empty());
  BOOST_TEST(gun.capacity() > 0);

  gun.push(Bullet('A', 1.0));
  gun.push(Bullet('B', 2.0));
  gun.pop();
  BOOST_TEST(gun.size() == 1);
  BOOST_TEST(gun.top() == Bullet('A', 1.0));

  gun.pop();
  BOOST_TEST(gun.empty());
}

BOOST_AUTO_TEST_CASE(stack_swap)
{
  Stack< int > gun1;
  Stack< int > gun2;
  gun1.swap(gun2);
  BOOST_CHECK(gun1 == gun2);

  gun1.push(1);
  gun2.push(2);
  gun2.push(3);
  gun2.swap(gun1);
  BOOST_TEST(gun1.top() == 3);

  gun1.swap(gun1);
  BOOST_TEST(gun1.top() == 3);
}

BOOST_AUTO_TEST_CASE(stack_equal_operators)
{
  Stack< Bullet > rifle1;
  Stack< Bullet > rifle2;
  BOOST_CHECK(rifle1 == rifle2);

  rifle1.push(Bullet('A', 1.0));
  rifle1.pop();
  BOOST_CHECK(rifle1 == rifle2);

  rifle1.push(Bullet('A', 1.0));
  BOOST_CHECK(rifle1 != rifle2);

  rifle2.push(Bullet('A', 1.0));
  BOOST_CHECK(rifle2 == rifle1);
}

