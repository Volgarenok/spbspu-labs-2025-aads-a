#include "io_stack.hpp"
#include <string>

void zakirov::scan_infix(std::istream & in, Stack< Queue < std::string > > & stack_infix)
{
  std::string scan_infix;
  std::string str_infix;

  while (!in.eof())
  {
    Queue < std::string > temporary_queue;
    str_infix.clear();
    scan_infix.clear();
    getline(in, scan_infix);
    if (scan_infix.empty())
    {
      continue;
    }

    for (std::string::iterator i = scan_infix.begin(); i != scan_infix.end(); ++i)
    {
      if (isspace(*i) && !str_infix.empty())
      {
        temporary_queue.push(str_infix);
        str_infix.clear();
        continue;
      }

      str_infix += *i;
    }

    if (!str_infix.empty())
    {
      temporary_queue.push(str_infix);
    }
    if (!temporary_queue.empty())
    {
      stack_infix.push(std::move(temporary_queue));
    }
  }
}

void zakirov::output_results(std::ostream & out, Stack< double > & results)
{
  if (results.empty())
  {
    return;
  }

  out << results.top();
  results.pop();
  while (!results.empty())
  {
    out << ' ' << results.top();
    results.pop();
  }
}
