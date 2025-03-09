#ifndef SEQUENCEFUNCTIOANS_H
#define SEQUENCEFUNCTIOANS_H
#include "sequenceusings.h"
#include <iostream>
namespace asafov
{
  sequence_list_t getSequences(std::istream& in);
  void outputSequences(sequence_list_t& sequences, std::ostream& out);
}
#endif
