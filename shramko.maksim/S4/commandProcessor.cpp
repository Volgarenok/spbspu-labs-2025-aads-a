#include "commandProcessor.hpp"
#include "commands.hpp"

void shramko::processCommand(TreeOfTrees & trees, const std::string & cmd, std::istream & in, std::ostream & out)
{
  if (cmd == "print")
  {
    print(trees, in, out);
  }
  else if (cmd == "complement")
  {
    complement(trees, in, out);
  }
  else if (cmd == "intersect")
  {
    intersect(trees, in, out);
  }
  else if (cmd == "union")
  {
    unite(trees, in, out);
  }
  else
  {
    out << "<INVALID COMMAND>\n";
  }
}
