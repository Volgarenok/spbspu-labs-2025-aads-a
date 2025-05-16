#include "command_processing.hpp"

void lanovenko::merge(map_t& dictionaries)
{
  std::string new_dataset, dataset_1, dataset_2;
  std::cin >> new_dataset >> dataset_1 >> dataset_2;
  const std::map< int, std::string > lhs = dictionaries.at(dataset_1);
  const std::map< int, std::string > rhs = dictionaries.at(dataset_2);
  std::map< int, std::string > dataset_union{};
  dataset_union.insert(lhs.begin(), lhs.end());
  dataset_union.insert(rhs.begin(), rhs.end());
  dictionaries[new_dataset] = dataset_union;
}

void lanovenko::complement(map_t& dictionaries)
{
  std::string new_dataset, dataset_1, dataset_2;
  std::cin >> new_dataset >> dataset_1 >> dataset_2;
  const std::map< int, std::string > lhs = dictionaries.at(dataset_1);
  const std::map< int, std::string > rhs = dictionaries.at(dataset_2);
  std::map < int, std::string > dataset_complement{};
  for (const auto& pair : lhs)
  {
    if (rhs.find(pair.first) == rhs.end())
    {
      dataset_complement.insert(pair);
    }
  }
  dictionaries[new_dataset] = dataset_complement;
}

void lanovenko::intersect(map_t& dictionaries)
{
  std::string new_dataset, dataset_1, dataset_2;
  std::cin >> new_dataset >> dataset_1 >> dataset_2;
  const std::map< int, std::string > lhs = dictionaries.at(dataset_1);
  const std::map< int, std::string > rhs = dictionaries.at(dataset_2);
  std::map < int, std::string > dataset_intersect{};
  for (const auto& pair : lhs)
  {
    if (rhs.find(pair.first) != rhs.end())
    {
      dataset_intersect.insert(pair);
    }
  }
  dictionaries[new_dataset] = dataset_intersect;
}

lanovenko::CommandType lanovenko::parseCommand(const std::string &cmd)
{
  if (cmd == "print")
  {
    return CommandType::PRINT;
  }
  if (cmd == "complement")
  {
    return CommandType::COMPLEMENT;
  }
  if (cmd == "intersect")
  {
    return CommandType::INTERSECT;
  }
  if (cmd == "union")
  {
    return CommandType::UNION;
  }
  return CommandType::INVALID;
}
