#include "parse_text_and_push_it_to_queue.hpp"
#include <iostream>

void petrov::parseTextAndPushItToQueue(std::string & text, std::queue< std::string > & queue)
{
  std::string string_for_queue;
  for (auto it = text.cbegin(); it != text.cend(); ++it)
  {
    if (*it != ' ' && it != text.cend())
    {
      string_for_queue += *it;
    }
    else
    {
      queue.push(string_for_queue);
      string_for_queue.clear();
    }
  }
  queue.push(string_for_queue);
  queue.push("|");
}
