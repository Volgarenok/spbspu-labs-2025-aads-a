#ifndef SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S2_OPERATIONS_HPP
#define SPBSPU_LABS_2025_AADS_A_KIZHIN_EVGENIY_S2_OPERATIONS_HPP

#include <cstdint>

namespace kizhin {
  enum class OperationPrecedence;
  class BinaryOperation;

  class Addition;
  class Subtraction;
  class Multiplication;
  class Division;
  class Modulus;
}

enum class kizhin::OperationPrecedence {
  addition = 90,
  subtraction = 90,
  multiplication = 100,
  division = 100,
  modulus = 100,
};

class kizhin::BinaryOperation
{
public:
  using number_type = std::int64_t;

  BinaryOperation() = default;
  BinaryOperation(const BinaryOperation&) = delete;
  virtual ~BinaryOperation() = default;
  BinaryOperation& operator=(const BinaryOperation&) = delete;
  virtual number_type eval(number_type, number_type) const = 0;
  virtual OperationPrecedence precedence() const noexcept = 0;
};

class kizhin::Addition: public BinaryOperation
{
public:
  number_type eval(number_type, number_type) const override;
  OperationPrecedence precedence() const noexcept override;
  static Addition& instance() noexcept;

private:
  Addition() = default;
};

class kizhin::Subtraction: public BinaryOperation
{
public:
  number_type eval(number_type, number_type) const override;
  OperationPrecedence precedence() const noexcept override;
  static Subtraction& instance() noexcept;

private:
  Subtraction() = default;
};

class kizhin::Multiplication: public BinaryOperation
{
public:
  number_type eval(number_type, number_type) const override;
  OperationPrecedence precedence() const noexcept override;
  static Multiplication& instance() noexcept;

private:
  Multiplication() = default;
};

class kizhin::Division: public BinaryOperation
{
public:
  number_type eval(number_type, number_type) const override;
  OperationPrecedence precedence() const noexcept override;
  static Division& instance() noexcept;

private:
  Division() = default;
};

class kizhin::Modulus: public BinaryOperation
{
public:
  number_type eval(number_type, number_type) const override;
  OperationPrecedence precedence() const noexcept override;
  static Modulus& instance() noexcept;

private:
  Modulus() = default;
};

#endif

