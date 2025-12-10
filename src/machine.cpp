#include "machine.hpp"
namespace sim {
void Machine::run() {
  hart_.set_mem(&memory_);
  // memory_.dump();
  hart_.run();
}

void Machine::set_pc(const std::uint64_t &pc_val) { hart_.set_pc(pc_val); }

void Machine::add_data(const char *data, const std::uint64_t &size,
                       ELFIO::Elf64_Addr virtual_addr) {
  memory_.store_data(data, size, virtual_addr);
  // add it to mmu
}
} // namespace sim