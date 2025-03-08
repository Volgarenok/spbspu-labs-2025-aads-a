#ifndef TEST_COMMON_HPP
#define TEST_COMMON_HPP

namespace savintsev
{
  bool divisible_by_five(const int & value)
  {
    return (value % 2);
  }
  struct is_even
  {
    bool operator()(const int & value)
    {
      return (value % 2) == 0;
    }
  };
}

#endif
