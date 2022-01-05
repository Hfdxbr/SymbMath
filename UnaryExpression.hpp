#pragma once
#include <cmath>

#include "BasicExpression.hpp"
#include "BinaryExpression.hpp"

namespace SymbolicMath {
namespace Helpers {
template <class E, template <class> class Unary>
constexpr auto UnaryExpresionHelper(const Expression<E>& expr) {
  if constexpr (is_constant_v<E>)
    return Constant(Unary(expr).Evaluate());
  else
    return Unary(expr);
}

template <class T, template <class> class Unary, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto UnaryExpresionHelper(const T& value) {
  return UnaryExpresionHelper<Constant<T>, Unary>(Constant(value));
}
}  // namespace Helpers

namespace ImplDetails {
template <class E>
class InvImpl : public Expression<InvImpl<E>> {
  const E expr;

 public:
  InvImpl(const Expression<E>& expr) : expr(expr.CastToUnderlying()) {}
  template <class X>
  constexpr auto operator()(const X& x) const noexcept {
    return -expr(x);
  }

  constexpr auto Evaluate() const noexcept { return -expr.Evaluate(); }

  template <size_t M>
  constexpr auto Derive() const noexcept {
    return -expr.template Derive<M>();
  }
};

template <class E>
class CosImpl : public Expression<CosImpl<E>> {
  const E expr;

 public:
  CosImpl(const Expression<E>& expr) : expr(expr.CastToUnderlying()) {}
  template <class X>
  constexpr auto operator()(const X& x) const noexcept {
    return std::cos(expr(x));
  }

  constexpr auto Evaluate() const noexcept { return std::cos(expr.Evaluate()); }

  template <size_t M>
  constexpr auto Derive() const noexcept {
    return -Sin(expr) * expr.template Derive<M>();
  }
};

template <class E>
class SinImpl : public Expression<SinImpl<E>> {
  const E expr;

 public:
  SinImpl(const Expression<E>& expr) : expr(expr.CastToUnderlying()) {}
  template <class X>
  constexpr auto operator()(const X& x) const noexcept {
    return std::sin(expr(x));
  }

  constexpr auto Evaluate() const noexcept { return std::sin(expr.Evaluate()); }

  template <size_t M>
  constexpr auto Derive() const noexcept {
    return Cos(expr) * expr.template Derive<M>();
  }
};

template <class E>
class TanImpl : public Expression<TanImpl<E>> {
  const E expr;

 public:
  TanImpl(const Expression<E>& expr) : expr(expr.CastToUnderlying()) {}
  template <class X>
  constexpr auto operator()(const X& x) const noexcept {
    return std::tan(expr(x));
  }

  constexpr auto Evaluate() const noexcept { return std::tan(expr.Evaluate()); }

  template <size_t M>
  constexpr auto Derive() const noexcept {
    return expr.template Derive<M>() / (Cos(expr) * Cos(expr));
  }
};

template <class E>
class CotImpl : public Expression<CotImpl<E>> {
  const E expr;

 public:
  CotImpl(const Expression<E>& expr) : expr(expr.CastToUnderlying()) {}
  template <class X>
  constexpr auto operator()(const X& x) const noexcept {
    return 1. / std::tan(expr(x));
  }

  constexpr auto Evaluate() const noexcept { return 1. / std::tan(expr.Evaluate()); }

  template <size_t M>
  constexpr auto Derive() const noexcept {
    return -expr.template Derive<M>() / (Sin(expr) * Sin(expr));
  }
};

template <class E>
class ExpImpl : public Expression<ExpImpl<E>> {
  const E expr;

 public:
  ExpImpl(const Expression<E>& expr) : expr(expr.CastToUnderlying()) {}
  template <class X>
  constexpr auto operator()(const X& x) const noexcept {
    return std::exp(expr(x));
  }

  constexpr auto Evaluate() const noexcept { return std::exp(expr.Evaluate()); }

  template <size_t M>
  constexpr auto Derive() const noexcept {
    return ExpImpl(expr) * expr.template Derive<M>();
  }
};

template <class E>
class LogImpl : public Expression<LogImpl<E>> {
  const E expr;

 public:
  LogImpl(const Expression<E>& expr) : expr(expr.CastToUnderlying()) {}
  template <class X>
  constexpr auto operator()(const X& x) const noexcept {
    return std::log(expr(x));
  }

  constexpr auto Evaluate() const noexcept { return std::log(expr.Evaluate()); }

  template <size_t M>
  constexpr auto Derive() const noexcept {
    return expr.template Derive<M>() / expr;
  }
};
}  // namespace ImplDetails

template <class Arg>
constexpr auto operator-(const Arg& arg) {
  return Helpers::UnaryExpresionHelper<Arg, ImplDetails::InvImpl>(arg);
}

template <class Arg>
constexpr auto Cos(const Arg& arg) {
  return Helpers::UnaryExpresionHelper<Arg, ImplDetails::CosImpl>(arg);
}

template <class Arg>
constexpr auto Sin(const Arg& arg) {
  return Helpers::UnaryExpresionHelper<Arg, ImplDetails::SinImpl>(arg);
}

template <class Arg>
constexpr auto Tan(const Arg& arg) {
  return Helpers::UnaryExpresionHelper<Arg, ImplDetails::TanImpl>(arg);
}

template <class Arg>
constexpr auto Cot(const Arg& arg) {
  return Helpers::UnaryExpresionHelper<Arg, ImplDetails::CotImpl>(arg);
}

template <class Arg>
constexpr auto Exp(const Arg& arg) {
  return Helpers::UnaryExpresionHelper<Arg, ImplDetails::ExpImpl>(arg);
}

template <class Arg>
constexpr auto Log(const Arg& arg) {
  return Helpers::UnaryExpresionHelper<Arg, ImplDetails::LogImpl>(arg);
}
}  // namespace SymbolicMath