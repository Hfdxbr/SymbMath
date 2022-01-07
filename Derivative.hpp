#include "Expression.hpp"
#include "Variable.hpp"

namespace SymbolicMath {
template <class E, unsigned id, class... Rest>
constexpr auto D(const Expression<E>& expr, const Variable<id>& v, Rest... rest) {
  return D(expr.CastToUnderlying().template Derive<v>(), rest...);
}

template <class E, unsigned id>
constexpr auto D(const Expression<E>& expr, const Variable<id>& v) {
  return expr.CastToUnderlying().template Derive<v>();
}
}  // namespace SymbolicMath