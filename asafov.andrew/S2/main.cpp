#include <iostream>
#include <iomanip>
#include "countFunctions.h"

int main()
{
  std::string str;
  std::getline(std::cin, str);
  asafov::queue_t queue;
  asafov::str_to_queue(queue, str);
  asafov::into_polish(queue);
  std::cout << std::fixed << std::setprecision(0) << asafov::count(queue) << '\n';
  return 0;
}
