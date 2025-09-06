#include "queue.hpp"

namespace shramko
{
  template class Queue< std::string >;
  template class Queue< Queue< std::string > >;
}
