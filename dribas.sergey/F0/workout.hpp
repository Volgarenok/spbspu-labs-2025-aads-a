#ifndef WORKOUT_HPP
#define WORKOUT_HPP

#include <string>
#include <ctime>
#include <istream>
#include <ostream>
#include <avlTree.hpp>

namespace dribas
{
  struct workout
  {
    std::string name;
    int avgHeart = 0;
    int maxHeart = 0;
    int cadence = 0;
    double distance = 0.0;
    double avgPaceMinPerKm = 0.0;
    time_t timeStart = 0;
    time_t timeEnd = 0;
  };

  struct training_suite
  {
    AVLTree< size_t, AVLTree< time_t, workout > > suite;
  };

  workout parseGpx(std::istream&);
  std::istream& operator>>(std::istream&, workout&);
  std::ostream& operator<<(std::ostream&, const workout&);
}

#endif
