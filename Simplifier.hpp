#pragma once

#include "Expression.hpp"

#include "Constant.hpp"
#include "Variable.hpp"

namespace SymbolicMath::ImplDetails {
template <class E>
class _Negate;

template <class E1, class E2>
class _Plus;

template <class E1, class E2>
class _Minus;

template <class E1, class E2>
class _Multiply;

template <class E1, class E2>
class _Divide;
}  // namespace SymbolicMath::ImplDetails

namespace SymbolicMath {
template <class E>
constexpr auto Simplify(const Expression<E>& expr) {
  return expr.CastToUnderlying();
}

template <class T, T v>
constexpr auto Simplify(const IntegralConstant<T, v>& expr) {
  return expr;
}

template <class T>
constexpr auto Simplify(const Constant<T>& expr) {
  return expr;
}

template <unsigned N>
constexpr auto Simplify(const Variable<N>& expr) {
  return expr;
}

template <class T, T v, template <class> class Unary>
constexpr auto Simplify(const Expression<Unary<IntegralConstant<T, v>>>& expr) {
  constexpr auto output = expr.CastToUnderlying().Evaluate();
  return IntegralConstant<decltype(output), output>{};
}

template <class T, template <class> class Unary>
constexpr auto Simplify(const Expression<Unary<Constant<T>>>& expr) {
  constexpr auto output = expr.CastToUnderlying().Evaluate();
  return Constant(output);
}

template <class E>
constexpr auto Simplify(const ImplDetails::_Negate<ImplDetails::_Negate<E>>& expr) {
  return expr.GetSub().GetSub();
}

template <class T1, T1 v1, class T2, T2 v2, template <class, class> class Binary>
constexpr auto Simplify(const Expression<Binary<IntegralConstant<T1, v1>, IntegralConstant<T2, v2>>>& expr) {
  constexpr auto output = expr.CastToUnderlying().Evaluate();
  return IntegralConstant<decltype(output), output>{};
}

template <class T1, T1 v1, class T2, template <class, class> class Binary>
constexpr auto Simplify(const Expression<Binary<IntegralConstant<T1, v1>, Constant<T2>>>& expr) {
  constexpr auto output = expr.CastToUnderlying().Evaluate();
  return Constant(output);
}

template <class T1, class T2, T2 v2, template <class, class> class Binary>
constexpr auto Simplify(const Expression<Binary<Constant<T1>, IntegralConstant<T2, v2>>>& expr) {
  constexpr auto output = expr.CastToUnderlying().Evaluate();
  return Constant(output);
}

template <class T1, class T2, template <class, class> class Binary>
constexpr auto Simplify(const Expression<Binary<Constant<T1>, Constant<T2>>>& expr) {
  constexpr auto output = expr.CastToUnderlying().Evaluate();
  return Constant(output);
}

template <class T, class E>
constexpr auto Simplify(const ImplDetails::_Plus<IntegralConstant<T, 0>, E>& expr) {
  auto [_, e] = expr.GetSubs();
  return e;
}

template <class E, class T>
constexpr auto Simplify(const ImplDetails::_Plus<E, IntegralConstant<T, 0>>& expr) {
  auto [e, _] = expr.GetSubs();
  return e;
}

template <class T, class E>
constexpr auto Simplify(const ImplDetails::_Minus<IntegralConstant<T, 0>, E>& expr) {
  auto [_, e] = expr.GetSubs();
  return -e;
}

template <class E, class T>
constexpr auto Simplify(const ImplDetails::_Minus<E, IntegralConstant<T, 0>>& expr) {
  auto [e, _] = expr.GetSubs();
  return e;
}

template <class T, class E>
constexpr auto Simplify(const ImplDetails::_Multiply<IntegralConstant<T, 1>, E>& expr) {
  auto [_, e] = expr.GetSubs();
  return e;
}

template <class E, class T>
constexpr auto Simplify(const ImplDetails::_Multiply<E, IntegralConstant<T, 1>>& expr) {
  auto [e, _] = expr.GetSubs();
  return e;
}

template <class T, class E>
constexpr auto Simplify(const ImplDetails::_Multiply<IntegralConstant<T, 0>, E>& expr) {
  return IntegralConstant<int, 0>{};
}

template <class E, class T>
constexpr auto Simplify(const ImplDetails::_Multiply<E, IntegralConstant<T, 0>>& expr) {
  return IntegralConstant<int, 0>{};
}

template <class E, class T>
constexpr auto Simplify(const ImplDetails::_Divide<E, IntegralConstant<T, 1>>& expr) {
  auto [e, _] = expr.GetSubs();
  return e;
}

template <class T, class E>
constexpr auto Simplify(const ImplDetails::_Divide<IntegralConstant<T, 0>, E>& expr) {
  return IntegralConstant<int, 0>{};
}
}  // namespace SymbolicMath