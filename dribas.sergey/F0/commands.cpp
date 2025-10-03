#include <iomanip>
#include <fstream>
#include <cctype>
#include <cmath>
#include <limits>
#include <utility>

#include "commands.hpp"
#include "survival.hpp"
#include "rest_time.hpp"
#include "race_predictor.hpp"
#include "avlTree.hpp"

namespace
{
  bool compareStrings(const std::string& field_val, const std::string& op, const std::string& value_str)
  {
    if (op == "==") {
      return field_val == value_str;
    }
    if (op == "!=") {
      return field_val != value_str;
    }
    return false;
  }

  bool compareNumbers(double field_val, const std::string& op, double value)
  {
    if (op == "==") {
      return field_val == value;
    }
    if (op == "!=") {
      return field_val != value;
    }
    if (op == ">") {
      return field_val > value;
    }
    if (op == "<") {
      return field_val < value;
    }
    if (op == ">=") {
      return field_val >= value;
    }
    if (op == "<=") return field_val <= value;
    return false;
  }

  double getWorkoutParam(const dribas::workout& w, const std::string& param)
  {
    if (param == "avg_heart") {
      return w.avgHeart;
    }
    if (param == "max_heart") {
      return w.maxHeart;
    }
    if (param == "avg_cadence") {
      return w.cadence;
    }
    if (param == "distance") {
      return w.distance;
    }
    if (param == "avg_pace") {
      return w.avgPaceMinPerKm;
    }
    if (param == "start_time") {
      return w.timeStart;
    }
    if (param == "end_time") {
      return w.timeEnd;
    }
    return 0.0;
  }

  time_t parseDate(int year, int month, int day)
  {
    std::tm tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    return std::mktime(&tm);
  }
}

void dribas::add_training_from_file(std::istream& in, std::ostream& out, Suite& suite)
{
  std::string filename;
  in >> filename;
  std::ifstream file(filename);
  workout training;
  file >> training;
  suite[1].insert(std::make_pair(training.timeStart, training));
  out << "Training added: " << training.name << "\n";
}

void dribas::show_all_trainings(std::ostream& out, const Suite& suite)
{
  auto it = suite.find(1);
  if (it != suite.end()) {
    for (auto workout_it = it->second.begin(); workout_it != it->second.end(); ++workout_it) {
      out << workout_it->second << "\n";
    }
  } else {
    out << "No workouts found\n";
  }
}

void dribas::show_trainings_by_date(std::istream& in, std::ostream& out, const Suite& suite)
{
  int year, month, day, end_year, end_month, end_day;
  char dash;
  in >> year >> dash >> month >> dash >> day >> end_year >> dash >> end_month >> dash >> end_day;
  time_t start = parseDate(year, month, day);
  time_t end = parseDate(end_year, end_month, end_day) + 86400;
  auto it = suite.find(1);
  if (it != suite.end()) {
    size_t count = 0;
    for (auto workout_it = it->second.begin(); workout_it != it->second.end(); ++workout_it) {
      if (workout_it->first >= start && workout_it->first <= end) {
        out << workout_it->second << "\n";
        count++;
      }
    }
    out << "Found: " << count << " workouts\n";
  } else {
    out << "No workouts found\n";
  }
}

void dribas::query_trainings(std::istream& in, std::ostream& out, Suite& suite)
{
  std::string param, op, value;
  in >> param >> op >> value;
  size_t new_id = 1;
  for (auto it = suite.begin(); it != suite.end(); ++it) {
    if (it->first >= new_id) new_id = it->first + 1;
  }
  AVLTree< time_t, workout > filtered;
  auto source_it = suite.find(1);
  if (source_it != suite.end()) {
    for (auto it = source_it->second.begin(); it != source_it->second.end(); ++it) {
      const workout& w = it->second;
      bool match = false;
      if (param == "name") {
        match = compareStrings(w.name, op, value);
      } else {
        double num_val = getWorkoutParam(w, param);
        double num_value = std::stod(value);
        match = compareNumbers(num_val, op, num_value);
      }
      if (match) filtered.insert(std::make_pair(it->first, it->second));
    }
  }
  suite.insert(std::make_pair(new_id, filtered));
  out << "Query ID: " << new_id << "\n";
  out << "Found: " << filtered.size() << " workouts\n";
}

void dribas::calculate_average(std::istream& in, std::ostream& out, Suite& suite)
{
  std::string param;
  size_t id;
  in >> param >> id;
  auto it = suite.find(id);
  if (it != suite.end() && !it->second.empty()) {
    double sum = 0;
    size_t count = 0;
    for (auto workout_it = it->second.begin(); workout_it != it->second.end(); ++workout_it) {
      sum += getWorkoutParam(workout_it->second, param);
      count++;
    }
    out << "Average " << param << ": " << (sum / count) << "\n";
  } else {
    out << "Collection not found or empty\n";
  }
}

void dribas::add_training_manual(std::istream& in, std::ostream& out, Suite& suite)
{
  workout w;
  int year, month, day, hour, min;
  char dash, colon;
  out << "Name: ";
  in >> w.name;
  out << "Date (YYYY-MM-DD HH:MM): ";
  in >> year >> dash >> month >> dash >> day >> hour >> colon >> min;
  w.timeStart = parseDate(year, month, day) + hour * 3600 + min * 60;
  out << "Distance (km): ";
  in >> w.distance;
  int h, m, s;
  out << "Duration (HH:MM:SS): ";
  in >> h >> colon >> m >> colon >> s;
  w.timeEnd = w.timeStart + h * 3600 + m * 60 + s;
  out << "Cadence ";
  in >> w.cadence;
  out << "Avg heart: ";
  in >> w.avgHeart;
  w.maxHeart = w.avgHeart;
  if (w.distance > 0) {
    w.avgPaceMinPerKm = (w.timeEnd - w.timeStart) / 60.0 / w.distance;
  }
  suite[1].insert(std::make_pair(w.timeStart, w));
  out << "Workout added\n";
}

void dribas::survival_score(std::ostream& out, const Suite& suite)
{
  auto it = suite.find(1);
  if (it != suite.end()) {
    out << calculate_survival_score(it->second);
  } else {
    out << "No workouts found\n";
  }
}

void dribas::show_rest(std::istream& in, std::ostream& out, const Suite& suite)
{
  int year, month, day;
  char dash;
  in >> year >> dash >> month >> dash >> day;
  time_t date = parseDate(year, month, day);
  auto it = suite.find(1);
  if (it != suite.end()) {
    int total_recovery = 0;
    size_t count = 0;
    for (auto workout_it = it->second.begin(); workout_it != it->second.end(); ++workout_it) {
      if (workout_it->first >= date && workout_it->first < date + 86400) {
        total_recovery += 1;
        count++;
      }
    }
    out << "=== Recovery Report ===\n";
    out  << "Date: " << year << "-" << month << "-" << day << "\n";
    out << "Workouts: " << count << "\n";
    out << "Total recovery: " << total_recovery / 60 << "h " << total_recovery % 60 << "m\n";
    if (total_recovery < 1440) out << "Recommendation: Light activity\n";
    else if (total_recovery < 2880) out << "Recommendation: Rest day\n";
    else out << "Recommendation: Full rest for " << (total_recovery / 1440) << " days\n";
  } else {
    out << "No workouts found\n";
  }
}

void dribas::predict_result(std::ostream& out, const Suite& suite)
{
  auto it = suite.find(1);
  if (it != suite.end()) {
    out << predict_result(it->second);
  } else {
    out << "No workouts found\n";
  }
}

void dribas::analyze_training_segment(std::istream& in, std::ostream& out, const Suite& suite)
{
  size_t id;
  time_t start, end;
  in >> id >> start >> end;
  auto it = suite.find(id);
  if (it != suite.end()) {
    double hr_sum = 0, cad_sum = 0, dist_sum = 0;
    double hr_min = std::numeric_limits< double >::max();
    double hr_max = std::numeric_limits< double >::lowest();
    double cad_min = std::numeric_limits< double >::max();
    double cad_max = std::numeric_limits< double >::lowest();
    size_t count = 0;
    for (auto workout_it = it->second.begin(); workout_it != it->second.end(); ++workout_it) {
      if (workout_it->first >= start && workout_it->first <= end) {
        const workout& w = workout_it->second;
        hr_sum += w.avgHeart;
        cad_sum += w.cadence;
        dist_sum += w.distance;
        count++;
        if (w.avgHeart < hr_min) hr_min = w.avgHeart;
        if (w.avgHeart > hr_max) hr_max = w.avgHeart;
        if (w.cadence < cad_min) cad_min = w.cadence;
        if (w.cadence > cad_max) cad_max = w.cadence;
      }
    }
    if (count > 0) {
      out << "=== Segment Analysis ===\n";
      out<< "Workouts: " << count << "\n";
      out << "Avg heart: " << (hr_sum / count) << " bpm\n";
      out << "Min/Max heart: " << hr_min << "/" << hr_max << " bpm\n";
      out << "Avg cadence: " << (cad_sum / count) << " rpm\n";
      out << "Min/Max cadence: " << cad_min << "/" << cad_max << " rpm\n";
      out << "Avg distance: " << (dist_sum / count) << " km\n";
    } else {
      out << "No workouts in range\n";
    }
  } else {
    out << "Collection not found\n";
  }
}

void dribas::delete_training_by_key(std::istream& in, std::ostream& out, Suite& suite)
{
  int year, month, day, hour, min;
  char dash, colon;
  in >> year >> dash >> month >> dash >> day >> hour >> colon >> min;
  time_t key = parseDate(year, month, day) + hour * 3600 + min * 60;
  if (suite.find(1) != suite.end()) {
    if (suite[1].erase(key)) {
      out << "Workout deleted\n";
    } else {
      out << "Workout not found\n";
    }
  } else {
    out << "Main collection not found\n";
  }
}

void dribas::delete_training_suite(std::istream& in, std::ostream& out, Suite& suite)
{
  size_t id;
  in >> id;
  if (id == 1) {
    out << "Cannot delete main suite\n";
  } else if (suite.erase(id)) {
    out << "Suite " << id << " deleted\n";
  } else {
    out << "Suite not found\n";
  }
}
