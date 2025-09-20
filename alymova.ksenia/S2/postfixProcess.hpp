#ifndef POSTFIX_PROCESS_HPP
#define POSTFIX_PROCESS_HPP
#include <string>
#include <queue.hpp>

namespace alymova
{
  bool isOverflowAddition(long long int lhs, long long int rhs);
  bool isOverflowSubstraction(long long int lhs, long long int rhs);
  bool isOverflowMulti(long long int lhs, long long int rhs);
  long long int mod(long long int item1, long long int item2);

  namespace detail
  {
    bool haveNotLessPriority(const std::string& token, const std::string& top);
    bool isOperation(const std::string& token);
  }
}
#endif
