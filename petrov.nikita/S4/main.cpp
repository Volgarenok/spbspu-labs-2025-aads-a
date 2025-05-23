#include <fstream>
#include <map>
#include <string>
#include <iostream>

namespace
{
  int checkArguments(const int argc);

  int checkArguments(const int argc)
  {
    if (argc == 2)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
}

int main(int argc, const char * const * argv)
{
  if (!checkArguments(argc))
  {
    std::cerr << "ERROR: Incorrect number of arguments ";
    std::cerr << "\n";
    return 1;
  }
  using subtree_t = std::map< int, std::string, std::less< int > >;
  using maintree_t = std::map< std::string, subtree_t >;
  maintree_t tree;
  std::ifstream input(argv[1]);
  while (!input.eof())
  {
    subtree_t subtree;
    std::string dataset;
    input >> dataset;
    if (input.eof())
    {
      break;
    }
    else if (dataset.empty())
    {
      continue;
    }
    std::pair< int, std::string > data;
    while (input)
    {
      input >> data.first;
      input >> data.second;
      if (!input)
      {
        input.clear();
        break;
      }
      subtree.insert({ data.first, data.second });
    }
    tree.insert({ dataset, subtree });
  }
  std::string command_name;
  while (!std::cin.eof())
  {
    std::cin >> command_name;
    if (std::cin.eof())
    {
      continue;
    }
    try
    {
      if (command_name == "print")
      {
        std::string dataset_parameter;
        std::cin >> dataset_parameter;
        auto it = tree.find(dataset_parameter);
        if (it == tree.end())
        {
          throw std::logic_error("<INVALID COMMAND>");
        }
        if (it->second.empty())
        {
          std::cout << "<EMPTY>";
          std::cout << "\n";
        }
        else
        {
          auto sub_it = it->second.cbegin();
          std::cout << it->first << " ";
          std::cout << sub_it->first << " " << (sub_it++)->second;
          for (; sub_it != it->second.cend(); ++sub_it)
          {
            std::cout << " " << sub_it->first << " " << sub_it->second;
          }
          std::cout << "\n";
        }
      }
      else if (command_name == "complement")
      {
        std::string new_dataset;
        std::string first_dataset;
        std::string second_dataset;
        std::cin >> new_dataset;
        std::cin >> first_dataset;
        std::cin >> second_dataset;
        auto first_it = tree.find(first_dataset);
        auto second_it = tree.find(second_dataset);
        if (first_it == tree.end() || second_it == tree.end())
        {
          throw std::logic_error("<INVALID COMMAND>");
        }
        auto first_sub_it = first_it->second.cbegin();
        auto second_sub_it = second_it->second.cbegin();
        subtree_t new_subtree;
        while (first_sub_it != first_it->second.cend() && second_sub_it != second_it->second.cend())
        {
          if (first_sub_it->first == second_sub_it->first)
          {
            ++first_sub_it;
            ++second_sub_it;
          }
          else if (first_sub_it->first < second_sub_it->first)
          {
            new_subtree.insert({ first_sub_it->first, first_sub_it->second });
            ++first_sub_it;
          }
          else
          {
            new_subtree.insert({ second_sub_it->first, second_sub_it->second });
            ++second_sub_it;
          }
        }
        if (first_sub_it != first_it->second.cend())
        {
          while (first_sub_it != first_it->second.cend())
          {
            new_subtree.insert({ first_sub_it->first, first_sub_it->second });
            ++first_sub_it;
          }
        }
        else if (second_sub_it != second_it->second.cend())
        {
          while (second_sub_it != second_it->second.cend())
          {
            new_subtree.insert({ second_sub_it->first, second_sub_it->second });
            ++second_sub_it;
          }
        }
        auto check_existance_it = tree.find(new_dataset);
        if (check_existance_it != tree.end())
        {
          tree.erase(new_dataset);
        }
        tree.insert({ new_dataset, new_subtree });
      }
      else if (command_name == "intersect")
      {
        std::string new_dataset;
        std::string first_dataset;
        std::string second_dataset;
        std::cin >> new_dataset;
        std::cin >> first_dataset;
        std::cin >> second_dataset;
        auto first_it = tree.find(first_dataset);
        auto second_it = tree.find(second_dataset);
        if (first_it == tree.end() || second_it == tree.end())
        {
          throw std::logic_error("<INVALID COMMAND>");
        }
        auto first_sub_it = first_it->second.cbegin();
        auto second_sub_it = second_it->second.cbegin();
        subtree_t new_subtree;
        while (first_sub_it != first_it->second.cend() && second_sub_it != second_it->second.cend())
        {
          if (first_sub_it->first == second_sub_it->first)
          {
            new_subtree.insert({ first_sub_it->first, first_sub_it->second });
            ++first_sub_it;
            ++second_sub_it;
          }
          else if (first_sub_it->first < second_sub_it->first)
          {
            ++first_sub_it;
          }
          else
          {
            ++second_sub_it;
          }
        }
        auto check_existance_it = tree.find(new_dataset);
        if (check_existance_it != tree.end())
        {
          tree.erase(new_dataset);
        }
        tree.insert({ new_dataset, new_subtree });
      }
      else if (command_name == "union")
      {
        std::string new_dataset;
        std::string first_dataset;
        std::string second_dataset;
        std::cin >> new_dataset;
        std::cin >> first_dataset;
        std::cin >> second_dataset;
        auto first_it = tree.find(first_dataset);
        auto second_it = tree.find(second_dataset);
        if (first_it == tree.end() || second_it == tree.end())
        {
          throw std::logic_error("<INVALID COMMAND>");
        }
        auto first_sub_it = first_it->second.cbegin();
        auto second_sub_it = second_it->second.cbegin();
        subtree_t new_subtree;
        while (first_sub_it != first_it->second.cend() && second_sub_it != second_it->second.cend())
        {
          if (first_sub_it->first == second_sub_it->first)
          {
            new_subtree.insert({ first_sub_it->first, first_sub_it->second });
            ++first_sub_it;
            ++second_sub_it;
          }
          else if (first_sub_it->first < second_sub_it->first)
          {
            new_subtree.insert({ first_sub_it->first, first_sub_it->second });
            ++first_sub_it;
          }
          else
          {
            new_subtree.insert({ second_sub_it->first, second_sub_it->second });
            ++second_sub_it;
          }
        }
        if (first_sub_it != first_it->second.cend())
        {
          while (first_sub_it != first_it->second.cend())
          {
            new_subtree.insert({ first_sub_it->first, first_sub_it->second });
            ++first_sub_it;
          }
        }
        else if (second_sub_it != second_it->second.cend())
        {
          while (second_sub_it != second_it->second.cend())
          {
            new_subtree.insert({ second_sub_it->first, second_sub_it->second });
            ++second_sub_it;
          }
        }
        auto check_existance_it = tree.find(new_dataset);
        if (check_existance_it != tree.end())
        {
          tree.erase(new_dataset);
        }
        tree.insert({ new_dataset, new_subtree });
      }
      else
      {
        std::cin.ignore(1024, '\n');
        std::cout << "<INVALID COMMAND>" << "\n";
      }
    }
    catch (const std::logic_error & e)
    {
      std::cout << e.what() << "\n";
    }
  }
}
