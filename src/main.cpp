#include "webserver.hpp"

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  TestClass<int> t(1);

  std::cout << t << "\n";
  return 0;
}