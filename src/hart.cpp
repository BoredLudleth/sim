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
  mem_->set_hart(this);

#if ENABLE_MMU
  mmu_enabled_ = true;
  mmu_.set_hart(this);
  mmu_.set_satp(0x80002000);
  create_page_table(*mem_, 0x2000000);
#else
  mmu_enabled_ = false;
#endif

  while (step()) {
  };
  auto end = std::chrono::high_resolution_clock::now();
  double seconds =
      std::chrono::duration_cast<std::chrono::duration<double>>(end - start)
          .count();
  std::cout << "Total time: " << seconds << " s" << std::endl;
  std::cout << "Number of instructions: " << std::dec << n_instructions
            << std::endl;
  std::cout << "Average perfomance: " << n_instructions / (seconds * 1e6)
            << std::endl;
  dump_registers();
  mmu_.dump_tlb();
  mmu_.dump_stats();
  return;
}

#if ENABLE_CACHE
bool Hart::step() {
  if (cache_.execute_from_cache(pc)) {
    return pc < memory_size;
  }

  uint32_t command = mem_->read_physical_word(pc);
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
#else
bool Hart::step() {
  uint32_t command = mem_->read_physical_word(pc);
  DecodedInstruction decoded = decode(command);
  decoded.first(this);
  pc += 4;
  ++n_instructions;
  return pc < memory_size;
}
#endif

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
bool Hart::is_mmu_enabled_() const { return mmu_enabled_; }

bool Hart::translate_mmu(uint32_t vaddr, uint32_t &paddr,
                         uint32_t access_type) {
  if (!mmu_enabled_) {
    paddr = vaddr;
    return true;
  }

  bool page_fault = false;
  bool success = mmu_.translate(vaddr, paddr, access_type, page_fault);

  if (page_fault) {
    handle_page_fault(vaddr, access_type);
    return false;
  }

  return success;
}

void Hart::handle_page_fault(uint32_t vaddr, uint32_t cause) {
  csr_[0x342] = cause;
  csr_[0x341] = pc;
  csr_[0x343] = vaddr;

  csr_[0x300] |= (1 << 7);
  csr_[0x300] &= ~(1 << 3);

  pc = csr_[0x305];

  std::cout << "[MMU] Page fault at vaddr=0x" << std::hex << vaddr
            << ", cause=" << cause << ", saved pc=0x" << csr_[0x141] << std::dec
            << "\n";
}

bool create_page_table(Memory &mem, uint32_t table_phys_addr) {

  std::cout << "Creating page table at phys addr: 0x" << std::hex
            << table_phys_addr << "\n";

  if (table_phys_addr % 4096 != 0) {
    std::cerr << "Error: Page table must be 4KB aligned!\n";
    return false;
  }

  uint32_t *root_table = new uint32_t[1024];

  std::cout << "Mode: 4KB pages (2-level)\n";

  uint32_t level2_addr = table_phys_addr + 2 * 4096;

  for (int vpn1 = 0; vpn1 < 1024; vpn1++) {
    uint32_t pte1_value = (level2_addr >> 12 << 10) | (1 << 0);
    root_table[vpn1] = pte1_value;
    uint32_t *level2_table = new uint32_t[1024];

    for (int vpn0 = 0; vpn0 < 1024; vpn0++) {
      uint32_t virt_page = (vpn1 << 10) | vpn0;
      uint32_t phys_page = (virt_page + 0x10) & memory_size;

      uint32_t pte0_value = (phys_page << 10) | // PPN в битах [31:10]
                            (1 << 0) |          // V - valid
                            (1 << 1) |          // R - readable
                            (1 << 2) |          // W - writable
                            (1 << 3) |          // X - executable
                            (1 << 4) |          // U - user accessible
                            (1 << 6);           // A - accessed

      level2_table[vpn0] = pte0_value;
    }

    for (int i = 0; i < 1024; i++) {
      uint32_t addr = level2_addr + (i * 4);
      uint32_t value = level2_table[i];
      mem.write_physical_byte(addr, value & 0xFF);
      mem.write_physical_byte(addr + 1, (value >> 8) & 0xFF);
      mem.write_physical_byte(addr + 2, (value >> 16) & 0xFF);
      mem.write_physical_byte(addr + 3, (value >> 24) & 0xFF);
    }

    delete[] level2_table;
    level2_addr += 4096;
  }

  for (int i = 0; i < 1024; i++) {
    uint32_t addr = table_phys_addr + (i * 4);
    uint32_t value = root_table[i];

    mem.write_physical_byte(addr, value & 0xFF);
    mem.write_physical_byte(addr + 1, (value >> 8) & 0xFF);
    mem.write_physical_byte(addr + 2, (value >> 16) & 0xFF);
    mem.write_physical_byte(addr + 3, (value >> 24) & 0xFF);
  }

  delete[] root_table;

  std::cout << "Page table created successfully\n";
  std::cout << "  Root table at: 0x" << std::hex << table_phys_addr << "\n";
  std::cout << "  Entries: 1024\n";

  return true;
}
} // namespace sim