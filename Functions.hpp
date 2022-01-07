#pragma once

#include "Constant.hpp"
#include "Expression.hpp"
#include "Operators.hpp"

#include <cmath>

namespace SymbolicMath {
namespace ImplDetails {
template <class E>
class _Sin : public UnaryExpression<E, _Sin> {
  using Base = UnaryExpression<E, _Sin>;
  friend Base;

 private:  // Members
  static constexpr auto func = [](auto x) { return std::sin(x); };
  static constexpr auto deriv = [](auto x, auto dx) { return Cos(x) * dx; };

 public:  // Constructors
  constexpr _Sin(const Expression<E>& expr) noexcept : Base(expr) {}
};

template <class E>
class _Cos : public UnaryExpression<E, _Cos> {
  using Base = UnaryExpression<E, _Cos>;
  friend Base;

 private:  // Members
  static constexpr auto func = [](auto x) { return std::cos(x); };
  static constexpr auto deriv = [](auto x, auto dx) { return -Sin(x) * dx; };

 public:  // Constructors
  constexpr _Cos(const Expression<E>& expr) noexcept : Base(expr) {}
};

template <class E>
class _Tan : public UnaryExpression<E, _Tan> {
  using Base = UnaryExpression<E, _Tan>;
  friend Base;

 private:  // Members
  static constexpr auto func = [](auto x) { return std::tan(x); };
  static constexpr auto deriv = [](auto x, auto dx) { return dx / (Cos(x) * Cos(x)); };

 public:  // Constructors
  constexpr _Tan(const Expression<E>& expr) noexcept : Base(expr) {}
};

template <class E>
class _Exp : public UnaryExpression<E, _Exp> {
  using Base = UnaryExpression<E, _Exp>;
  friend Base;

 private:  // Members
  static constexpr auto func = [](auto x) { return std::exp(x); };
  static constexpr auto deriv = [](auto x, auto dx) { return x * dx; };

 public:  // Constructors
  constexpr _Exp(const Expression<E>& expr) noexcept : Base(expr) {}
};
}  // namespace ImplDetails

template <class Arg>
constexpr auto Cos(const Arg& arg) {
  return ImplDetails::_Cos(ToExpression<Arg>(arg));
}

template <class Arg>
constexpr auto Sin(const Arg& arg) {
  return ImplDetails::_Sin(ToExpression<Arg>(arg));
}

template <class Arg>
constexpr auto Tan(const Arg& arg) {
  return ImplDetails::_Tan(ToExpression<Arg>(arg));
}

template <class Arg>
constexpr auto Exp(const Arg& arg) {
  return ImplDetails::_Exp(ToExpression<Arg>(arg));
}
}  // namespace SymbolicMath