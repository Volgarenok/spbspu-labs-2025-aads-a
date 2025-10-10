#include "commandProcessor.hpp"
#include "commands.hpp"
#include <functional>

namespace shramko
{
  void printWrapper(TreeOfTrees& trees, std::istream& in, std::ostream& out)
  {
    print(trees, in, out);
  }

  void processCommand(TreeOfTrees & trees, const std::string & cmd, std::istream & in, std::ostream & out)
  {
    using CommandFunction = std::function< void(TreeOfTrees&, std::istream&, std::ostream&) >;
    static UBstTree< std::string, CommandFunction > commandMap;
    if (commandMap.empty())
    {
      commandMap["print"] = printWrapper;
      commandMap["complement"] = complement;
      commandMap["intersect"] = intersect;
      commandMap["union"] = unite;
    }
    auto it = commandMap.find(cmd);
    if (it != commandMap.cend())
    {
      it->second(trees, in, out);
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }
}
