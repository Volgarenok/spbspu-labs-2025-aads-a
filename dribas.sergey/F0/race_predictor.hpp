#ifndef RACE_PREDICTION_HPP
#define RACE_PREDICTION_HPP

#include <ostream>
#include <avlTree.hpp>

#include "workout.hpp"

namespace dribas
{
  struct RacePrediction
  {
    double fiveKm;
    double tenKm;
    double halfMarathon;
    double marathon;
  };
  RacePrediction predict_result(const AVLTree< time_t, workout >&);
  std::ostream& operator<<(std::ostream&, const RacePrediction&);
}

#endif
