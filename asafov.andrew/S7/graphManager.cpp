#include "graphManager.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

namespace asafov
{
  void GraphManager::read_from_file(const std::string& filename)
  {
    std::ifstream in(filename.c_str());
    std::string line;

    while (std::getline(in, line))
    {
      if (line.empty()) continue;

      size_t pos = line.find(' ');
      if (pos == std::string::npos) continue;

      std::string name = line.substr(0, pos);
      int count = std::atoi(line.substr(pos + 1).c_str());

      Graph g;
      for (int i = 0; i < count; ++i)
      {
        std::getline(in, line);
        if (line.empty())
        {
          --i;
          continue;
        }

        size_t p1 = line.find(' ');
        size_t p2 = line.find(' ', p1 + 1);
        std::string from = line.substr(0, p1);
        std::string to = line.substr(p1 + 1, p2 - p1 - 1);
        unsigned weight = static_cast< unsigned >(std::atoi(line.substr(p2 + 1).c_str()));
        g.add_edge(from, to, weight);
      }

      graphs[name] = g;
    }
  }

  void GraphManager::process_commands()
  {
    std::string line;
    while (std::getline(std::cin, line))
    {
      if (line.empty()) continue;

      std::vector< std::string > parts;
      size_t start = 0;
      while (true)
      {
        size_t space = line.find(' ', start);
        if (space == std::string::npos)
        {
          parts.push_back(line.substr(start));
          break;
        }
        parts.push_back(line.substr(start, space - start));
        start = space + 1;
      }

      if (parts[0] == "graphs")
      {
        std::vector< std::string > names;
        for (std::unordered_map< std::string, Graph >::iterator it = graphs.begin(); it != graphs.end(); ++it)
          names.push_back(it->first);
        for (size_t i = 0; i < names.size(); ++i)
        {
          for (size_t j = i + 1; j < names.size(); ++j)
          {
            if (names[j] < names[i])
            {
              std::string tmp = names[i];
              names[i] = names[j];
              names[j] = tmp;
            }
          }
        }
        for (size_t i = 0; i < names.size(); ++i)
          std::cout << names[i] << '\n';
      }
      else if (parts[0] == "vertexes" && parts.size() == 2)
      {
        if (graphs.find(parts[1]) == graphs.end())
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }
        std::vector< std::string > v = graphs[parts[1]].get_vertices();
        if (v.empty())
        {
          std::cout << '\n';
          continue;
        }
        for (size_t i = 0; i < v.size(); ++i)
          std::cout << v[i] << '\n';
      }
      else if (parts[0] == "outbound" && parts.size() == 3)
      {
        if (graphs.find(parts[1]) == graphs.end() || !graphs[parts[1]].has_vertex(parts[2]))
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }
        std::unordered_map< std::string, std::vector< unsigned > > m = graphs[parts[1]].get_outbound(parts[2]);
        std::vector< std::string > keys;
        for (std::unordered_map< std::string, std::vector< unsigned > >::iterator it = m.begin(); it != m.end(); ++it)
          keys.push_back(it->first);
        for (size_t i = 0; i < keys.size(); ++i)
        {
          for (size_t j = i + 1; j < keys.size(); ++j)
          {
            if (keys[j] < keys[i])
            {
              std::string tmp = keys[i];
              keys[i] = keys[j];
              keys[j] = tmp;
            }
          }
        }
        for (size_t i = 0; i < keys.size(); ++i)
        {
          std::cout << keys[i];
          const std::vector< unsigned >& weights = m[keys[i]];
          for (size_t j = 0; j < weights.size(); ++j)
            std::cout << " " << weights[j];
          std::cout << '\n';
        }
        std::cout << '\n';
      }
      else if (parts[0] == "inbound" && parts.size() == 3)
      {
        if (graphs.find(parts[1]) == graphs.end() || !graphs[parts[1]].has_vertex(parts[2]))
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }
        std::unordered_map< std::string, std::vector< unsigned > > m = graphs[parts[1]].get_inbound(parts[2]);
        std::vector< std::string > keys;
        for (std::unordered_map< std::string, std::vector< unsigned > >::iterator it = m.begin(); it != m.end(); ++it)
          keys.push_back(it->first);
        for (size_t i = 0; i < keys.size(); ++i)
        {
          for (size_t j = i + 1; j < keys.size(); ++j)
          {
            if (keys[j] < keys[i])
            {
              std::string tmp = keys[i];
              keys[i] = keys[j];
              keys[j] = tmp;
            }
          }
        }
        for (size_t i = 0; i < keys.size(); ++i)
        {
          std::cout << keys[i];
          const std::vector< unsigned >& weights = m[keys[i]];
          for (size_t j = 0; j < weights.size(); ++j)
            std::cout << " " << weights[j];
          std::cout << '\n';
        }
        std::cout << '\n';
      }
      else if (parts[0] == "bind" && parts.size() == 5)
      {
        if (graphs.find(parts[1]) == graphs.end())
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }
        unsigned w = static_cast< unsigned >(std::atoi(parts[4].c_str()));
        graphs[parts[1]].add_edge(parts[2], parts[3], w);
      }
      else if (parts[0] == "cut" && parts.size() == 5)
      {
        if (graphs.find(parts[1]) == graphs.end() || !graphs[parts[1]].has_vertex(parts[2]) ||
          !graphs[parts[1]].has_vertex(parts[3]) ||
          !graphs[parts[1]].remove_edge(parts[2], parts[3], static_cast< unsigned >(std::atoi(parts[4].c_str()))))
        {
          std::cout << "<INVALID COMMAND>\n";
        }
      }
      else if (parts[0] == "create")
      {
        if (parts.size() == 2)
        {
          if (graphs.find(parts[1]) != graphs.end())
          {
            std::cout << "<INVALID COMMAND>\n";
            continue;
          }
          graphs[parts[1]] = Graph();
        }
        else if (parts.size() >= 3)
        {
          std::string name = parts[1];
          int k = std::atoi(parts[2].c_str());
          if (static_cast< int >(parts.size()) != 3 + k || graphs.find(name) != graphs.end())
          {
            std::cout << "<INVALID COMMAND>\n";
            continue;
          }
          Graph g;
          for (int i = 0; i < k; ++i)
          {
            g.add_vertex(parts[3 + i]);
          }
          graphs[name] = g;
        }
        else
        {
          std::cout << "<INVALID COMMAND>\n";
        }
      }
      else if (parts[0] == "merge" && parts.size() == 4)
      {
        std::string newg = parts[1];
        std::string g1 = parts[2];
        std::string g2 = parts[3];
        if (graphs.find(newg) != graphs.end() || graphs.find(g1) == graphs.end() || graphs.find(g2) == graphs.end())
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }
        Graph g;
        g.merge_from(graphs[g1]);
        g.merge_from(graphs[g2]);
        graphs[newg] = g;
      }
      else if (parts[0] == "extract")
      {
        if (parts.size() < 4)
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }
        std::string newg = parts[1];
        std::string oldg = parts[2];
        int k = std::atoi(parts[3].c_str());
        if (static_cast< int >(parts.size()) != 4 + k || graphs.find(newg) != graphs.end() || graphs.find(oldg) ==
          graphs.end())
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }
        std::vector< std::string > subset;
        for (int i = 0; i < k; ++i)
          subset.push_back(parts[4 + i]);
        Graph g = graphs[oldg].extract(subset);
        if (g.get_vertices().empty() && !subset.empty())
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }
        graphs[newg] = g;
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
  }
}
