#include "SymbolicMath.hpp"

#include <iostream>

int main() {
  using namespace SymbolicMath;
  auto [x, y] = MakeVariables<2>();
  auto expr1 = D(Cos(x + y), y);
  auto expr2 = D(Sin(Cos(x + y)), y);
  auto expr3 = D(Exp(x) + Sin(Cos(x + y)), y);
  std::cout << GetTypeName(expr1) << "(1,1)=" << expr1(1, 1) << std::endl << std::endl << std::endl;
  std::cout << GetTypeName(expr2) << "(1,1)=" << expr2(1, 1) << std::endl << std::endl << std::endl;
  std::cout << GetTypeName(expr3) << "(1,1)=" << expr3(1, 1) << std::endl << std::endl << std::endl;
  return 0;
}