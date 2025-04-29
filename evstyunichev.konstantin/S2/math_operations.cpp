#include "math_operations.hpp"
#include <cmath>
#include <stdexcept>

long long evstyunichev::summ(long long a, long long b, bool is_negative)
{
  if (is_negative)
  {
    if (b == Minll)
    {
      throw std::overflow_error("summ");
    }
    b = -b;
  }
  if (a < b)
  {
    std::swap(a, b);
  }
  if (((b > 0) && (Maxll - a < b)) || ((a < 0) && (Minll - a > b)))
  {
    throw std::overflow_error("summ");
  }
  return a + b;
}

long long evstyunichev::division(long long a, long long b)
{
  if ((a == Minll) && (b == -1))
  {
    throw std::overflow_error("div");
  }
  if (b == 0)
  {
    throw std::logic_error("division by 0");
  }
  return a / b;
}

long long evstyunichev::multipling(long long a, long long b)
{
  if ((a == 0) || (b == 0))
  {
    return 0;
  }
  if (a < b)
  {
    std::swap(a, b);
  }
  if (((a / b < 0) && (Minll / b < a)) || ((a / b > 0) && (std::abs(Maxll / a) < b)))
  {
    throw std::logic_error("mul");
  }
  return a * b;
}

long long evstyunichev::modul(long long a, long long b)
{
  long long ans = a % b;
  if (ans < 0)
  {
    if (b > 0)
    {
      b = -b;
    }
    ans -=b;
  }
  return ans;
}

long long evstyunichev::concatinate(long long a, long long b)
{
  if ((a <= 0) || (b < 0))
  {
    throw std::logic_error("conc");
  }
  long long k = 1;
  do
  {
    k *= 10;
  }
  while (b < k);
  return summ(multipling(a, k), b);
}
