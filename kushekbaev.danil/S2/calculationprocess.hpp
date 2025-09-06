#ifndef CALCULATIONPROCESS_HPP
#define CALCULATIONPROCESS_HPP

#include <string>
#include "queue.hpp"

namespace kushekbaev
{
  Queue< std::string > convertToPostfix(Queue< std::string > Q);
  long long int calculatePostfix(Queue< std::string > postfixQ);
}

#endif
