#include "commands.hpp"
#include "io-utils.hpp"

template< typename Key, typename T >
using dictionary_t = std::map< Key, T >;
template< typename Key, typename T >
using dataset_t = std::map< std::string, dictionary_t< Key, T > >;

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "<WRONG NUMBER OF ARGUMENTS>";
    return 1;
  }
  std::ifstream in(argv[1]);
  dataset_t< int, std::string > dataset = kushekbaev::readInputFromFile< int, std::string >(in);
  while (!std::cin.eof())
  {
    try
    {
      kushekbaev::executeCommand< int, std::string >(std::cin, std::cout, dataset);
    }
    catch (const std::exception& e)
    {
      std::cout << e.what() << "\n";
    }
  }
}
