#ifndef IO_STACK_HPP
#define IO_STACK_HPP
#include <iostream>
#include <string>
#include "queue.hpp"
#include "stack.hpp"

namespace zakirov
{
  void scan_infix(std::istream & in, Stack< Queue < std::string > > & stack_infix);
  void output_results(std::ostream & out, Stack< double > & results);
}

#endif

