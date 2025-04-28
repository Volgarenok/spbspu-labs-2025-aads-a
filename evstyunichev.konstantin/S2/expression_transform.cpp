#include "expression_transform.hpp"
#include <iostream>
#include <string>
#include "../common/stack.hpp"
#include "../common/queue.hpp"

char evstyunichev::utoc(Unit &a)
{
  char ans = 0;
  if (!a.is_num)
  {
    ans = a.value;
  }
  else
  {
    throw std::overflow_error("int to char!");
  }
  return ans;
}

void evstyunichev::proceed(Unit &&a, Stack< Unit > &st, Queue< Unit > &qq)
{
  if (a.is_num)
  {
    qq.push(a);
    return;
  }
  if (a.value == ')')
  {
    while (!st.empty() && st.top().value != '(')
    {
      qq.push(st.top());
      st.pop();
    }
    if (st.empty())
    {
      throw std::logic_error("bad brackets");
    }
    st.pop();
    return;
  }
  else if (a.value == '(')
  {
    st.push(a);
    return;
  }
  while (!st.empty() && !comp_operand(st.top(), a))
  {
    qq.push(st.top());
    st.pop();
  }
  st.push(a);
  return;
}

int evstyunichev::operand_priory(Unit &a)
{
  int ans = -1;
  switch (a.value)
  {
  case ('('):
    ans = 0;
    break;
  case ('|'):
    ans = 1;
    break;
  case ('+'):
    ans = 2;
    break;
  case ('-'):
    ans = 2;
    break;
  case ('/'):
    ans = 3;
    break;
  case ('%'):
    ans = 3;
    break;
  case ('*'):
    ans = 3;
    break;
  default:
    break;
  }
  return ans;
}

bool evstyunichev::comp_operand(Unit &a, Unit &b)
{
  return operand_priory(a) < operand_priory(b);
}

evstyunichev::Unit evstyunichev::cast_to_unit(std::string &&s)
{
  for (auto c: "-+*/%|()")
  {
    if (s.size() == 1 && c == s[0])
    {
      return Unit{ c, 0 };
    }
  }
  return Unit{ std::stoll(s), 1 };
}

long long evstyunichev::result(std::string &str)
{
  using my_stack = evstyunichev::Stack< Unit >;
  using my_queue = evstyunichev::Queue< Unit >;
  my_stack st{};
  my_queue qq{};
  size_t start = 0, end = str.find(' ', 0);
  for (; end != std::string::npos; end = str.find(' ', end + 1))
  {
    proceed(cast_to_unit(str.substr(start, end - start)), st, qq);
    start = end + 1;
  }
  if (start)
  {
    proceed(cast_to_unit(str.substr(start, end - start)), st, qq);
  }
  while (!st.empty())
  {
    if (st.top().value == '(')
    {
      throw std::logic_error("bad brackets");
    }
    qq.push(st.top());
    st.pop();
  }
  while (!qq.empty())
  {
    Unit cur = qq.front();
    if (cur.is_num)
    {
      st.push(cur);
    }
    else if (st.size() > 1)
    {
      Unit b = st.top(), a{};
      st.pop();
      a = st.top();
      st.pop();
      st.push(Unit{ calculate(a, b, cur), 1 });
    }
    else
    {
      throw std::logic_error("invalid expr");
    }
    qq.pop();
  }
  if (st.size() > 1)
  {
    throw std::logic_error("bad expr");
  }
  return st.top().value;
  return 0;
}

long long evstyunichev::calculate(Unit &a, Unit &b, Unit &op)
{
  if (!a.is_num || !b.is_num)
  {
    throw std::logic_error("hm");
  }
  switch (op.value)
  {
  case ('-'):
    return a.value - b.value;
  case ('+'):
    return a.value + b.value;
  case ('*'):
    return a.value * b.value;
  case ('/'):
    return a.value / b.value;
  case ('%'):
    return a.value / b.value;
  case ('|'):
    return a.value | b.value;
  default:
    throw std::logic_error("hm");
  }
}
