#include "additional-utilities.hpp"

std::list< std::pair< std::string, std::list< int >* >* >* getPList(std::istream& in)
{
  using pair_t = std::pair< std::string, std::list< int >* >;
  std::list< pair_t* >* stdList = new std::list< pair_t* >;
  std::string name = "";
  std::string data = "";
  while (!in.eof())
  {
    std::list< int >* vals = new std::list< int >;
    in >> name;
    while (data != "\n")
    {
      in >> data;
      vals->push_back(std::stoi(data));
    }
    pair_t* Pair = new pair_t{ name, vals };
    stdList->push_back(Pair);
  }
  return stdList;
}
