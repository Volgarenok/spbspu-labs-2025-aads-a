#include <limits>
#include "dict-utils.hpp"

int main(int argc, char **argv)
{
  using namespace sveshnikov;
  if (argc != 2)
  {
    std::cerr << "ERROR: the file is not specified!" << '\n';
    return 1;
  }

  CommandHolder_t comand_holder;
  DataTree_t dictionary_tree;
  try
  {
    comand_holder = getCommands();
    dictionary_tree = loadDicts(argv[1]);
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      comand_holder.at(command)(std::cin, dictionary_tree);
      if (command == "print")
      {
        std::cout << '\n';
      }
    }
    catch (const std::out_of_range &e)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (std::exception &e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  return 0;
}
