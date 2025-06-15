#include <iostream>
#include <stdexcept>
#include <list/list.hpp>
#include "read_sequences.hpp"
#include "print_results.hpp"

int main()
{
  using namespace smirnov;
  try
  {
    List< std::pair< std::string, List< size_t > > > sequences;
    readSequences(std::cin, sequences);
    if (sequences.empty())
    {
      std::cout << 0 << "\n";
      return 0;
    }
    printName(std::cout, sequences);
    bool hasNumbers = false;
    for (auto seqIt = sequences.begin(); seqIt != sequences.end(); ++seqIt)
    {
      if (!seqIt->second.empty())
      {
        hasNumbers = true;
        break;
      }
    }
    if (!hasNumbers)
    {
      std::cout << 0 << "\n";
      return 0;
    }
    List< List< size_t > > resultSequences;
    newSequences(resultSequences, sequences);
    if (!resultSequences.empty())
    {
      printSequences(std::cout, resultSequences);
    }
    printSums(std::cout, resultSequences);
  }
  catch (const std::overflow_error & e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
