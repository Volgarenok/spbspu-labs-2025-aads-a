#include <limits>
#include <iostream>
#include "graph_utils.hpp"

int main(int argc, char **argv)
{
  using namespace abramov;

  if (argc != 2)
  {
    std::cerr << "Wrong parameters\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "Incorrect filename\n";
    return 1;
  }
  GraphCollection collection{};
  readGraphs(input, collection);
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      if (command == "graphs")
      {
        collection.printAllGraphs(std::cout);
      }
      else if (command == "vertexes")
      {
        std::string name;
        std::cin >> name;
        printVertsSorted(collection, name, std::cout);
      }
      else if (command == "outbound")
      {
        std::string name;
        std::string vert;
        std::cin >> name >> vert;
        printOutVerts(collection, name, vert, std::cout);
      }
      else if (command == "inbound")
      {
        std::string name;
        std::string vert;
        std::cin >> name >> vert;
        printInVerts(collection, name, vert, std::cout);
      }
      else if (command == "bind")
      {
        std::string name;
        std::cin >> name;
        addEdgeToGraph(collection, name, std::cin);
      }
      else if (command == "cut")
      {
        std::string name;
        std::cin >> name;
        cutEdgeFromGraph(collection, name, std::cin);
      }
      else if (command == "create")
      {
        createGraph(collection, std::cin);
      }
      else if (command == "merge")
      {
        std::string name;
        std::cin >> name;
        mergeGraphs(collection, name, std::cin);
      }
      else if (command == "extract")
      {
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
    catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
