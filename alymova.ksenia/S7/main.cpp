#include <iostream>
#include <fstream>
#include <functional>
#include <limits>
#include "graph.hpp"
#include "graph-commands.hpp"
#include "hash-table.hpp"
#include "hash-functions.hpp"

int main(int argc, char** argv)
{
  using namespace alymova;
  using GraphSet = HashTable< std::string, Graph, Hasher< std::string > >;
  using CommandSet = HashTable< std::string, std::function< void(GraphSet&) >, Hasher< std::string > >;

  if (argc != 2)
  {
    std::cerr << "<INCORRECT ARGUMENTS>\n";
    return 1;
  }
  std::ifstream file;
  file.open(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "<INCORRECT FILE>\n";
    return 1;
  }

  try
  {
    GraphSet graphs = readGraphsFile(file);
    file.clear();

    CommandSet commands = complectCommands(std::cin, std::cout);
    std::string command;
    while (!(std::cin >> command).eof())
    {
      try
      {
        commands.at(command)(graphs);
        if (command == "graphs" || command == "vertexes" || command == "outbound" || command == "inbound")
        {
          std::cout << '\n';
        }
      }
      catch (const std::exception& e)
      {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        std::cout << "<INVALID COMMAND>\n";
      }
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
}

