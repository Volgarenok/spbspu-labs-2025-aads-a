#include "countFunctions.h"
#include <stdexcept>

void asafov::str_to_queue(queue_t& queue, const std::string& str)
{
  size_t i = 0;
  while (i < str.size())
  {
    if (isspace(str[i]))
    {
      i++;
      continue;
    }

    if (str[i] == '(' || str[i] == ')' ||
        str[i] == '+' || str[i] == '-' ||
        str[i] == '*' || str[i] == '/' ||
        str[i] == '%')
    {
      queue.push(std::string(1, str[i]));
      i++;
    }
    else if (isdigit(str[i]))
    {
      size_t start = i;
      while (i < str.size() && isdigit(str[i]))
      {
        i++;
      }
      queue.push(str.substr(start, i - start));
    }
    else
    {
      throw std::logic_error("invalid character!");
    }
  }
}
