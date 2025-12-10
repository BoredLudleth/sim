#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <stack>

#include "cached.hpp"
#include "memory.hpp"

namespace sim {
using register_t = uint32_t;
const int n_regs = 32;
const int n_csr = 1024;

class Hart final {
private:
  Cached cache_;

public:
  long n_instructions = 0;
  Hart() { cache_.hart_ = this; }
  std::array<register_t, n_regs> gpr_;
  std::array<register_t, n_csr> csr_;
  Memory *mem_ = nullptr;
  register_t pc;

  void run();

  bool step();

  void set_register(const uint8_t &reg, const register_t &value);

  void set_pc(const register_t &value);

  void set_mem(Memory *mem);

  void dump_registers() const;
};
} // namespace sim