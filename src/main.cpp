#include <matcher.h>
#include <order_book.h>

#include <iostream>
#include <ostream>

int main() {
  double coldms = matcher();
  double warmms = matcher();
  double ms = matcher();

  std::cout << coldms << std::endl;
  std::cout << warmms << std::endl;
  std::cout << ms << std::endl;

  return 0;
}
