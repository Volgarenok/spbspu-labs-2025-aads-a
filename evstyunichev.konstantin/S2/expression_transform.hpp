#ifndef EXPRESSION_TRANSFORM_HPP
#define EXPRESSION_TRANSFORM_HPP
#include <string>
#include "../common/stack.hpp"
#include "../common/queue.hpp"
namespace evstyunichev
{
  struct Unit
  {
    int value;
    bool is_num;
  };

  int operand_priory(Unit &a);
  bool comp_operand(Unit &a, Unit &b);
  int result(std::string &s);
  Unit cast_to_unit(std::string &&s);
  void proceed(Unit &&a, Stack< Unit > &st, Queue< Unit > &qq);
  char utoc(Unit &a);
  int calculate(Unit &a, Unit &b, Unit &op);
}
#endif
