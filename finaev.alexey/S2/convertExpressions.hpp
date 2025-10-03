#ifndef CONVERTEXPRESSIONS_HPP
#define CONVERTEXPRESSIONS_HPP
#include <string>
#include <istream>
#include <queue.hpp>
#include <stack.hpp>
#include "mathOperations.hpp"

namespace finaev
{
  void countFinalResults(std::istream& in);
  Queue< std::string > fromInfToPost(finaev::Queue< std::string >& inf);
  long long calculatePost(finaev::Queue< std::string >& post);
}

#endif
