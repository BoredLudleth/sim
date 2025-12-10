#include "loader.hpp"

int main(int argc, char *argv[]) {
  using namespace sim;

  if (argc == 1) {
    throw std::runtime_error("Program file didn't provided");
  }

  Loader loader;
  loader.read_elf(argv[1]);
  loader.run();

  return 0;
}