#pragma once

#include "Expression.hpp"

#include "Constant.hpp"
#include "Variable.hpp"

#include <cassert>

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

template <class E1, class E2>
constexpr bool is_same_template_v = false;

template <template <class...> class T, class... E1, class... E2>
constexpr bool is_same_template_v<T<E1...>, T<E2...>> = true;

template <unsigned N, class T>
struct ParameterExtractor {
  using type = T;
};
template <unsigned N, template <class...> class T, class... E>
struct ParameterExtractor<N, T<E...>> {
  using type = std::tuple_element_t<N, std::tuple<E...>>;
};
template <unsigned N, class T>
using nth_parameter_t = typename ParameterExtractor<N, T>::type;

template <class E1, class E2>
struct IsSameExpression {
  static constexpr bool Check() noexcept {
    if constexpr (std::is_same_v<E1, E2>) {
      return true;
    } else if constexpr (is_same_template_v<E1, E2>) {
      if constexpr (is_unary_expression_v<E1> && is_unary_expression_v<E2>) {
        return IsSameExpression<nth_parameter_t<0, E1>, nth_parameter_t<0, E2>>::Check();
      } else if constexpr (is_binary_expression_v<E1> && is_binary_expression_v<E2>) {
        return (IsSameExpression<nth_parameter_t<0, E1>, nth_parameter_t<0, E2>>::Check() &&
                IsSameExpression<nth_parameter_t<1, E1>, nth_parameter_t<1, E2>>::Check());
      } else {
        assert(false && "unknown expression type");
      }
    } else {
      return false;
    }
  }
};

template <class E11, class E12, class E21, class E22>
struct IsSameExpression<_Plus<E11, E12>, _Plus<E21, E22>> {
  static constexpr bool Check() noexcept {
    return IsSameExpression<E11, E21>::Check() && IsSameExpression<E12, E22>::Check() ||
           IsSameExpression<E11, E22>::Check() && IsSameExpression<E12, E21>::Check();
  }
};

template <class E11, class E12, class E21, class E22>
struct IsSameExpression<_Multiply<E11, E12>, _Multiply<E21, E22>> {
  static constexpr bool Check() noexcept {
    return IsSameExpression<E11, E21>::Check() && IsSameExpression<E12, E22>::Check() ||
           IsSameExpression<E11, E22>::Check() && IsSameExpression<E12, E21>::Check();
  }
};
template <class T, T v, template <class> class Unary>
constexpr auto EvaluateU = Unary<IntegralConstant<T, v>>(IntegralConstant<T, v>()).Evaluate();

template <class T1, T1 v1, class T2, T2 v2, template <class, class> class Binary>
constexpr auto EvaluateB = Binary<IntegralConstant<T1, v1>, IntegralConstant<T2, v2>>(IntegralConstant<T1, v1>(),
                                                                                      IntegralConstant<T2, v2>())
                               .Evaluate();
}  // namespace SymbolicMath::ImplDetails

namespace SymbolicMath {
template <class E1, class E2>
constexpr bool is_same_expression_v =
    E1::is_constexpr_v&& E2::is_constexpr_v&& ImplDetails::IsSameExpression<E1, E2>::Check();

template <class E11, class E12, class E2>
constexpr bool is_commutative_simplification_suitable_v =
    is_same_expression_v<E11, E2> || is_same_expression_v<E12, E2>;

template <class T>
constexpr bool is_integrally_evaluated_v = false;

template <class T, T v, template <class> class Unary>
constexpr bool is_integrally_evaluated_v<Unary<IntegralConstant<T, v>>> =
    std::is_integral_v<decltype(ImplDetails::EvaluateU<T, v, Unary>)>;

template <class T1, T1 v1, class T2, T2 v2, template <class, class> class Binary>
constexpr bool is_integrally_evaluated_v<Binary<IntegralConstant<T1, v1>, IntegralConstant<T2, v2>>> =
    std::is_integral_v<decltype(ImplDetails::EvaluateB<T1, v1, T2, v2, Binary>)>;

template <class E>
constexpr auto Simplify(const Expression<E>& expr) noexcept {
  return expr.CastToUnderlying();
}

template <class T, T v>
constexpr auto Simplify(const IntegralConstant<T, v>& expr) noexcept {
  return expr;
}

template <class T>
constexpr auto Simplify(const Constant<T>& expr) noexcept {
  return expr;
}

template <unsigned N>
constexpr auto Simplify(const Variable<N>& expr) noexcept {
  return expr;
}

// clang-format off
template <class T, T v,
          template <class> class Unary,
          class = std::enable_if_t<is_integrally_evaluated_v<Unary<IntegralConstant<T, v>>>>>
constexpr auto Simplify(const Expression<Unary<IntegralConstant<T, v>>>& expr) {
  constexpr auto output = expr.CastToUnderlying().Evaluate();
  return IntegralConstant<decltype(output), output>{};
}
// clang-format on

template <class T, template <class> class Unary, class = std::enable_if_t<is_constant_v<T>>>
constexpr auto Simplify(const Expression<Unary<T>>& expr) {
  auto output = expr.CastToUnderlying().Evaluate();
  return Constant<decltype(output)>(output);
}

template <class E>
constexpr auto Simplify(const ImplDetails::_Negate<ImplDetails::_Negate<E>>& expr) noexcept {
  return expr.GetSub().GetSub();
}

// clang-format off
template <class T1, T1 v1, class T2, T2 v2,
          template <class, class> class Binary,
          class = std::enable_if_t<is_integrally_evaluated_v<Binary<IntegralConstant<T1, v1>, IntegralConstant<T2, v2>>>>>
constexpr auto Simplify(const Expression<Binary<IntegralConstant<T1, v1>, IntegralConstant<T2, v2>>>& expr) {
  constexpr auto output = expr.CastToUnderlying().Evaluate();
  return IntegralConstant<decltype(output), output>{};
}
// clang-format on

// clang-format off
template <class E1, class E2, 
          template <class, class> class Binary,
          class = std::enable_if_t<is_constant_v<E1> && is_constant_v<E2>>>
constexpr auto Simplify(const Expression<Binary<E1, E2>>& expr) {
  auto output = expr.CastToUnderlying().Evaluate();
  return Constant<decltype(output)>(output);
}
// clang-format on

template <class E, class T>
constexpr auto Simplify(const ImplDetails::_Plus<E, IntegralConstant<T, 0>>& expr) noexcept {
  auto [e, _] = expr.GetSubs();
  return e;
}

template <class T, class E>
constexpr auto Simplify(const ImplDetails::_Minus<IntegralConstant<T, 0>, E>& expr) noexcept {
  auto [_, e] = expr.GetSubs();
  return -e;
}

template <class E, class T>
constexpr auto Simplify(const ImplDetails::_Minus<E, IntegralConstant<T, 0>>& expr) noexcept {
  auto [e, _] = expr.GetSubs();
  return e;
}

template <class E, class = std::enable_if_t<E::is_constexpr_v>>
constexpr auto Simplify(const ImplDetails::_Minus<E, E>& expr) noexcept {
  return IntegralConstant<int, 0>{};
}

template <class E1, class E2, class = std::enable_if_t<is_same_expression_v<E1, E2>>>
constexpr auto Simplify(const ImplDetails::_Minus<E1, E2>& expr) noexcept {
  return IntegralConstant<int, 0>{};
}

template <class E1, class E2, class E3, class = std::enable_if_t<is_commutative_simplification_suitable_v<E1, E2, E3>>>
constexpr auto Simplify(const ImplDetails::_Minus<ImplDetails::_Plus<E1, E2>, E3>& expr) noexcept {
  constexpr bool same_left = is_same_expression_v<E1, E3>;
  auto e = expr.GetSubs().first.GetSubs();
  if constexpr (same_left)
    return e.second;
  else
    return e.first;
}

template <class E1, class E2, class E3, class = std::enable_if_t<is_commutative_simplification_suitable_v<E2, E3, E1>>>
constexpr auto Simplify(const ImplDetails::_Minus<E1, ImplDetails::_Plus<E2, E3>>& expr) noexcept {
  constexpr bool same_left = is_same_expression_v<E1, E2>;
  auto e = expr.GetSubs().second.GetSubs();
  if constexpr (same_left)
    return -e.second;
  else
    return -e.first;
}

template <class T, class E>
constexpr auto Simplify(const ImplDetails::_Multiply<IntegralConstant<T, 1>, E>& expr) noexcept {
  auto [_, e] = expr.GetSubs();
  return e;
}

template <class E, class T>
constexpr auto Simplify(const ImplDetails::_Multiply<E, IntegralConstant<T, 1>>& expr) noexcept {
  auto [e, _] = expr.GetSubs();
  return e;
}

template <class T, class E>
constexpr auto Simplify(const ImplDetails::_Multiply<IntegralConstant<T, 0>, E>& expr) noexcept {
  return IntegralConstant<int, 0>{};
}

template <class E, class T>
constexpr auto Simplify(const ImplDetails::_Multiply<E, IntegralConstant<T, 0>>& expr) noexcept {
  return IntegralConstant<int, 0>{};
}

template <class E, class T>
constexpr auto Simplify(const ImplDetails::_Divide<E, IntegralConstant<T, 1>>& expr) noexcept {
  auto [e, _] = expr.GetSubs();
  return e;
}

template <class T, class E>
constexpr auto Simplify(const ImplDetails::_Divide<IntegralConstant<T, 0>, E>& expr) noexcept {
  return IntegralConstant<int, 0>{};
}

template <class E, class = std::enable_if_t<E::is_constexpr_v>>
constexpr auto Simplify(const ImplDetails::_Divide<E, E>& expr) noexcept {
  return IntegralConstant<int, 1>{};
}

template <class E1, class E2, class E3, class = std::enable_if_t<is_commutative_simplification_suitable_v<E1, E2, E3>>>
constexpr auto Simplify(const ImplDetails::_Divide<ImplDetails::_Multiply<E1, E2>, E3>& expr) noexcept {
  constexpr bool same_left = is_same_expression_v<E1, E3>;
  auto e = expr.GetSubs().first.GetSubs();
  if constexpr (same_left)
    return e.second;
  else
    return e.first;
}

template <class E1, class E2, class E3, class = std::enable_if_t<is_commutative_simplification_suitable_v<E2, E3, E1>>>
constexpr auto Simplify(const ImplDetails::_Divide<E1, ImplDetails::_Multiply<E2, E3>>& expr) noexcept {
  constexpr bool same_left = is_same_expression_v<E1, E2>;
  auto e = expr.GetSubs().second.GetSubs();
  if constexpr (same_left)
    return IntegralConstant<int, 1>{} / e.second;
  else
    return IntegralConstant<int, 1>{} / e.first;
}

template <class E1, class E2, class = std::enable_if_t<is_same_expression_v<E1, E2>>>
constexpr auto Simplify(const ImplDetails::_Divide<E1, E2>& expr) noexcept {
  return IntegralConstant<int, 1>{};
}
}  // namespace SymbolicMath