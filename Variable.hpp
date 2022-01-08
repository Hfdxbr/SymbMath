#pragma once

#include "Expression.hpp"

#include <tuple>
#include <utility>

namespace SymbolicMath {
template <unsigned N>
class Variable : public Expression<Variable<N>> {
 public:  // Members
  static constexpr bool is_constexpr_v = true;

 public:  // Constructors
  Variable(){};

 public:  // Methods
  template <class... X>
  constexpr auto operator()(const X&... x) const noexcept {
    return std::get<N>(std::forward_as_tuple(x...));
  }

  template <unsigned M>
  constexpr auto Derive() const noexcept {
    if constexpr (M == N)
      return IntegralConstant<int, 1>();
    else
      return IntegralConstant<int, 0>();
  }

  constexpr operator unsigned() const { return N; }
};

template <unsigned... I>
constexpr auto MakeVariables(std::integer_sequence<unsigned, I...>) {
  return std::make_tuple(Variable<I>()...);
}

template <unsigned N, class Indexes = std::make_integer_sequence<unsigned, N>>
constexpr auto MakeVariables() {
  return MakeVariables(Indexes{});
}
}  // namespace SymbolicMath