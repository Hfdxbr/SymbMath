#pragma optimize("", off)
#include "SymbolicMath.hpp"

#include <iostream>

template <class E, class Arg, class... Args>
void EvaluateAt(const E& expr, Arg&& arg, Args&&... args) {
  using namespace SymbolicMath;
  std::cout << GetTypeName(expr) << "(" << arg;
  ((std::cout << ',' << args), ...);
  std::cout << ")=" << expr(arg, args...) << std::endl << std::endl;
}

int main() {
  using namespace SymbolicMath;
  auto [x, y] = MakeVariables<2>();
  auto expr1 = Cos(x + y) / Cos(y + x) + Sin(x + y * x) - Sin(x * y + x);
  auto expr2 = Exp(x + y) - (Sin(x) + Exp(y + x));
  auto expr3 = D(Exp(x) + Sin(Cos(x + y)), y);
  auto expr4 = D(x, x) + IntegralConstant<short, 1>{};
  auto expr5 = Sin(expr4) + IntegralConstant<int, 1>{};

  EvaluateAt(expr1, 1, 1);
  EvaluateAt(expr2, 1, 1);
  EvaluateAt(expr3, 1, 1);
  EvaluateAt(expr4, 1, 1);
  EvaluateAt(expr5, 1, 1);
  return 0;
}