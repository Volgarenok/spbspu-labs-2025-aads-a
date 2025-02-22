#include <istream>
#include <string>
#include <list>
namespace nikonov
{
  std::list< std::pair< std::string, std::list< int >* >* >* getPList(std::istream& input);
  void processPList(std::list< std::pair< std::string, std::list< int >* >* >*);
}
