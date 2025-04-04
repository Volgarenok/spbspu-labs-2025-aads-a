#include "io-utils.hpp"
#include <iostream>
#include <sstream>

namespace aleksandrov
{
  StreamGuard::StreamGuard(std::basic_ios< char >& s):
    s_(s),
    fill_(s.fill()),
    precision_(s.precision()),
    width_(s.width()),
    flags_(s.flags())
  {}

  StreamGuard::~StreamGuard()
  {
    s_.fill(fill_);
    s_.precision(precision_);
    s_.width(width_);
    s_.flags(flags_);
  }

  std::istream& operator>>(std::istream& input, InfixPart& token)
  {
    std::istream::sentry s(input);
    if (!s)
    {
      return input;
    }
    StreamGuard guard(input);

    char c = input.peek();

    if (std::isdigit(c))
    {
      std::string num;
      while (std::isdigit(c))
      {
        num += input.get();
        c = input.peek();
      }

      long long int value = 0ll;
      std::istringstream iss(num);
      if (iss >> value)
      {
        token = InfixPart(value);
        return input;
      }
      else
      {
        for (auto it = num.rbegin(); it != num.rend(); ++it)
        {
          input.putback(*it);
        }
        input.setstate(std::ios::failbit);
      }
    }
    else
    {
      char symbol = '\0';
      if (input >> symbol)
      {
        try
        {
          token = InfixPart(symbol);
        }
        catch (const std::logic_error&)
        {
          input.putback(symbol);
          input.setstate(std::ios::failbit);
        }
      }
    }
    return input;
  }
}

