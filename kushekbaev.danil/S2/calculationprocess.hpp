#ifndef CALCULATIONPROCESS_HPP
#define CALCULATIONPROCESS_HPP

#include <string>
#include "queue.hpp"

namespace kushekbaev
{
  Queue< std::string > convertToPostfix(Queue< std::string >& queue);
  long long int calculatePostfix(Queue< std::string > postfixQueue);
}

#endif
