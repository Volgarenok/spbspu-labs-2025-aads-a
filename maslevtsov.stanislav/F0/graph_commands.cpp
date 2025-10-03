#include "graph_commands.hpp"
#include <fstream>
#include <vector/definition.hpp>
#include <hash_table/definition.hpp>
#include "graph.hpp"

bool maslevtsov::check_graphs_format(std::istream& in)
{
  std::string graph_name;
  while ((in >> graph_name) && (!in.eof())) {
    Graph gr;
    in >> gr;
  }
  if (in.eof()) {
    return true;
  }
  return false;
}

void maslevtsov::print_help_manual(std::ostream& out)
{
  out << "UNDIRECTED UNWEIGHT GRAPH\n\n";
  out << "SYNOPSIS:\n";
  out << "program [OPTION]... [FILE]...\n";
  out << "Launching the program with loading graphs from the [FILE]\n";
  out << "Options: --help - help output\n";
  out << "         --check - check graphs foramt in [FILE]\n\n";
  out << "GRAPHS FORMAT:\n";
  out << "<graph_name>\n<vertices_number>\n<vertice1>: <vertice2>...\n<vertice2>: <vertice1>...\n...\n";
  out << "Vertices of the graph are numbered with an unsigned integer, starting from zero.\n\n";
  out << "USER COMMANDS:\n";
  out << "1. save [FILE] - save all current graphs to [FILE];\n";
  out << "2. open [FILE] - read graphs from [FILE];\n";
  out << "3. add-graph <graph_name> <vertices_number> <edges_number>\n   <vertice1> <vertice2>\n   ...\n";
  out << "   create a graph named <graph_name> with edges: <vertice1> <vertice2>...;\n";
  out << "4. add-vertice <graph_name> <vertice>\n";
  out << "   add vertice to <graph_name>;\n";
  out << "5. add-edge <graph_name> <vertice1> <vertice2>\n";
  out << "   add edge to <graph_name>;\n";
  out << "6. print-graph <graph_name> - print adjacency list for <graph_name>;\n";
  out << "7. copy <graph_name> <new_graph_name> - copy <graph_name> to <new_graph_name>;\n";
  out << "8. delete-graph <graph_name> - delete <graph_name>;\n";
  out << "9. delete-vertice <graph_name> <vertice> - delete <vertice> from <graph_name>;\n";
  out << "10. delete-edge <graph_name> <vertice1> <vertice2> - delete edge from <graph_name>;\n";
  out << "11. union <new_graph_name> <graph_name1> <graph_name2>\n";
  out << "    create <new_graph_name> based on two existing <graph_name1> and <graph_name2>;\n";
  out << "12. subgraph <new_graph_name> <graph_name> <vertices_number> <vertice1>...\n";
  out << "    create <new_graph_name> by selecting the vertices of <graph_name>, preserving the edges;\n";
  out << "13. bfs <graph_name> <start_node>\n";
  out << "    breadth-first traversal of <graph_name> starting at <start_node>,\n";
  out << "    printing all distances from <start_node> to each node reachable from <start_node>;\n";
  out << "14. path <graph_name> <start_node> <goal_node>\n";
  out << "    find the shortest chain of nodes from <start_node> to <goal_node>,\n";
  out << "    calculate the minimum distance in <graph_name>;\n";
  out << "15. width <graph_name> - find width of <graph_name>;\n";
  out << "16. components <graph_name> - find connected components in <graph_name>.";
}

void maslevtsov::save_graphs(const graphs_t& graphs, std::istream& in)
{
  std::string filename;
  in >> filename;
  std::ofstream save_file(filename);
  if (!save_file) {
    throw std::invalid_argument("invalid filename");
  }
  for (auto i = graphs.cbegin(); i != graphs.cend(); ++i) {
    save_file << i->first << '\n';
    save_file << i->second << '\n';
  }
}

void maslevtsov::open_graphs(graphs_t& graphs, std::istream& in, std::ostream& out)
{
  std::string filename;
  in >> filename;
  std::ifstream open_file(filename);
  if (!open_file) {
    throw std::invalid_argument("invalid filename");
  }
  out << "Do you want to save current graphs? y/n: ";
  std::string save_info;
  in >> save_info;
  if (save_info != "y" && save_info != "n") {
    throw std::invalid_argument("invalid save info");
  }
  if (save_info == "y") {
    out << "Enter filename: ";
    save_graphs(graphs, in);
  }
  graphs_t new_graphs;
  std::string graph_name;
  while ((open_file >> graph_name) && (!open_file.eof())) {
    Graph gr;
    open_file >> gr;
    new_graphs[graph_name] = gr;
  }
  if (!open_file.eof()) {
    throw std::invalid_argument("invalid graphs format");
  }
  graphs = new_graphs;
}

void maslevtsov::add_graph(graphs_t& graphs, std::istream& in)
{
  std::string graph_name;
  in >> graph_name;
  if (graphs.find(graph_name) != graphs.end()) {
    throw std::invalid_argument("graph already exist");
  }
  Graph gr;
  size_t edge_count = 0;
  in >> edge_count;
  for (size_t i = 0; i != edge_count; ++i) {
    unsigned vertice1 = 0, vertice2 = 0;
    in >> vertice1 >> vertice2;
    gr.add_edge(vertice1, vertice2);
  }
  graphs[graph_name] = gr;
}

void maslevtsov::add_vertice(graphs_t& graphs, std::istream& in)
{
  std::string graph_name;
  unsigned vertice = 0;
  in >> graph_name >> vertice;
  graphs.at(graph_name).add_vertice(vertice);
}

void maslevtsov::add_edge(graphs_t& graphs, std::istream& in)
{
  std::string graph_name;
  unsigned vertice1 = 0, vertice2 = 0;
  in >> graph_name >> vertice1 >> vertice2;
  graphs.at(graph_name).add_edge(vertice1, vertice2);
}

void maslevtsov::print_graph(const graphs_t& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  in >> graph_name;
  out << graphs.at(graph_name);
  out << '\n';
}

void maslevtsov::copy_graph(graphs_t& graphs, std::istream& in)
{
  std::string gr_name, new_gr_name;
  in >> gr_name >> new_gr_name;
  if (graphs.find(new_gr_name) != graphs.end()) {
    throw std::invalid_argument("invalid graph names");
  }
  Graph new_gr(graphs.at(gr_name));
  graphs[new_gr_name] = new_gr;
}

void maslevtsov::delete_graph(graphs_t& graphs, std::istream& in)
{
  std::string graph_name;
  in >> graph_name;
  if (graphs.find(graph_name) == graphs.end()) {
    throw std::invalid_argument("invalid graph names");
  }
  graphs.erase(graphs.find(graph_name));
}

void maslevtsov::delete_vertice(graphs_t& graphs, std::istream& in)
{
  std::string graph_name;
  unsigned vertice = 0;
  in >> graph_name >> vertice;
  graphs.at(graph_name).delete_vertice(vertice);
}

void maslevtsov::delete_edge(graphs_t& graphs, std::istream& in)
{
  std::string graph_name;
  unsigned vertice1 = 0, vertice2 = 0;
  in >> graph_name >> vertice1 >> vertice2;
  graphs.at(graph_name).delete_edge(vertice1, vertice2);
}

void maslevtsov::union_graphs(graphs_t& graphs, std::istream& in)
{
  std::string new_gr_name, gr_name1, gr_name2;
  in >> new_gr_name >> gr_name1 >> gr_name2;
  if (graphs.find(new_gr_name) != graphs.end()) {
    throw std::invalid_argument("invalid graph names");
  }
  Graph new_gr(graphs.at(gr_name1), graphs.at(gr_name2));
  graphs[new_gr_name] = new_gr;
}

void maslevtsov::create_subgraph(graphs_t& graphs, std::istream& in)
{
  std::string new_gr_name, gr_name;
  size_t vertice_count = 0;
  in >> new_gr_name >> gr_name >> vertice_count;
  if (graphs.find(new_gr_name) != graphs.end()) {
    throw std::invalid_argument("invalid graph names");
  }
  maslevtsov::Vector< unsigned > vertices;
  unsigned vertice = 0;
  for (size_t i = 0; i != vertice_count; ++i) {
    in >> vertice;
    vertices.push_back(vertice);
  }
  Graph new_gr(graphs.at(gr_name), vertices);
  graphs[new_gr_name] = new_gr;
}
