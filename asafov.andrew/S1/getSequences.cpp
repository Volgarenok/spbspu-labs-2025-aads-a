#include "sequencefunctions.h"
#include <stdexcept>

void asafov::getSequences(sequence_list_t& sequences, std::istream& in = std::cin)
{
  do
  {
    std::string name;
    data_list_t list;
    data_t temp = 0;
    in >> name;
    while (!in.fail())
    {
      in >> temp;
      if (in.fail())
      {
        break;
      }
      list.push_back(temp);
    }
    if (!in.eof())
    {
      in.clear();
    }
    sequence_t pair = std::make_pair(name, list);
    sequences.push_back(pair);
  } while (!in.eof());
}
