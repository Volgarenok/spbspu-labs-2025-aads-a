#ifndef CONVERTEXPRESSIONS_HPP
#define CONVERTEXPRESSIONS_HPP
#include <string>
#include <istream>
#include <stdexcept>
#include "operations.hpp"
#include "stack.hpp"
#include "queue.hpp"

namespace lebedev
{
  Queue< std::string > convertInfToPost(const std::string& expr);
  long long calculatePost(Queue< std::string >& post);
  Stack< std::string > inputExpressions(std::istream& input);
}
#endif
