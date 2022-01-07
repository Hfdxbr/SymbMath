#pragma once

#include "Constant.hpp"
#include "Expression.hpp"

namespace SymbolicMath {
namespace ImplDetails {
template <class E>
class _Negate : public UnaryExpression<E, _Negate> {
  using Base = UnaryExpression<E, _Negate>;
  friend Base;

 private:  // Members
  static constexpr auto func = [](auto x) { return -x; };
  static constexpr auto deriv = [](auto x, auto dx) { return -dx; };

 public:  // Constructors
  constexpr _Negate(const Expression<E>& expr) noexcept : Base(expr) {}
};

template <class E1, class E2>
class _Plus : public BinaryExpression<E1, E2, _Plus> {
  using Base = BinaryExpression<E1, E2, _Plus>;
  friend Base;

 private:  // Members
  static constexpr auto func = [](auto x1, auto x2) { return x1 + x2; };
  static constexpr auto deriv = [](auto x1, auto dx1, auto x2, auto dx2) { return dx1 + dx2; };

 public:  // Constructors
  constexpr _Plus(const Expression<E1>& expr1, const Expression<E2>& expr2) noexcept : Base(expr1, expr2) {}
};

template <class E1, class E2>
class _Minus : public BinaryExpression<E1, E2, _Minus> {
  using Base = BinaryExpression<E1, E2, _Minus>;
  friend Base;

 private:  // Members
  static constexpr auto func = [](auto x1, auto x2) { return x1 - x2; };
  static constexpr auto deriv = [](auto x1, auto dx1, auto x2, auto dx2) { return dx1 - dx2; };

 public:  // Constructors
  constexpr _Minus(const Expression<E1>& expr1, const Expression<E2>& expr2) noexcept : Base(expr1, expr2) {}
};

template <class E1, class E2>
class _Multiply : public BinaryExpression<E1, E2, _Multiply> {
  using Base = BinaryExpression<E1, E2, _Multiply>;
  friend Base;

 private:  // Members
  static constexpr auto func = [](auto x1, auto x2) { return x1 * x2; };
  static constexpr auto deriv = [](auto x1, auto dx1, auto x2, auto dx2) { return dx1 * x2 + x1 * dx2; };

 public:  // Constructors
  constexpr _Multiply(const Expression<E1>& expr1, const Expression<E2>& expr2) noexcept : Base(expr1, expr2) {}
};

template <class E1, class E2>
class _Divide : public BinaryExpression<E1, E2, _Divide> {
  using Base = BinaryExpression<E1, E2, _Divide>;
  friend Base;

 private:  // Members
  static constexpr auto func = [](auto x1, auto x2) { return x1 / x2; };
  static constexpr auto deriv = [](auto x1, auto dx1, auto x2, auto dx2) { return dx1 / x2 - x1 * dx2 / (x2 * x2); };

 public:  // Constructors
  constexpr _Divide(const Expression<E1>& expr1, const Expression<E2>& expr2) noexcept : Base(expr1, expr2) {}
};
}  // namespace ImplDetails

template <class Arg>
constexpr auto operator-(const Arg& arg) {
  return ImplDetails::_Negate(ToExpression(arg));
}

template <class Arg1, class Arg2>
constexpr auto operator+(const Arg1& arg1, const Arg2& arg2) {
  return ImplDetails::_Plus(ToExpression(arg1), ToExpression(arg2));
}

template <class Arg1, class Arg2>
constexpr auto operator-(const Arg1& arg1, const Arg2& arg2) {
  return ImplDetails::_Minus(ToExpression(arg1), ToExpression(arg2));
}

template <class Arg1, class Arg2>
constexpr auto operator*(const Arg1& arg1, const Arg2& arg2) {
  return ImplDetails::_Multiply(ToExpression(arg1), ToExpression(arg2));
}

template <class Arg1, class Arg2>
constexpr auto operator/(const Arg1& arg1, const Arg2& arg2) {
  return ImplDetails::_Divide(ToExpression(arg1), ToExpression(arg2));
}
}  // namespace SymbolicMath