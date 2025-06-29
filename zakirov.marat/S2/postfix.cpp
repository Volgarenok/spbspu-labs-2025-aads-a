#include "postfix.hpp"
#include <limits>
#include "queue.hpp"
#include "stack.hpp"

zakirov::Postfix::Postfix(const std::string & infix)
{
  Queue< std::string > infix_queue;
  size_t old_pos = 0;
  size_t new_pos = 0;
  while (new_pos != std::string::npos)
  {
    new_pos = infix.find(" ", old_pos);
    infix_queue.push(infix.substr(old_pos, new_pos - old_pos));
    old_pos = new_pos + 1;
  }

  Stack< std::string > op_buffer;
  while (!infix_queue.empty())
  {
    std::string symbol = infix_queue.front();
    infix_queue.pop();
    if (check_operand(symbol))
    {
      postfix.push(symbol);
    }
    else if (check_operator(symbol))
    {
      while (!op_buffer.empty() && check_priority(symbol) <= check_priority(op_buffer.top()) && op_buffer.top() != "(")
      {
        postfix.push(op_buffer.top());
        op_buffer.pop();
      }

      op_buffer.push(symbol);
    }
    else if (symbol == "(")
    {
      op_buffer.push(symbol);
    }
    else if (symbol == ")")
    {
      while (op_buffer.top() != "(" && !op_buffer.empty())
      {
        postfix.push(op_buffer.top());
        op_buffer.pop();
      }

      if (!op_buffer.empty())
      {
        op_buffer.pop();
      }
      else
      {
        throw std::invalid_argument("Incorrect placement of brackets");
      }
    }
    else
    {
      throw std::invalid_argument("Incorrect expression");
    }
  }

  while (!op_buffer.empty())
  {
    if (op_buffer.top() != "(")
    {
      postfix.push(op_buffer.top());
      op_buffer.pop();
    }
    else
    {
      throw std::invalid_argument("Incorrect placement of brackets");
    }
  }
}

zakirov::Postfix zakirov::Postfix::operator+(const Postfix & other)
{
  Postfix result(*this);
  result.unite_postfixes(other);
  result.postfix.push("+");
  return result;
}

zakirov::Postfix zakirov::Postfix::operator-(const Postfix & other)
{
  Postfix result(*this);
  result.unite_postfixes(other);
  result.postfix.push("-");
  return result;
}

zakirov::Postfix zakirov::Postfix::operator*(const Postfix & other)
{
  Postfix result(*this);
  result.unite_postfixes(other);
  result.postfix.push("*");
  return result;
}

zakirov::Postfix zakirov::Postfix::operator/(const Postfix & other)
{
  Postfix result(*this);
  result.unite_postfixes(other);
  result.postfix.push("/");
  return result;
}

zakirov::Postfix zakirov::Postfix::operator%(const Postfix & other)
{
  Postfix result(*this);
  result.unite_postfixes(other);
  result.postfix.push("%");
  return result;
}

long long zakirov::Postfix::operator()()
{
  Stack< long long > result;
  Queue< std::string > postfix_temp(postfix);
  long long first_v = 0;
  long long second_v = 0;
  long long result_v = 0;
  while (!postfix_temp.empty())
  {
    if (check_operand(postfix_temp.front()))
    {
      result.push(transform_to_llint(postfix_temp.front()));
      postfix_temp.pop();
    }
    else if (check_operator(postfix_temp.front()))
    {
      if (result.size() < 2)
      {
        throw std::invalid_argument("Incorrect expression");
      }

      first_v = result.top();
      result.pop();
      second_v = result.top();
      result.pop();
      result_v = calculate_postfix(first_v, second_v, postfix_temp.front().front());
      result.push(result_v);
      postfix_temp.pop();
    }
  }

  if (result.size() != 1)
  {
    throw std::invalid_argument("Incorrect expression");
  }

  return result.top();
}

zakirov::Postfix zakirov::Postfix::unite_postfixes(const Postfix & other)
{
  Postfix temporary(other);
  while (!temporary.postfix.empty())
  {
    postfix.push(temporary.postfix.front());
    temporary.postfix.pop();
  }

  return *this;
}

void zakirov::Postfix::check_overflow(long long first, long long second, char oper)
{
  long long llint_max = std::numeric_limits< long long >::max();
  long long llint_min = std::numeric_limits< long long >::min();
  long long max_num = std::max(first, second);
  long long min_num = std::min(first, second);
  if (oper == '+')
  {
    if ((max_num > 0 && min_num > 0) && (llint_max - max_num < min_num))
    {
      throw std::overflow_error("Variable overflow");
    }
    else if ((max_num < 0 && min_num < 0) && (llint_min - max_num > min_num))
    {
      throw std::overflow_error("Variable underflow");
    }
  }
  else if (oper == '-')
  {
    if ((max_num < 0 && min_num > 0) && (llint_max + max_num < min_num))
    {
      throw std::overflow_error("Variable overflow");
    }
    else if ((max_num > 0 && min_num < 0) && (llint_min + max_num > min_num))
    {
      throw std::overflow_error("Variable underflow");
    }
  }
  else if (oper == '*')
  {
    if (((max_num < 0 && min_num < 0) && (llint_max / min_num > min_num)) || ((max_num > 0 && min_num > 0) && (llint_max / max_num < min_num)))
    {
      throw std::overflow_error("Variable overflow");
    }
    else if (((max_num > 0 && min_num < 0) || (max_num < 0 && min_num > 0)) && (llint_min / max_num > min_num))
    {
      throw std::overflow_error("Variable underflow");
    }
  }
  else if (oper == '/')
  {
    if (min_num == llint_min && max_num == -1)
    {
      throw std::overflow_error("Variable overflow");
    }
  }
}

bool zakirov::Postfix::check_operand(const std::string & line)
{
  size_t dot_counter = 0;
  std::string::const_iterator fillable_it = line.cbegin();
  if ((*fillable_it == '-' || *fillable_it == '+') && line.size() != 1)
  {
    ++fillable_it;
  }

  for (; fillable_it != line.cend(); ++fillable_it)
  {
    if (*fillable_it == '.')
    {
      ++dot_counter;
    }
    else if (!isdigit(*fillable_it))
    {
      return false;
    }
  }

  if (dot_counter > 1)
  {
    return false;
  }

  return true;
}

bool zakirov::Postfix::check_operator(std::string symbol)
{
  constexpr char operators[5] = {'+', '-', '*', '/', '%'};
  for (size_t i = 0; i < 5; ++i)
  {
    if (symbol.front() == operators[i] && symbol.size() == 1)
    {
      return true;
    }
  }

  return false;
}

bool zakirov::Postfix::check_priority(std::string symbol)
{
  return symbol == "*" || symbol == "/";
}

zakirov::Queue< std::string > zakirov::Postfix::transform_to_postfix(Queue< std::string > & infix)
{
  Stack< std::string > op_buffer;
  Queue< std::string > result;

  while (!infix.empty())
  {
    std::string symbol = infix.front();
    infix.pop();
    if (check_operand(symbol))
    {
      result.push(symbol);
    }
    else if (check_operator(symbol))
    {
      while (!op_buffer.empty() && check_priority(symbol) <= check_priority(op_buffer.top()) && op_buffer.top() != "(")
      {
        result.push(op_buffer.top());
        op_buffer.pop();
      }

      op_buffer.push(symbol);
    }
    else if (symbol == "(")
    {
      op_buffer.push(symbol);
    }
    else if (symbol == ")")
    {
      while (op_buffer.top() != "(" && !op_buffer.empty())
      {
        result.push(op_buffer.top());
        op_buffer.pop();
      }

      if (!op_buffer.empty())
      {
        op_buffer.pop();
      }
      else
      {
        throw std::invalid_argument("Incorrect placement of brackets");
      }
    }
    else
    {
      throw std::invalid_argument("Incorrect expression");
    }
  }

  while (!op_buffer.empty())
  {
    if (op_buffer.top() != "(")
    {
      result.push(op_buffer.top());
      op_buffer.pop();
    }
    else
    {
      throw std::invalid_argument("Incorrect placement of brackets");
    }
  }

  return result;
}

long long zakirov::Postfix::transform_to_llint(const std::string & line)
{
  Stack< char > num_buffer;
  bool flag = false;
  std::string::const_iterator fillable_it = line.cbegin();
  if (*fillable_it == '-')
  {
    flag = true;
    ++fillable_it;
  }

  long long result = 0;
  for (; fillable_it != line.end(); ++fillable_it)
  {
    num_buffer.push(*fillable_it);
  }

  size_t size_b = num_buffer.size();
  long long num_power = 1;
  if (flag)
  {
    num_power = -1;
  }

  for (size_t i = 0; i < size_b; ++i, num_power *= 10)
  {
    result += (num_buffer.top() - '0') * num_power;
    num_buffer.pop();
  }

  return result;
}

long long zakirov::Postfix::calculate_postfix(long long first, long long second, char oper)
{
  check_overflow(second, first, oper);
  if (oper == '/')
  {
    return second / first;
  }
  else if (oper == '*')
  {
    return second * first;
  }
  else if (oper == '-')
  {
    return second - first;
  }
  else if (oper == '+')
  {
    return second + first;
  }

  if (oper == '%' && second < 0)
  {
    return first + (second % first);
  }

  return second % first;
}
