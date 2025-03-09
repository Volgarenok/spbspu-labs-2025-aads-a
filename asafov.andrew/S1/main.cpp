#include "sequencefunctions.h"

int main()
{
  sequence_list_t sequences = asafov::getSequences(std::cin);
  asafov::outputSequences(sequences, std::cout);
  return 0;
}
