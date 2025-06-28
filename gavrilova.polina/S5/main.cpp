#include <exception>
#include <fstream>
#include <iostream>
#include <string>

#include <ArrayBuffer.hpp>
#include <tree/ConstIterator.hpp>
#include <tree/Iterator.hpp>
#include <tree/TwoThreeTree.hpp>

#include "Collector.hpp"

namespace detail {

  template < typename Key, typename Value >
  gavrilova::ArrayBuffer< std::pair< Key, Value > > parse_file(const std::string& filename)
  {
    std::ifstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Error! Invalid opening file!");
    }

    gavrilova::ArrayBuffer< std::pair< Key, Value > > data;
    Key key;
    Value value;

    while (file >> key >> value) {
      data.push_back(std::make_pair(key, value));
    }

    return data;
  }

}

int main(int argc, char* argv[])
{
  using namespace gavrilova;

  if (argc != 3) {
    std::cerr << "Incorrect format of input.\n";
    return 1;
  }

  Collector< long long int, std::string > collector;

  try {
    ArrayBuffer< std::pair< long long int, std::string > > data = detail::parse_file< long long int, std::string >(argv[2]);

    TwoThreeTree< long long int, std::string > tree;

    for (size_t i = 0; i < data.size(); ++i) {
      tree.insert({data[i].first, data[i].second});
    }

    if (tree.empty()) {
      std::cout << "<EMPTY>\n";
      return 0;
    }

    std::string order = argv[1];
    if (order == "ascending") {
      collector = tree.traverse_lnr(collector);
    } else if (order == "descending") {
      collector = tree.traverse_rnl(collector);
    } else if (order == "breadth") {
      collector = tree.traverse_breadth(collector);
    } else {
      std::cerr << "Invalid traversal order\n";
      return 1;
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  std::cout << collector.getSum();
  const ArrayBuffer< std::string >& values = collector.getBuffer();
  for (size_t i = 0; i < values.size(); ++i) {
    std::cout << " " << values[i];
  }
  std::cout << "\n";
}
