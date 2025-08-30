#ifndef CALCEXPRS_HPP
#define CALCEXPRS_HPP
#include <string>
#include "queue.hpp"

namespace abramov
{
  long long int calcExpr(const std::string &s);
  long long int calcPostfix(abramov::Queue< std::string > *queue);
  Queue< std::string > *getPostfix(const std::string &s);
}
#endif
