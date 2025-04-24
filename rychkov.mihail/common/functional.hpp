#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

namespace rychkov
{
  template< class C, class T >
  class compare_to_value
  {
  public:
    compare_to_value(C condition, const T& rhs):
      value_(rhs),
      condition_(condition)
    {}
    bool operator()(const T& lhs)
    {
      return condition_(lhs, value_);
    }
  private:
    const T& value_;
    C condition_;
  };
};

#endif
