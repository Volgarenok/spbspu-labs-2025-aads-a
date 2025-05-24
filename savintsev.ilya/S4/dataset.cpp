#include "dataset.h"
#include <iostream>
#include <fstream>

namespace savintsev
{
  DatasetCollection load_dataset_from(const std::string & filename)
  {
    std::ifstream file(filename);

    DatasetCollection datasets;

    if (!file)
    {
      return datasets;
    }

    while (file)
    {
      std::string label;
      file >> label;
      Dataset values;
      int key;
      std::string value;
      while (file.peek() != '\n' && file)
      {
        if (file >> key >> value)
        {
          values[key] = value;
        }
        else
        {
          file.clear();
          break;
        }
      }
      datasets[label] = std::move(values);
    }
    return datasets;
  }

  void print_dataset(const DatasetCollection & datasets)
  {
    std::string label;
    std::cin >> label;
    const Dataset & values = datasets.at(label);
    if (values.empty())
    {
      std::cout << "<EMPTY>\n";
      return;
    }
    std::cout << label;
    for (auto it = values.begin(); it != values.end(); ++it)
    {
      std::cout << ' ' << it->first << ' ' << it->second;
    }
    std::cout << '\n';
  }

  void complement_datasets(DatasetCollection & datasets)
  {
    std::string newset, set1, set2;
    std::cin >> newset >> set1 >> set2;
    Dataset values = datasets.at(set1);
    for (auto it = datasets.at(set2).begin(); it != datasets.at(set2).end(); ++it)
    {
      if (values.count(it->first))
      {
        values.erase(it->first);
      }
      else
      {
        values[it->first] = it->second;
      }
    }
    datasets[newset] = std::move(values);
  }

  void intersect_datasets(DatasetCollection & datasets)
  {
    std::string newset, set1, set2;
    std::cin >> newset >> set1 >> set2;

    Dataset result;
    const Dataset & set2_ref = datasets.at(set2);

    for (auto it = datasets.at(set1).begin(); it != datasets.at(set1).end(); ++it)
    {
      if (set2_ref.count(it->first))
      {
        result[it->first] = it->second;
      }
    }

    datasets[newset] = std::move(result);
  }

  void union_datasets(DatasetCollection & datasets)
  {
    std::string newset, set1, set2;
    std::cin >> newset >> set1 >> set2;
    Dataset values = datasets.at(set1);
    for (auto it = datasets.at(set2).begin(); it != datasets.at(set2).end(); ++it)
    {
      if (!values.count(it->first))
      {
        values[it->first] = it->second;
      }
    }
    datasets[newset] = std::move(values);
  }
}
