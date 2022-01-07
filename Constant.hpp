#pragma once

#include "Expression.hpp"

namespace SymbolicMath {
template <class T, T value>
class IntegralConstant : public Expression<IntegralConstant<T, value>> {
 public:  // Constructors
  constexpr IntegralConstant(){};
  constexpr IntegralConstant(const IntegralConstant& expr){};

 public:  // Aliases
  using zero_t = IntegralConstant<T, 0>;
  using unity_t = IntegralConstant<T, 1>;

 public:  // Methods
  constexpr auto Evaluate() const noexcept { return value; }

  template <class... X>
  constexpr auto operator()(const X&... x) const noexcept {
    return value;
  }

  template <unsigned M>
  constexpr auto Derive() const noexcept {
    return IntegralConstant<int, 0>{};
  }
};

template <class T>
constexpr bool is_integral_constant_v = false;

template <class T, T v>
constexpr bool is_integral_constant_v<IntegralConstant<T, v>> = true;

template <class T, T v>
constexpr auto ToExpression(const std::integral_constant<T, v>) noexcept {
  return IntegralConstant<T, v>{};
}

template <class T>
class Constant : public Expression<Constant<T>> {
  static_assert(std::is_arithmetic_v<T>);

 private:  // Members
  const T value;

 public:  // Constructors
  constexpr Constant(const T& value) : value(value){};
  constexpr Constant(const Constant& expr) : value(expr.Evaluate()){};

 public:  // Methods
  constexpr auto Evaluate() const noexcept { return value; }

  template <class... X>
  constexpr auto operator()(const X&... x) const noexcept {
    return value;
  }

  template <unsigned M>
  constexpr auto Derive() const noexcept {
    return IntegralConstant<int, 0>();
  }
};

template <class T>
constexpr bool is_constant_v = false;

template <class T>
constexpr bool is_constant_v<Constant<T>> = true;

template <class T, T v>
constexpr bool is_constant_v<IntegralConstant<T, v>> = true;

template <class T>
constexpr std::enable_if_t<std::is_arithmetic_v<T>, Constant<T>> ToExpression(const T& v) noexcept {
  return Constant<T>(v);
}
}  // namespace SymbolicMath