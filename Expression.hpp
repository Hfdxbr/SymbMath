#pragma once

#include <type_traits>
#include <utility>

namespace SymbolicMath {
template <class E>
struct Expression {
  constexpr E& CastToUnderlying() { return static_cast<E&>(*this); }
  constexpr const E& CastToUnderlying() const { return static_cast<const E&>(*this); }
};

template <class E>
constexpr auto ToExpression(const Expression<E>& expr) noexcept {
  return expr.CastToUnderlying();
}

template <class E, template <class> class Impl>
class UnaryExpression : public Expression<Impl<E>> {
 protected:  // Members
  const E expr;

 public:  // Members
  static constexpr bool is_constexpr_v = E::is_constexpr_v;

 public:  // Constructors
  constexpr UnaryExpression(const Expression<E>& expr) noexcept : expr(expr.CastToUnderlying()) {}

 public:  // Methods
  constexpr auto GetSub() const noexcept { return expr; }

  constexpr auto Evaluate() const { return Impl<E>::func(expr.Evaluate()); }

  template <class... X>
  constexpr auto operator()(const X&... x) const {
    return Impl<E>::func(expr(x...));
  }

  template <unsigned M>
  constexpr auto Derive() const noexcept {
    return Impl<E>::deriv(expr, expr.template Derive<M>());
  }
};

template <class E>
constexpr bool is_unary_expression_v =
    std::is_base_of_v<Expression<E>, E> ? is_unary_expression_v<Expression<E>> : false;

template <class E>
constexpr bool is_unary_expression_v<Expression<E>> = false;

template <class E, template <class> class Impl>
constexpr bool is_unary_expression_v<Expression<Impl<E>>> = true;

template <class E1, class E2, template <class, class> class Impl>
class BinaryExpression : public Expression<Impl<E1, E2>> {
 protected:  // Members
  const E1 expr1;
  const E2 expr2;

 public:  // Members
  static constexpr bool is_constexpr_v = E1::is_constexpr_v && E2::is_constexpr_v;

 public:  // Constructors
  constexpr BinaryExpression(const Expression<E1>& expr1, const Expression<E2>& expr2) noexcept
      : expr1(expr1.CastToUnderlying()), expr2(expr2.CastToUnderlying()) {}

 public:  // Methods
  constexpr auto GetSubs() const noexcept { return std::make_pair(expr1, expr2); }

  constexpr auto Evaluate() const { return Impl<E1, E2>::func(expr1.Evaluate(), expr2.Evaluate()); }

  template <class... X>
  constexpr auto operator()(const X&... x) const {
    return Impl<E1, E2>::func(expr1(x...), expr2(x...));
  }

  template <unsigned M>
  constexpr auto Derive() const noexcept {
    return Impl<E1, E2>::deriv(expr1, expr1.template Derive<M>(), expr2, expr2.template Derive<M>());
  }
};

template <class E>
constexpr bool is_binary_expression_v =
    std::is_base_of_v<Expression<E>, E> ? is_binary_expression_v<Expression<E>> : false;

template <class E>
constexpr bool is_binary_expression_v<Expression<E>> = false;

template <class E1, class E2, template <class, class> class Impl>
constexpr bool is_binary_expression_v<Expression<Impl<E1, E2>>> = true;
}  // namespace SymbolicMath