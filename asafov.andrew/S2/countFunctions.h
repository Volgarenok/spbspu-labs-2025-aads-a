#ifndef PARSING_FUNCTIONS_H
#define PARSING_FUNCTIONS_H

#include <string>
#include <stack>
#include <queue>

namespace asafov
{
  using stack_t = std::stack<std::string>;
  using queue_t = std::queue<std::string>;

  void str_to_queue(queue_t& queue, const std::string& string);
  void into_polish(queue_t& queue);
  double count(queue_t& queue);
}
#endif
