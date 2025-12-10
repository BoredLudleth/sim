#include <chrono>
#include <iomanip>

#include "cached.hpp"
#include "generated_instructions.hpp"
#include "hart.hpp"
#include "memory.hpp"

namespace sim {

void Hart::run() {
  gpr_[2] = memory_size - 1;
  auto start = std::chrono::high_resolution_clock::now();
  while (step()) {
  };
  auto end = std::chrono::high_resolution_clock::now();
  double seconds =
      std::chrono::duration_cast<std::chrono::duration<double>>(end - start)
          .count();
  std::cout << "Average perfomance: " << n_instructions / (seconds * 1e6)
            << std::endl;
  dump_registers();
  return;
}
bool Hart::step() {
  if (cache_.execute_from_cache(pc)) {
    return pc < memory_size;
  }

  uint32_t command = mem_->read_word(pc);
  register_t current_pc = pc;

  DecodedInstruction decoded = decode(command);
  if (!decoded.second) {
    cache_.cache_it(current_pc);
  } else {
    decoded.first(this);
    pc += 4;
    ++n_instructions;
    return pc < memory_size;
  }

  cache_.execute_from_cache(pc);

  return pc < memory_size;
}
void Hart::set_register(const uint8_t &reg, const register_t &value) {
  gpr_[reg] = value;
}
void Hart::set_pc(const register_t &value) { pc = value; }
void Hart::set_mem(Memory *mem) { mem_ = mem; }
void Hart::dump_registers() const {
  const char *reg_names[32] = {
      "zero", "ra", "sp", "gp", "tp",  "t0",  "t1", "t2", "s0/fp", "s1", "a0",
      "a1",   "a2", "a3", "a4", "a5",  "a6",  "a7", "s2", "s3",    "s4", "s5",
      "s6",   "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5",    "t6"};

  std::cout << "=== REGISTER DUMP ===" << std::endl;
  std::cout << "PC: 0x" << std::hex << std::setw(8) << std::setfill('0') << pc
            << " (" << std::dec << pc << ")" << std::endl;
  std::cout << std::endl;

  for (int i = 0; i < 32; i++) {
    std::cout << "x" << std::setw(2) << std::setfill('0') << i << " ("
              << std::setw(5) << std::setfill(' ') << std::left << reg_names[i]
              << std::right << "): " << "0x" << std::hex << std::setw(8)
              << std::setfill('0') << gpr_[i] << " (" << std::dec
              << std::setw(11) << std::setfill(' ')
              << static_cast<int32_t>(gpr_[i]) << ")";
    if ((i + 1) % 2 == 0) {
      std::cout << std::endl;
    } else {
      std::cout << " | ";
    }
  }
  std::cout << "=====================" << std::endl << std::endl;
}
} // namespace sim