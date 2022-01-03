#include <cmath>
#include <type_traits>

template <class E>
struct Expression {
  constexpr E& operator()() { return static_cast<E&>(*this); }
  constexpr const E& operator()() const { return static_cast<const E&>(*this); }
};

template <class T>
struct Constant : Expression<Constant<T>> {
  Constant(const T& value) : value{value} {}
  template <class Arg>
  constexpr const T& operator()(const Arg& x) const {
    return value;
  }

  const T value;
};

template <class T>
struct Variable : Expression<Variable<T>> {
  template <class Arg>
  constexpr T operator()(const Arg& x) const {
    return x;
  }
};

template <class E, class F>
class UnaryExpression : public Expression<UnaryExpression<E, F>> {
  const E expr;
  const F func;

 public:
  UnaryExpression(const Expression<E>& expr, const F& func) : expr{expr()}, func(func) {}
  template <class Arg>
  constexpr auto operator()(const Arg& x) const -> decltype(func(expr(x))) {
    return func(expr(x));
  }
};

template <class T, class F, class = typename std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto UnaryExpresionHelper(const T& value, const F& func) {
  return UnaryExpression<Constant<T>, F>(Constant(value), func);
}

template <class E, class F>
constexpr auto UnaryExpresionHelper(const Expression<E>& expr, const F& func) {
  return UnaryExpression<E, F>(expr, func);
}

template <class T>
constexpr auto Cos(const T& arg) {
  return UnaryExpresionHelper(arg, [](auto x) { return std::cos(x); });
}

template <class T>
constexpr auto Sin(const T& arg) {
  return UnaryExpresionHelper(arg, [](auto x) { return std::sin(x); });
}

template <class T>
constexpr auto Tan(const T& arg) {
  return UnaryExpresionHelper(arg, [](auto x) { return std::tan(x); });
}

template <class T>
constexpr auto Cot(const T& arg) {
  return UnaryExpresionHelper(arg, [](auto x) { return 1. / std::tan(x); });
}

template <class T>
constexpr auto Exp(const T& arg) {
  return UnaryExpresionHelper(arg, [](auto x) { return std::exp(x); });
}

template <class T>
constexpr auto Log(const T& arg) {
  return UnaryExpresionHelper(arg, [](auto x) { return std::log(x); });
}

template <class T>
constexpr auto operator-(const T& arg) {
  return UnaryExpresionHelper(arg, [](auto x) { return -x; });
}

template <class E1, class E2, class F>
class BinaryExpression : public Expression<BinaryExpression<E1, E2, F>> {
  const E1 expr1;
  const E2 expr2;
  const F func;

 public:
  BinaryExpression(const Expression<E1>& expr1, const Expression<E2>& expr2, const F& func)
      : expr1{expr1()}, expr2{expr2()}, func(func) {}
  template <class Arg>
  constexpr auto operator()(const Arg& x) const -> decltype(func(expr1(x), expr2(x))) {
    return func(expr1(x), expr2(x));
  }
};

template <class T, class E, class F, class = typename std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto BinaryExpresionHelper(const T& value, const Expression<E>& expr, const F& func) {
  return BinaryExpression<Constant<T>, E, F>(Constant(value), expr, func);
}

template <class E, class T, class F, class = typename std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto BinaryExpresionHelper(const Expression<E>& expr, const T& value, const F& func) {
  return BinaryExpression<E, Constant<T>, F>(expr, Constant(value), func);
}

template <class E1, class E2, class F>
constexpr auto BinaryExpresionHelper(const Expression<E1>& expr1, const Expression<E2>& expr2, const F& f) {
  return BinaryExpression<E1, E2, F>(expr1, expr2, f);
}

template <class T1, class T2>
constexpr auto operator+(const T1& arg1, const T2& arg2) {
  return BinaryExpresionHelper(arg1, arg2, [](auto x, auto y) { return x + y; });
}

template <class T1, class T2>
constexpr auto operator-(const T1& arg1, const T2& arg2) {
  return BinaryExpresionHelper(arg1, arg2, [](auto x, auto y) { return x - y; });
}

template <class T1, class T2>
constexpr auto operator*(const T1& arg1, const T2& arg2) {
  return BinaryExpresionHelper(arg1, arg2, [](auto x, auto y) { return x * y; });
}

template <class T1, class T2>
constexpr auto operator/(const T1& arg1, const T2& arg2) {
  return BinaryExpresionHelper(arg1, arg2, [](auto x, auto y) { return x / y; });
}