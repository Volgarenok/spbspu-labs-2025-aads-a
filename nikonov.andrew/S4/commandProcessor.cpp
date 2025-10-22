#include "commandProcessor.hpp"
#include <map>
#include <functional>
#include <stdexcept>

void nikonov::processCommand(TreeOfTrees & trees, const std::string & cmd, std::istream & in, std::ostream & out)
{
  std::map< std::string, std::function< void(TreeOfTrees&, std::istream&, std::ostream&) > > commandMap;
  commandMap["print"] = print;
  commandMap["complement"] = complement;
  commandMap["intersect"] = intersect;
  commandMap["union"] = unite;
  try
  {
    commandMap.at(cmd)(trees, in, out);
  }
  catch (const std::out_of_range&)
  {
    out << "<INVALID COMMAND>\n";
  }
}
