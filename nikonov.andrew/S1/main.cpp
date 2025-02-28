#include <iostream>
#include "additional-utilities.hpp"

int main()
{
  using pair_t = std::pair< std::string, std::list< int > >;
  std::list< pair_t > * pList = nullptr;
  try
  {
    pList = nikonov::getPList(std::cin);
    nikonov::processPList(pList);
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what();
    delete pList;
    return 1;
  }
  delete pList;
}
