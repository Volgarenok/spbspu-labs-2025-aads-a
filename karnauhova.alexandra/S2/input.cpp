#include "input.hpp"

bool karnauhova::isOperator(const std::string& element)
{
  return element == "+" || element == "/" || element == "-" || element == "*" || element == "%";
}

karnauhova::Queue< std::string > karnauhova::splitStr(const std::string& str)
{
  Queue< std::string > inf;
  const char separator = ' ';
  size_t start = 0;
  size_t end = str.find(separator);
  while (end != std::string::npos)
  {
    if (end != start)
    {
      inf.push(str.substr(start, end - start));
    }
    start = end + 1;
    end = str.find(separator, start);
  }
  if (start < str.length())
  {
    inf.push(str.substr(start));
  }
  return inf;
}

karnauhova::Queue< std::string > karnauhova::toPost(Queue< std::string > inf)
{
  Stack< std::string > symbols;
  Queue< std::string > post;
  while (!inf.empty())
  {
    std::string element = inf.front();
    if (isOperator(element))
    {
      if (!symbols.empty())
      {
        std::string last = symbols.top();
        if (isOperator(last))
        {
          post.push(last);
          symbols.pop();
        }
      }
      symbols.push(element);
      inf.pop();
      continue;
    }
    if (element == "(")
    {
      symbols.push(element);
      inf.pop();
      continue;
    }
    if (element == ")")
    {
      if (symbols.empty())
      {
        throw std::logic_error("Incorrect 1");
      }
      std::string last = symbols.top();
      if (last != "(" && !isOperator(last))
      {
        throw std::logic_error("Incorrect 2");
      }
      if (last != "(")
      {
        post.push(last);
        symbols.pop();
      }
      symbols.pop();
      inf.pop();
      continue;
    }
    std::stoll(element);
    post.push(element);
    inf.pop();
  }
  if (symbols.empty())
  {
    return post;
  }
  post.push(symbols.top());
  symbols.pop();
  if (!symbols.empty())
  {
    throw std::logic_error("Error in input");
  }
  return post;
}

karnauhova::Stack< long long int > karnauhova::inputStr(std::istream& in)
{
  karnauhova::Stack< long long int > calc;
  std::string str;
  while (std::getline(in, str))
  {
    if (str.empty())
    {
      continue;
    }
    karnauhova::Queue< std::string > inf = karnauhova::splitStr(str);
    karnauhova::Queue< std::string > post = toPost(inf);
    long long int sum = procPost(post);
    calc.push(sum);
  }
  in.clear();
  return calc;
}
