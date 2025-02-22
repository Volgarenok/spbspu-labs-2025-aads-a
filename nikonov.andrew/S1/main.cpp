#include <iostream>
#include "additional-utilities.hpp"

int main()
{
  using pair_t = std::pair< std::string, std::list< int >* >;
  std::list< pair_t* >* pList = nikonov::getPList(std::cin);
  nikonov::processPList(pList);
}
