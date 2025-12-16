#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <vector>

namespace sim {

class Hart;

constexpr uint32_t PTE_V = 1 << 0; // Valid
constexpr uint32_t PTE_R = 1 << 1; // Readable
constexpr uint32_t PTE_W = 1 << 2; // Writable
constexpr uint32_t PTE_X = 1 << 3; // Executable
constexpr uint32_t PTE_U = 1 << 4; // User accessible
constexpr uint32_t PTE_G = 1 << 5; // Global
constexpr uint32_t PTE_A = 1 << 6; // Accessed
constexpr uint32_t PTE_D = 1 << 7; // Dirty

struct TLBEntry {
  bool valid = false;
  uint32_t virtual_page;
  uint32_t physical_page;
  uint32_t flags;
  uint32_t asid;
};

struct PageTableEntry {
  bool valid = false;
  uint32_t ppn;
  uint32_t flags; // R, W, X, U, G, A, D
};

class MMU {
private:
  static constexpr size_t TLB_SIZE = 64;
  std::array<TLBEntry, TLB_SIZE> tlb_;
  size_t tlb_pointer_ = 0;

  uint32_t satp_ = 0;

  uint32_t mode_ = 0;

  uint64_t tlb_hits_ = 0;
  uint64_t tlb_misses_ = 0;
  uint64_t page_faults_ = 0;
  Hart *hart_;

  bool check_permissions(uint32_t pte_flags, uint32_t access_type);

public:
  MMU();

  void init_tlb();

  void set_hart(Hart *hart);

  void dump_tlb() const;

  bool translate(uint32_t vaddr, uint32_t &paddr, uint32_t access_type,
                 bool &page_fault);

  void tlb_add(uint32_t vaddr, uint32_t paddr, uint32_t flags, uint32_t asid);
  void tlb_clear();
  void tlb_remove(uint32_t vaddr, uint32_t asid);

  void set_satp(uint32_t value);
  uint32_t get_satp() const;

  bool read_pte(uint32_t pte_addr, PageTableEntry &pte);
  bool write_pte(uint32_t pte_addr, const PageTableEntry &pte);

  void dump_stats() const;
};

} // namespace sim