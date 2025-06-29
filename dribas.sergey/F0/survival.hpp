#ifndef SURVIVAL_HPP
#define SURVIVAL_HPP

#include <ostream>
#include <string>
#include "avlTree.hpp"
#include "workout.hpp"

namespace dribas
{
  struct WorkoutAccumulator
  {
    double totalDistance = 0.0;
    double totalDuration = 0.0;
    double strengthSum = 0.0;
    size_t workoutCount = 0;
  };

  WorkoutAccumulator accumulate_workout_data(WorkoutAccumulator, const std::pair<time_t, workout>&);

  struct SurvivalScore
  {
    double enduranceScore = 0.0;
    double strengthScore = 0.0;
    int estimatedSurvivalDays = 0;
    std::string recommendations[2];
    size_t recCount = 0;
  };

  class RecommendationGenerator
  {
  public:
    explicit RecommendationGenerator(const SurvivalScore& s) : score(s) {}
    std::string operator()(const std::pair<double, std::string>& rule) const;
  private:
    const SurvivalScore& score;
  };

  SurvivalScore calculate_survival_score(const AVLTree<time_t, workout>&);
  std::ostream& operator<<(std::ostream&, const SurvivalScore&);
}

#endif
