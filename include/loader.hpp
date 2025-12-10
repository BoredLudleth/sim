#pragma once

#include <cstdint>
#include <elfio/elfio.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "machine.hpp"

namespace sim {
class Loader final {
private:
  Machine machine_;

public:
  void read_elf(const std::filesystem::path &path);

  void run();
};
} // namespace sim