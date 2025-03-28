#ifndef SEQUENCEFUNCTIOANS_H
#define SEQUENCEFUNCTIOANS_H
#include <iostream>
#include <string>
#include <utility>
#include "forward_list.hpp"
namespace asafov
{
  using data_t = size_t;
  using data_list_t = asafov::Forward_list<data_t>;
  using sequence_t = std::pair<std::string, data_list_t>;
  using sequence_list_t = asafov::Forward_list<sequence_t>;

  void getSequences(sequence_list_t& sequences, std::istream& in);
  void outputSequences(sequence_list_t& sequences, std::ostream& out);
}
#endif
