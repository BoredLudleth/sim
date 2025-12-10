#pragma once

#include "hart.hpp"
#include "memory.hpp"
#include <elfio/elfio.hpp>

namespace sim {
class Machine final {
private:
  Hart hart_;

public:
  Memory memory_;
  void run();

  void set_pc(const std::uint64_t &pc_val);

  void add_data(const char *data, const std::uint64_t &size,
                ELFIO::Elf64_Addr virtual_addr);
};
} // namespace sim
