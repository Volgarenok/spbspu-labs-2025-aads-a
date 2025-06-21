#include "inputDict.hpp"

void finaev::inDictionaries(std::istream& in, mainDict& dict)
{
  std::string name = "";
  while (in)
  {
    in >> name;
    std::string data = "";
    int key = 0;
    finaev::AVLtree< int, std::string > temp;
    while (in && in.peek() != '\n')
    {
      in >> key >> data;
      temp.insert({ key, data });
    }
    dict.insert({ name, temp });
  }
  in.clear();
}
