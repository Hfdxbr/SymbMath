#pragma once

#include <cassert>
#include <type_traits>

namespace SymbolicMath {
template <class E>
struct Expression {
  constexpr E& CastToUnderlying() { return static_cast<E&>(*this); }
  constexpr const E& CastToUnderlying() const { return static_cast<const E&>(*this); }
};

template <class T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
class Constant : public Expression<Constant<T>> {
  const T value;

 public:
  Constant(const T& value) : value(value){};
  Constant(const Constant<T>& expr) : value(expr.Evaluate()){};

  template <class X>
  constexpr auto operator()(const X&) const noexcept {
    return value;
  }

  constexpr auto Evaluate() const noexcept { return value; }

  template <size_t M>
  constexpr auto Derive() const noexcept {
    return Constant(0);
  }
};

template <class E>
struct ConstantChecker : std::false_type {};

template <class T>
struct ConstantChecker<Constant<T>> : std::true_type {};

template <class E>
constexpr bool is_constant_v = ConstantChecker<E>::value;

template <class E, class T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr bool operator==(const Expression<E>& expr, const T& value) {
  return false;
}

template <class T1, class T2, class = std::enable_if_t<std::is_arithmetic_v<T2>>>
constexpr bool operator==(const Constant<T1>& expr, const T2& value) {
  return expr.CastToUnderlying().Evaluate() == value;
}

template <size_t N>
class Variable : public Expression<Variable<N>> {
 public:
  Variable(){};

  template <class X>
  constexpr auto operator()(const X& x) const noexcept {
    return x;
  }

  constexpr auto Evaluate() const noexcept { assert(false && "Variable expression cannot be evaluated"); }

  template <size_t M>
  constexpr auto Derive() const noexcept {
    if constexpr (M == N)
      return Constant(1);
    else
      return Constant(0);
  }

  constexpr operator size_t() const { return N; }
};
}  // namespace SymbolicMath