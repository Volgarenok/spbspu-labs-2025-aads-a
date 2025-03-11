#include <iostream>
#include "additional-utilities.hpp"

int main()
{
  using pair_t = std::pair< std::string, nikonov::List< size_t > >;
  nikonov::List< pair_t > * pList = nullptr;
  try
  {
    bool overflowFlag = false;
    pList = nikonov::getPList(std::cin, overflowFlag);
    nikonov::processPList(pList, overflowFlag);
  }
  catch (const std::bad_alloc & e)
  {
    std::cerr << e.what() << '\n';
  }
  catch (const std::overflow_error & e)
  {
    std::cerr << e.what() << '\n';
    delete pList;
    return 1;
  }
  delete pList;
}
