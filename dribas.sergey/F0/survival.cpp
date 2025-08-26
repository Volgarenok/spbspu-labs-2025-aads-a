#include "survival.hpp"
#include <iomanip>
#include <ostream>
#include "streamGuard.hpp"

namespace dribas
{
  WorkoutAccumulator accumulate_workout_data(WorkoutAccumulator acc, const std::pair< time_t, workout >& entry)
  {
    const workout& w = entry.second;
    acc.totalDistance += w.distance;
    acc.totalDuration += (w.timeEnd - w.timeStart) / 3600.0;
    acc.strengthSum += (w.maxHeart - w.avgHeart) + w.cadence;
    acc.workoutCount++;
    return acc;
  }
  std::string RecommendationGenerator::operator()(const std::pair< double, std::string >& rule) const
  {
    if (rule.first == 50.0 && score.enduranceScore < rule.first) {
      return rule.second;
    }
    if (rule.first == 30.0 && score.strengthScore < rule.first) {
      return rule.second;
    }
    return "";
  }

  SurvivalScore calculate_survival_score(const AVLTree< time_t, workout >& workouts)
  {
    SurvivalScore score;
    if (workouts.empty()) {
      return score;
    }

    WorkoutAccumulator totals;
    for (auto it = workouts.begin(); it != workouts.end(); ++it) {
      totals = accumulate_workout_data(totals, *it);
    }
    score.enduranceScore = (totals.totalDistance * totals.totalDuration) / totals.workoutCount;
    score.strengthScore = totals.strengthSum / totals.workoutCount;

    double result = (score.enduranceScore * 0.1) + (score.strengthScore * 0.05);
    result += (totals.totalDistance / totals.workoutCount * 0.2);
    score.estimatedSurvivalDays = static_cast< int >(result);

    const std::pair< double, std::string > rule1 = {50.0, "Increase long-distance workouts"};
    const std::pair< double, std::string > rule2 = {30.0, "Add interval training"};

    RecommendationGenerator generator{score};
    std::string rec1 = generator(rule1);
    if (!rec1.empty() && score.recCount < 2) {
        score.recommendations[score.recCount++] = rec1;
    }
    std::string rec2 = generator(rule2);
    if (!rec2.empty() && score.recCount < 2) {
        score.recommendations[score.recCount++] = rec2;
    }

    return score;
  }

  std::ostream& operator<<(std::ostream& os, const SurvivalScore& score)
  {
    dribas::StreamGuard guard(os);
    os << "=== Survival Analysis Report ===\n";
    os << "Estimated survival capacity: " << score.estimatedSurvivalDays << " days\n\n";
    os << "Key Metrics:\n";
    os << "- Endurance: " << std::fixed << std::setprecision(1) << score.enduranceScore << "\n";
    os << "- Strength:  " << score.strengthScore << "\n\n";
    if (score.recCount > 0) {
      os << "Recommendations:\n";
      for (size_t i = 0; i < score.recCount; i++) {
        os << score.recommendations[i] << "\n";
      }
    }
    return os;
  }
}
