#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <iterator>
#include <functional>
#include "functional.hpp"

namespace rychkov
{
  template< class InputIt, class Unary >
  Unary for_each(InputIt from, InputIt to, Unary f)
  {
    for (; from != to; ++from)
    {
      invoke(f, *from);
    }
    return f;
  }
  template< class InputIt, class Sz, class Unary >
  Unary for_each_n(InputIt from, Sz n, Unary f)
  {
    for (Sz i = 0; i < n; ++from)
    {
      invoke(f, *from);
    }
    return f;
  }

  template< class InputIt, class T = typename std::iterator_traits< InputIt >::value_type >
  InputIt find(InputIt from, InputIt to, const T& value)
  {
    for (; from != to; ++from)
    {
      if (*from == value)
      {
        return from;
      }
    }
    return to;
  }
  template< class InputIt, class UnaryPred >
  InputIt find_if(InputIt from, InputIt to, UnaryPred predicate)
  {
    for (; from != to; ++from)
    {
      if (invoke_r< bool >(predicate, *from))
      {
        return from;
      }
    }
    return to;
  }
  template< class InputIt, class UnaryPred >
  InputIt find_if_not(InputIt from, InputIt to, UnaryPred predicate)
  {
    for (; from != to; ++from)
    {
      if (!invoke_r< bool >(predicate, *from))
      {
        return from;
      }
    }
    return to;
  }

  template< class FwdIt, class BinaryPred >
  FwdIt unique(FwdIt from, FwdIt to, BinaryPred predicate)
  {
    if (from == to)
    {
      return to;
    }
    FwdIt result = from;
    while (++from != to)
    {
      if (!invoke_r< bool >(predicate, *result, *from) && (++result != from))
      {
        *result = std::move(*from);
      }
    }
    return ++result;
  }
  template< class FwdIt >
  FwdIt unique(FwdIt from, FwdIt to)
  {
    return rychkov::unique(from, to, std::equal_to<>{});
  }

  template< class InputIt, class OutputIt, class UnaryOper >
  OutputIt transform(InputIt from, InputIt to, OutputIt out, UnaryOper oper)
  {
    for (; from != to; ++from, ++out)
    {
      *out = invoke(oper, *from);
    }
    return out;
  }
  template< class InputIt, class OutputIt, class BinaryOper >
  OutputIt transform(InputIt from, InputIt to, InputIt from2, OutputIt out, BinaryOper oper)
  {
    for (; from != to; ++from, ++from2, ++out)
    {
      *out = invoke(oper, *from, *from2);
    }
    return out;
  }

  template< class InputIt, class OutputIt >
  OutputIt copy(InputIt from, InputIt to, OutputIt out)
  {
    for (; from != to; ++from, ++out)
    {
      *out = *from;
    }
    return out;
  }
  template< class InputIt, class OutputIt, class UnaryPred >
  OutputIt copy_if(InputIt from, InputIt to, OutputIt out, UnaryPred predicate)
  {
    for (; from != to; ++from, ++out)
    {
      if (invoke_r< bool >(predicate, *from))
      {
        *out = *from;
      }
    }
    return out;
  }
}

#endif
