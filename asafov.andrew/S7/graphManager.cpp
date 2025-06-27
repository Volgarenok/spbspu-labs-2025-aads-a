#include "GraphManager.h"
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
      while (start < line.size())
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
      if (parts.empty()) continue;

      const std::string& cmd = parts[0];
      if (cmd == "graphs")
      {
        for (const auto& kv: graphs)
          std::cout << kv.first << '\n';
      }
      else if (cmd == "vertexes" && parts.size() == 2)
      {
        auto it = graphs.find(parts[1]);
        if (it == graphs.end())
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }
        for (const auto& name: it->second.get_vertices())
          std::cout << name << '\n';
      }
      else if ((cmd == "outbound" || cmd == "inbound") && parts.size() == 3)
      {
        auto it = graphs.find(parts[1]);
        if (it == graphs.end() || !it->second.has_vertex(parts[2]))
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }
        auto edges = (cmd == "outbound") ? it->second.get_outbound(parts[2]) : it->second.get_inbound(parts[2]);

        for (const auto& kv: edges)
        {
          std::cout << kv.first;
          for (unsigned w: kv.second)
            std::cout << " " << w;
          std::cout << '\n';
        }
      }
      else if ((cmd == "bind" || cmd == "cut") && parts.size() == 5)
      {
        auto git = graphs.find(parts[1]);
        if (git == graphs.end() ||
          !git->second.has_vertex(parts[2]) ||
          !git->second.has_vertex(parts[3]))
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }
        unsigned w = static_cast< unsigned >(std::atoi(parts[4].c_str()));
        if (cmd == "bind")
        {
          git->second.add_edge(parts[2], parts[3], w);
        }
        else
        {
          if (!git->second.remove_edge(parts[2], parts[3], w))
            std::cout << "<INVALID COMMAND>\n";
        }
      }
      else if (cmd == "create")
      {
        if (parts.size() == 2)
        {
          if (graphs.count(parts[1]))
          {
            std::cout << "<INVALID COMMAND>\n";
          }
          else
          {
            graphs[parts[1]] = Graph();
          }
        }
        else if (parts.size() >= 3)
        {
          const std::string& name = parts[1];
          int k = std::atoi(parts[2].c_str());
          if (static_cast< int >(parts.size()) != 3 + k || graphs.count(name))
          {
            std::cout << "<INVALID COMMAND>\n";
          }
          else
          {
            Graph g;
            for (int i = 0; i < k; ++i)
              g.add_vertex(parts[3 + i]);
            graphs[name] = g;
          }
        }
        else
        {
          std::cout << "<INVALID COMMAND>\n";
        }
      }
      else if (cmd == "merge" && parts.size() == 4)
      {
        const std::string &newg = parts[1], &g1 = parts[2], &g2 = parts[3];
        if (graphs.count(newg) || !graphs.count(g1) || !graphs.count(g2))
        {
          std::cout << "<INVALID COMMAND>\n";
        }
        else
        {
          Graph g;
          g.merge_from(graphs[g1]);
          g.merge_from(graphs[g2]);
          graphs[newg] = g;
        }
      }
      else if (cmd == "extract")
      {
        if (parts.size() < 4)
        {
          std::cout << "<INVALID COMMAND>\n";
          continue;
        }
        const std::string &newg = parts[1], &oldg = parts[2];
        int k = std::atoi(parts[3].c_str());
        if (static_cast< int >(parts.size()) != 4 + k || graphs.count(newg) || !graphs.count(oldg))
        {
          std::cout << "<INVALID COMMAND>\n";
        }
        else
        {
          std::vector< std::string > subset(parts.begin() + 4, parts.end());
          Graph g = graphs[oldg].extract(subset);
          if (g.get_vertices().empty() && !subset.empty())
          {
            std::cout << "<INVALID COMMAND>\n";
          }
          else
          {
            graphs[newg] = g;
          }
        }
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
  }
}
