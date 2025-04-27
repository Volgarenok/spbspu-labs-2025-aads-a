#include <iostream>
#include <string>
#include "expression_transform.hpp"

int main()
{
  std::string str;
  while (std::getline(std::cin, str, '\n'))
  {
    try
    {
      int res = evstyunichev::result(str);
      std::cout << res << '\n';
    }
    catch (const std::logic_error &e)
    {
      std::cerr << e.what() << '\n';
      continue;
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
      return 2;
    }
  }
  return 0;
}
