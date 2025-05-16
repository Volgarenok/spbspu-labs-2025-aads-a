#include <limits>
#include <algorithm>
#include "input_output_dictionaries.hpp"
#include "command_processing.hpp"

int main(int argc, char** argv)
{
  using namespace lanovenko;
  if (argc != 2)
  {
    std::cerr << "< INVALID PARAMETRS COUNT >\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  std::map< std::string, std::map< int, std::string > > dictionaries{};
  getDictionaries(input, dictionaries);
  std::string command = "";
  while (!(std::cin >> command).eof())
  {
    CommandType cmdType = parseCommand(command);
    try
    {
      switch (cmdType)
      {
      case CommandType::PRINT:
        printDictionary(dictionaries);
        break;
      case CommandType::COMPLEMENT:
        complement(dictionaries);
        break;
      case CommandType::INTERSECT:
        intersect(dictionaries);
        break;
      case CommandType::UNION:
        merge(dictionaries);
        break;
      case CommandType::INVALID:
        std::cout << "<INVALID COMMAND>\n";
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        break;
      default:
        break;
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
