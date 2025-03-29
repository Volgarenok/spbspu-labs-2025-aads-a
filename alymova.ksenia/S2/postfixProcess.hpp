#ifndef POSTFIX_PROCESS_HPP
#define POSTFIX_PROCESS_HPP
#include <string>
#include "queue.hpp"

namespace alymova
{
  Queue< std::string > convert_postfix(const std::string& s);
  long long int count_postfix(alymova::Queue< std::string >& queue);
  bool my_isdigit(const std::string& s);
  bool is_overflow_addition(long long int lhs, long long int rhs);
  bool is_overflow_substraction(long long int lhs, long long int rhs);
  bool is_overflow_multi(long long int lhs, long long int rhs);
  long long int my_mod(long long int item1, long long int item2);
  int sign(long long int item);
  bool same_sign(long long int item1, long long int item2);
  bool is_first_priority(const std::string& token);
  bool is_second_priority(const std::string& token);
  bool is_need_priority(const std::string& token, const std::string& top);
}
#endif
