#include "key_summ.hpp"
#include <limits>
#include <calc_utils.hpp>

namespace
{
  bool isOverflow(const int a, const int b)
  {
    int max = std::numeric_limits< int >::max();
    int min = std::numeric_limits< int >::min();
    if (a > 0 && b > 0)
    {
      return a > max - b;
    }

    return (a < 0 && b < 0) && (a < min + b);
  }
}

demehin::KeySumm::KeySumm() noexcept:
  val_res(0),
  str_res("")
{}

void demehin::KeySumm::operator()(const std::pair< const int, std::string >& data)
{
  if (isOverflow(val_res, data.first))
  {
    throw std::logic_error("overflow");
  }

  val_res += data.first;
  if (str_res.size() == 0)
  {
    str_res = data.second;
  }
  else
  {
    str_res = str_res + " " + data.second;
  }
}
