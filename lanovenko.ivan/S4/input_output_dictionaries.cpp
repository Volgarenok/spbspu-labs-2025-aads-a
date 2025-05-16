#include "input_output_dictionaries.hpp"

void lanovenko::getDictionaries(std::istream &in, map_t &dictionaries)
{
  std::string name, value;
  int key = 0;
  while (!in.eof())
  {
    std::map< int, std::string > dictionary{};
    in >> name;
    while (in && in.peek() != '\n')
    {
      in >> key >> value;
      dictionary.insert({ key, value });
    }
    dictionaries.insert({ name, dictionary });
  }
  std::cin.clear();
}

void lanovenko::printDictionary(map_t &dictionaries)
{
  std::string name = "";
  std::cin >> name;
  const std::map< int, std::string > dictionary = dictionaries.at(name);
  if (dictionary.empty())
  {
    std::cout << "<EMPTY>" << "\n";
    return;
  }
  std::cout << name;
  for (const auto& pair : dictionary)
  {
    std::cout << ' ' << pair.first << ' ' << pair.second;
  }
  std::cout << '\n';
}
