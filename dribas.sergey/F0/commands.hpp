#ifndef CMDS_HPP
#define CMDS_HPP

#include "workout.hpp"

namespace dribas
{
  using Suite = AVLTree< size_t, AVLTree< time_t, dribas::workout > >;
  void add_training_manual(std::istream&, std::ostream&, Suite&);
  void add_training_from_file(std::istream&, std::ostream&, Suite&);
  void show_all_trainings(std::ostream&, const Suite&);
  void show_trainings_by_date(std::istream&, std::ostream&, const Suite&);
  void query_trainings(std::istream&, std::ostream&, Suite&);
  void calculate_average(std::istream&, std::ostream&, Suite&);
  void survival_score(std::ostream&, const Suite&);
  void show_rest(std::istream&, std::ostream&, const Suite&);
  void predict_result(std::ostream&, const Suite&);
  void analyze_training_segment(std::istream&, std::ostream&, const Suite&);
  void delete_training_by_key(std::istream&, std::ostream&, Suite&);
  void delete_training_suite(std::istream&, std::ostream&, Suite&);
}

#endif
