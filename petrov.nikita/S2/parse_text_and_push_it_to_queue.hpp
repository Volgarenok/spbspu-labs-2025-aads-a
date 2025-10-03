#ifndef PARSE_TEXT_AND_PUSH_IT_TO_QUEUE_HPP
#define PARSE_TEXT_AND_PUSH_IT_TO_QUEUE_HPP

#include <string>
#include <queue>

namespace petrov
{
  void parseTextAndPushItToQueue(std::string & text, std::queue< std::string > & queue);
}

#endif
