#pragma once

#include "BasicExpression.hpp"

namespace SymbolicMath {
namespace Helpers {
template <class E1, class E2, template <class, class> class Binary>
constexpr auto BinaryExpresionHelper(const Expression<E1>& expr1, const Expression<E2>& expr2) {
  if constexpr (is_constant_v<E1> && is_constant_v<E2>)
    return Constant(Binary(expr1, expr2).Evaluate());
  else
    return Binary(expr1, expr2);
}

template <class T, class E, template <class, class> class Binary, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto BinaryExpresionHelper(const T& value, const Expression<E>& expr) {
  return BinaryExpresionHelper<Constant<T>, E, Binary>(Constant(value), expr);
}

template <class E, class T, template <class, class> class Binary, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto BinaryExpresionHelper(const Expression<E>& expr, const T& value) {
  return BinaryExpresionHelper<E, Constant<T>, Binary>(expr, Constant(value));
}
}  // namespace Helpers

namespace ImplDetails {
template <class E1, class E2>
class PlusImpl : public Expression<PlusImpl<E1, E2>> {
  const E1 expr1;
  const E2 expr2;

 public:
  PlusImpl(const Expression<E1>& expr1, const Expression<E2>& expr2)
      : expr1(expr1.CastToUnderlying()), expr2(expr2.CastToUnderlying()) {}
  template <class X>
  constexpr auto operator()(const X& x) const noexcept {
    return expr1(x) + expr2(x);
  }

  constexpr auto Evaluate() const noexcept { return expr1.Evaluate() + expr2.Evaluate(); }

  template <size_t M>
  constexpr auto Derive() const noexcept {
    if constexpr (is_constant_v<E1>)
      return expr2.template Derive<M>();
    else if constexpr (is_constant_v<E2>)
      return expr1.template Derive<M>();
    else
      return expr1.template Derive<M>() + expr2.template Derive<M>();
  }
};

template <class E1, class E2>
class MinusImpl : public Expression<MinusImpl<E1, E2>> {
  const E1 expr1;
  const E2 expr2;

 public:
  MinusImpl(const Expression<E1>& expr1, const Expression<E2>& expr2)
      : expr1(expr1.CastToUnderlying()), expr2(expr2.CastToUnderlying()) {}
  template <class X>
  constexpr auto operator()(const X& x) const noexcept {
    return expr1(x) - expr2(x);
  }

  constexpr auto Evaluate() const noexcept { return expr1.Evaluate() - expr2.Evaluate(); }

  template <size_t M>
  constexpr auto Derive() const noexcept {
    if constexpr (is_constant_v<E1>)
      return -expr2.template Derive<M>();
    else if constexpr (is_constant_v<E2>)
      return expr1.template Derive<M>();
    else
      return expr1.template Derive<M>() - expr2.template Derive<M>();
  }
};

template <class E1, class E2>
class MultiplyImpl : public Expression<MultiplyImpl<E1, E2>> {
  const E1 expr1;
  const E2 expr2;

 public:
  MultiplyImpl(const Expression<E1>& expr1, const Expression<E2>& expr2)
      : expr1(expr1.CastToUnderlying()), expr2(expr2.CastToUnderlying()) {}
  template <class X>
  constexpr auto operator()(const X& x) const noexcept {
    return expr1(x) * expr2(x);
  }

  constexpr auto Evaluate() const noexcept { return expr1.Evaluate() * expr2.Evaluate(); }

  template <size_t M>
  constexpr auto Derive() const noexcept {
    if constexpr (is_constant_v<E1>)
      return expr1 * expr2.template Derive<M>();
    else if constexpr (is_constant_v<E2>)
      return expr1.template Derive<M>() * expr2;
    else
      return expr1 * expr2.template Derive<M>() + expr1.template Derive<M>() * expr2;
  }
};

template <class E1, class E2>
class DivideImpl : public Expression<DivideImpl<E1, E2>> {
  const E1 expr1;
  const E2 expr2;

 public:
  DivideImpl(const Expression<E1>& expr1, const Expression<E2>& expr2)
      : expr1(expr1.CastToUnderlying()), expr2(expr2.CastToUnderlying()) {}
  template <class X>
  constexpr auto operator()(const X& x) const noexcept {
    return expr1(x) / expr2(x);
  }

  constexpr auto Evaluate() const noexcept { return expr1.Evaluate() / expr2.Evaluate(); }

  template <size_t M>
  constexpr auto Derive() const noexcept {
    if constexpr (is_constant_v<E1>)
      return -expr1 * expr2.template Derive<M>() / (expr2 * expr2);
    else if constexpr (is_constant_v<E2>)
      return expr1.template Derive<M>() / expr2;
    else
      return (expr1.template Derive<M>() * expr2 - expr1 * expr2.template Derive<M>()) / (expr2 * expr2);
  }
};
}  // namespace ImplDetails

template <class Arg1, class Arg2>
constexpr auto operator+(const Arg1& arg1, const Arg2& arg2) {
    return Helpers::BinaryExpresionHelper<Arg1, Arg2, ImplDetails::PlusImpl>(arg1, arg2);
}

template <class Arg1, class Arg2>
constexpr auto operator-(const Arg1& arg1, const Arg2& arg2) {
  return Helpers::BinaryExpresionHelper<Arg1, Arg2, ImplDetails::MinusImpl>(arg1, arg2);
}

template <class Arg1, class Arg2>
constexpr auto operator*(const Arg1& arg1, const Arg2& arg2) {
  return Helpers::BinaryExpresionHelper<Arg1, Arg2, ImplDetails::MultiplyImpl>(arg1, arg2);
}

template <class Arg1, class Arg2>
constexpr auto operator/(const Arg1& arg1, const Arg2& arg2) {
  return Helpers::BinaryExpresionHelper<Arg1, Arg2, ImplDetails::DivideImpl>(arg1, arg2);
}
}  // namespace SymbolicMath