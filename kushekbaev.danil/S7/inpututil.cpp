#include "inpututil.hpp"

void kushekbaev::inputGraphs(std::istream& in, std::map< std::string, kushekbaev::Graph >& graphs)
{
  std::string name;
  size_t count = 0;
  while (in >> name >> count)
  {
    Graph gr;
    for (size_t i = 0; i < count; ++i)
    {
      std::string vect1, vect2;
      size_t weight = 0;
      in >> vect1 >> vect2 >> weight;
      gr.add_edge(vect1, vect2, weight);
    }
    graphs[name] = gr;
  }
}
