#ifndef CALC_POST_HPP
#define CALC_POST_HPP

#include "stack.hpp"
#include "queue.hpp"

namespace karnauhova
{
  long long int calculator(long long int first, long long int second, std::string operat);
  long long int procPost(Queue< std::string > post);
  long long int getSum(long long int first, long long int second);
  long long int getDifference(long long int first, long long int second);
  long long int getMultiplicate(long long int first, long long int second);
  long long int getDivisionMain(long long int first, long long int second);
  long long int getDivisionRemain(long long int first, long long int second);
}

#endif
