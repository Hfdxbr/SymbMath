#include "BasicExpression.hpp"
#include "BinaryExpression.hpp"
#include "UnaryExpression.hpp"

namespace SymbolicMath {
template <class E, size_t id, class... Rest>
constexpr auto D(const Expression<E>& expr, const Variable<id>& v, Rest... rest) {
  return D(expr.CastToUnderlying().template Derive<v>(), rest...);
}

template <class E, size_t id>
constexpr auto D(const Expression<E>& expr, const Variable<id>& v) {
  return expr.CastToUnderlying().template Derive<v>();
}
}  // namespace SymbolicMath