#idndef OUTPUTLIST_HPP
#define OUTPUTLIST_HPP
#include <ostream>
#include <list>
#include <utility>

namespace dirti
{
  void outputList(const std::list< std::pair< std::string, std::list< int >>> mainList, std::ostream& out);
}

#endif
