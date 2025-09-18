#include "sequencefunctions.h"

int main()
{
  asafov::sequence_list_t sequences;
  asafov::getSequences(sequences, std::cin);
  try
  {
    asafov::outputSequences(sequences, std::cout);
  }
  catch (const std::overflow_error&)
  {
    std::cerr << "owerlfow!\n";
    return 1;
  }
  return 0;
}
