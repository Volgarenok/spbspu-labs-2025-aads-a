#include <iostream>
#include "parsingFunctions.h"

int main()
{
  std::string str;
  std::cin >> str;
  str = asafov::into_polish(str);
  return 0;
}
