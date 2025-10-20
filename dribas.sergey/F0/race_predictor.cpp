#include "race_predictor.hpp"
#include <cmath>
#include <iomanip>
#include "streamGuard.hpp"

namespace
{
  void formatTime(double totalMinutes, char* buffer)
  {
    if (totalMinutes < 0) {
      buffer[0] = '-'; buffer[1] = '-'; buffer[2] = ':';
      buffer[3] = '-'; buffer[4] = '-'; buffer[5] = '\0';
      return;
    }
    long long totalSeconds = static_cast< long long >(totalMinutes * 60.0 + 0.5);
    long long minutes = totalSeconds / 60;
    long long seconds = totalSeconds % 60;
    long long m = minutes;
    int pos = 0;
    if (m >= 100) {
      long long div = 100;
      while (div) {
        buffer[pos++] = '0' + (m / div);
        m %= div;
        div /= 10;
      }
    } else {
      buffer[pos++] = '0' + (minutes / 10);
      buffer[pos++] = '0' + (minutes % 10);
    }
    buffer[pos++] = ':';
    buffer[pos++] = '0' + (seconds / 10);
    buffer[pos++] = '0' + (seconds % 10);
    buffer[pos] = '\0';
  }
}

dribas::RacePrediction dribas::predict_result(const dribas::AVLTree<time_t, workout>& workouts)
{
  RacePrediction prediction = { 0.0, 0.0, 0.0, 0.0 };
  size_t count = 0;
  double max_distance = 0.0;
  double min_pace = -1;
  for (auto it = workouts.begin(); it != workouts.end(); ++it) {
    const workout& w = it->second;
    if (w.distance > 0 && w.avgPaceMinPerKm > 0) {
      count++;
      if (w.distance > max_distance) max_distance = w.distance;
      if (w.avgPaceMinPerKm < min_pace) min_pace = w.avgPaceMinPerKm;
    }
  }

  if (count == 0) {
    return prediction;
  }

  prediction.fiveKm = min_pace * 1.06;
  prediction.tenKm = min_pace * 1.08;
  prediction.halfMarathon = (max_distance >= 10.0) ? min_pace * 1.12 : prediction.tenKm * 1.05;
  if (max_distance >= 21.0) {
    prediction.marathon = min_pace * 1.18;
  } else if (max_distance >= 10.0) {
    prediction.marathon = prediction.halfMarathon * 1.10;
  } else {
    prediction.marathon = prediction.halfMarathon * 1.15;
  }
  return prediction;
}

std::ostream& dribas::operator<<(std::ostream& os, const RacePrediction& prediction)
{
  StreamGuard guard(os);
  char timeBuffer[16];
  os << "=== Race Time Predictions ===\n";
  formatTime(prediction.fiveKm * 5.0, timeBuffer);
  os << "  5K:       " << timeBuffer << " (min:sec)\n";
  formatTime(prediction.tenKm * 10.0, timeBuffer);
  os << "  10K:      " << timeBuffer << " (min:sec)\n";
  formatTime(prediction.halfMarathon * 21.0975, timeBuffer);
  os << "  21.1K:    " << timeBuffer << " (min:sec)\n";
  formatTime(prediction.marathon * 42.195, timeBuffer);
  os << "  42.2K:    " << timeBuffer << " (min:sec)\n";
  return os;
}
