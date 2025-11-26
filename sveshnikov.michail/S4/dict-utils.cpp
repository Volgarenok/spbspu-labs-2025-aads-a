#include "dict-utils.hpp"
#include <fstream>

namespace
{
  void validateDict(std::string dict_name, const sveshnikov::DataTree_t &data)
  {
    if (data.find(dict_name) == data.cend())
    {
      throw std::out_of_range("<INVALID COMMAND>");
    }
  }
}

sveshnikov::DataTree_t sveshnikov::loadDicts(char *filename)
{
  std::ifstream in(filename);
  if (!in.is_open())
  {
    throw std::runtime_error("ERROR: Cannot open file!");
  }

  DataTree_t data;
  std::string name_dict;
  while (in >> name_dict)
  {
    Dict_t dict;
    while (in.peek() != '\n')
    {
      int key = 0;
      if (in >> key && in.peek() != '\n')
      {
        std::string value;
        in >> value;
        dict[key] = value;
      }
      else
      {
        throw std::invalid_argument("ERROR: Incorrect data format!");
      }
    }
    data[name_dict] = dict;
  }
  return data;
}

sveshnikov::CommandHolder_t sveshnikov::getCommands()
{
  CommandHolder_t command_holder;

  command_holder["print"] = printDict;
  command_holder["complement"] = complementDict;
  command_holder["intersect"] = intersectDict;
  command_holder["union"] = unionDict;

  return command_holder;
}

void sveshnikov::printDict(std::istream &in, const DataTree_t &data)
{
  std::string dataset;
  in >> dataset;
  validateDict(dataset, data);

  auto dict = data.find(dataset);
  if (dict->second.empty())
  {
    std::cout << "<EMPTY>";
    return;
  }
  std::cout << dataset;
  for (auto it = dict->second.cbegin(); it != dict->second.cend(); it++)
  {
    std::cout << " " << it->first << " " << it->second;
  }
}

void sveshnikov::complementDict(std::istream &in, DataTree_t &data)
{
  std::string newdataset, dataset_1, dataset_2;
  in >> newdataset >> dataset_1 >> dataset_2;
  validateDict(dataset_1, data);
  validateDict(dataset_2, data);

  auto dict1 = data.find(dataset_1);
  Dict_t new_dict = data.at(dataset_2);
  for (auto it = dict1->second.cbegin(); it != dict1->second.cend(); it++)
  {
    if (new_dict.find(it->first) == new_dict.end())
    {
      new_dict[it->first] = it->second;
    }
    else
    {
      new_dict.erase(it->first);
    }
  }
  data[newdataset] = new_dict;
}

void sveshnikov::intersectDict(std::istream &in, DataTree_t &data)
{
  std::string newdataset, dataset_1, dataset_2;
  in >> newdataset >> dataset_1 >> dataset_2;
  validateDict(dataset_1, data);
  validateDict(dataset_2, data);

  auto dict1 = data.find(dataset_1), dict2 = data.find(dataset_2);
  Dict_t new_dict;
  for (auto it = dict1->second.cbegin(); it != dict1->second.cend(); it++)
  {
    if (dict2->second.find(it->first) != dict2->second.end())
    {
      new_dict[it->first] = it->second;
    }
  }
  data[newdataset] = new_dict;
}

void sveshnikov::unionDict(std::istream &in, DataTree_t &data)
{
  std::string newdataset, dataset_1, dataset_2;
  in >> newdataset >> dataset_1 >> dataset_2;
  validateDict(dataset_1, data);
  validateDict(dataset_2, data);

  auto dict2 = data.find(dataset_2);
  Dict_t new_dict = data.at(dataset_1);
  for (auto it = dict2->second.cbegin(); it != dict2->second.cend(); it++)
  {
    if (new_dict.find(it->first) == new_dict.end())
    {
      new_dict[it->first] = it->second;
    }
  }
  data[newdataset] = new_dict;
}
