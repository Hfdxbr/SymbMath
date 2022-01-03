#include <iostream>
#include "test.hpp"

int main() {
  Variable<double> x;
  auto expr1 = Cos(x + 2) + 2. * Sin(x - 1);
  auto expr2 = -Exp(x);
  std::cout << expr1(0) << std::endl;
  std::cout << expr2(0) << std::endl;
  return 0;
}