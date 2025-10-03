#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <cstdlib>
#include "cuckoo-hash-map.h"
#include <two-three-tree.h>

namespace savintsev
{
  struct Graph
  {
    HashMap< std::string, HashMap< std::string, Array< int > > > edges;
    std::set< std::string > vertexes;
  };

  HashMap< std::string, Graph > graphs;

  Array< std::string > split(const std::string & line)
  {
    Array< std::string > result;
    size_t start = 0;
    while (start < line.length())
    {
      size_t space = line.find(' ', start);
      if (space == std::string::npos)
      {
        space = line.length();
      }
      result.push_back(line.substr(start, space - start));
      start = space + 1;
    }
    return result;
  }

  void cmd_graphs()
  {
    if (graphs.empty())
    {
      std::cout << "\n";
      return;
    }

    TwoThreeTree< std::string, Graph * > sorted;
    for (auto it = graphs.begin(); it != graphs.end(); ++it)
    {
      sorted[it->first] = std::addressof(it->second);
    }

    for (auto it = sorted.begin(); it != sorted.end(); ++it)
    {
      std::cout << it->first << '\n';
    }
  }

  void cmd_vertexes(const Array< std::string > & args)
  {
    if (args.size() < 2)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    const std::string & name = args[1];
    if (graphs.find(name) == graphs.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    if (graphs[name].vertexes.empty())
    {
      std::cout << "\n";
      return;
    }

    const std::set< std::string > & vertices = graphs[name].vertexes;
    for (auto it = vertices.begin(); it != vertices.end(); ++it)
    {
      std::cout << *it << '\n';
    }
  }

  void cmd_outbound(const Array< std::string > & args)
  {
    if (args.size() < 3)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    const std::string & g = args[1];
    const std::string & v = args[2];

    if (graphs.find(g) == graphs.end() || graphs[g].vertexes.find(v) == graphs[g].vertexes.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    TwoThreeTree< std::string, Array< int > > out;
    auto targets = graphs[g].edges.find(v);
    if (targets != graphs[g].edges.end())
    {
      for (auto jt = targets->second.begin(); jt != targets->second.end(); ++jt)
      {
        out[jt->first] = jt->second;
        std::sort(out[jt->first].begin(), out[jt->first].end());
      }
    }

    if (out.empty())
    {
      std::cout << "\n";
      return;
    }

    for (auto it = out.begin(); it != out.end(); ++it)
    {
      std::cout << it->first;
      for (Array< int >::iterator wt = it->second.begin(); wt != it->second.end(); ++wt)
      {
        std::cout << ' ' << *wt;
      }
      std::cout << '\n';
    }
  }

  void cmd_inbound(const Array< std::string > & args)
  {
    if (args.size() < 3)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    const std::string & g = args[1];
    const std::string & v = args[2];

    if (graphs.find(g) == graphs.end() || graphs[g].vertexes.find(v) == graphs[g].vertexes.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    TwoThreeTree< std::string, Array< int > > in;
    for (auto it = graphs[g].edges.begin(); it != graphs[g].edges.end(); ++it)
    {
      auto jt = it->second.find(v);
      if (jt != it->second.end())
      {
        in[it->first] = jt->second;
        std::sort(in[it->first].begin(), in[it->first].end());
      }
    }

    for (auto it = in.begin(); it != in.end(); ++it)
    {
      std::cout << it->first;
      for (Array< int >::iterator wt = it->second.begin(); wt != it->second.end(); ++wt)
      {
        std::cout << ' ' << *wt;
      }
      std::cout << '\n';
    }
  }

  void cmd_bind(const Array< std::string > & args)
  {
    if (args.size() < 5)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    const std::string & g = args[1];
    if (graphs.find(g) == graphs.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    const std::string & a = args[2];
    const std::string & b = args[3];
    int w = atoi(args[4].c_str());

    graphs[g].vertexes.insert(a);
    graphs[g].vertexes.insert(b);
    graphs[g].edges[a][b].push_back(w);
  }

  void cmd_cut(const Array< std::string > & args)
  {
    if (args.size() < 5)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    const std::string & g = args[1];
    const std::string & a = args[2];
    const std::string & b = args[3];
    int w = atoi(args[4].c_str());

    bool c1 = graphs.find(g) == graphs.end();
    bool c2 = graphs[g].edges.find(a) == graphs[g].edges.end();
    if (c1 || c2 || graphs[g].edges[a].find(b) == graphs[g].edges[a].end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    Array< int > & weights = graphs[g].edges[a][b];
    Array< int >::iterator it = std::find(weights.begin(), weights.end(), w);
    if (it == weights.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    weights.erase(it);
    if (weights.empty())
    {
      graphs[g].edges[a].erase(b);
    }
  }

  void cmd_create(const Array< std::string > & args)
  {
    if (args.size() < 2)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    const std::string & g = args[1];
    if (graphs.find(g) != graphs.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    Graph newg;
    for (size_t i = 3; i < args.size(); ++i)
    {
      newg.vertexes.insert(args[i]);
    }
    graphs[g] = newg;
  }


  void cmd_merge(const Array< std::string > & args)
  {
    if (args.size() < 4)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    const std::string & newg = args[1];
    const std::string & g1 = args[2];
    const std::string & g2 = args[3];

    if (graphs.find(newg) != graphs.end() || graphs.find(g1) == graphs.end() || graphs.find(g2) == graphs.end())
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    Graph g;
    const std::string gs[2] = {g1, g2};
    for (size_t i = 0; i < 2; ++i)
    {
      for (auto it = graphs[gs[i]].vertexes.begin(); it != graphs[gs[i]].vertexes.end(); ++it)
      {
        g.vertexes.insert(*it);
      }

      for (auto et = graphs[gs[i]].edges.begin(); et != graphs[gs[i]].edges.end(); ++et)
      {
        for (auto jt = et->second.begin(); jt != et->second.end(); ++jt)
        {
          auto t1 = et->first;
          auto t2 = jt->first;
          g.edges[t1][t2].insert(g.edges[t1][t2].end(), jt->second.begin(), jt->second.end());
        }
      }
    }

    graphs[newg] = g;
  }

  void cmd_extract(const Array< std::string > & args)
  {
    if (args.size() < 5)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    const std::string & newg = args[1];
    const std::string & oldg = args[2];
    size_t count = atoi(args[3].c_str());

    if (graphs.find(newg) != graphs.end() || graphs.find(oldg) == graphs.end() || args.size() < 4 + count)
    {
      std::cout << "<INVALID COMMAND>\n";
      return;
    }

    Graph g;
    for (size_t i = 0; i < count; ++i)
    {
      const std::string & vertex = args[4 + i];
      if (graphs[oldg].vertexes.find(vertex) == graphs[oldg].vertexes.end())
      {
        std::cout << "<INVALID COMMAND>\n";
        return;
      }
      g.vertexes.insert(vertex);
    }

    for (size_t i = 0; i < count; ++i)
    {
      const std::string & src = args[4 + i];
      for (size_t j = 0; j < count; ++j)
      {
        const std::string & dst = args[4 + j];
        bool c1 = graphs[oldg].edges.find(src) != graphs[oldg].edges.end();
        if (c1 && graphs[oldg].edges[src].find(dst) != graphs[oldg].edges[src].end())
        {
          g.edges[src][dst] = graphs[oldg].edges[src][dst];
        }
      }
    }

    graphs[newg] = g;
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Missing input file\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Cannot open file\n";
    return 1;
  }

  using namespace savintsev;

  std::string line;
  while (getline(file, line))
  {
    if (line.empty())
    {
      continue;
    }
    Array< std::string > header = split(line);
    if (header.size() != 2)
    {
      continue;
    }

    std::string name = header[0];
    int count = atoi(header[1].c_str());
    Graph g;

    for (int i = 0; i < count; ++i)
    {
      getline(file, line);
      if (line.empty())
      {
        continue;
      }
      Array< std::string > e = split(line);
      if (e.size() != 3)
      {
        continue;
      }

      std::string a = e[0], b = e[1];
      int w = atoi(e[2].c_str());
      g.vertexes.insert(a);
      g.vertexes.insert(b);
      g.edges[a][b].push_back(w);
    }

    graphs[name] = g;
  }

  using namespace std::placeholders;

  savintsev::HashMap< std::string, std::function< void(const Array< std::string > &) > > cmd_map;
  cmd_map["graphs"] = std::bind(cmd_graphs);
  cmd_map["vertexes"] = std::bind(cmd_vertexes, _1);
  cmd_map["outbound"] = std::bind(cmd_outbound, _1);
  cmd_map["inbound"] = std::bind(cmd_inbound, _1);
  cmd_map["bind"] = std::bind(cmd_bind, _1);
  cmd_map["cut"] = std::bind(cmd_cut, _1);
  cmd_map["create"] = std::bind(cmd_create, _1);
  cmd_map["merge"] = std::bind(cmd_merge, _1);
  cmd_map["extract"] = std::bind(cmd_extract, _1);

  std::string input;
  while (getline(std::cin, input))
  {
    if (input.empty())
    {
      continue;
    }
    Array< std::string > args = split(input);
    if (args.empty())
    {
      continue;
    }

    try
    {
      cmd_map.at(args[0])(args);
    }
    catch (const std::out_of_range &)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
