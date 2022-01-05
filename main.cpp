#include <iostream>
#include "SymbolicMath.hpp"

int main() {
  using namespace SymbolicMath;
  Variable<0> x;
  auto expr1 = Exp(x) + Sin(Cos(x + 2));
  auto expr2 = D(expr1, x);
  std::cout << expr1(1) << std::endl;
  std::cout << expr2(1) << std::endl;
  return 0;
}