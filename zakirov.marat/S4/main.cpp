#include <iostream>
#include <fstream>
#include <map>
#include "ProcessingDictionary.hpp"
#include "InputDictionaries.hpp"

int main(int argc, char * argv[])
{
  if (argc != 2)
  {
    std::cout << "<INVALID PARAMETERS>" << '\n';
    return 1;
  }

  std::ifstream in(argv[1]);
  std::map< std::string, std::map< long long, std::string > > dictionaries_list;
  zakirov::get_dictionaries(in, dictionaries_list);
  while (!std::cin.eof())
  {
    try
    {
      zakirov::execute_command(std::cin, std::cout, dictionaries_list);
    }
    catch(const std::exception & e)
    {
      std::cout << e.what() << '\n';
    }
  }
}
