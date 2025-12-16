#include "mmu.hpp"
#include "hart.hpp"
#include "memory.hpp"
#include <iostream>

namespace sim {

MMU::MMU() {
  tlb_clear();
  init_tlb();
}

void MMU::init_tlb() {
  for (auto &entry : tlb_) {
    entry.valid = false;
    entry.virtual_page = 0;
    entry.physical_page = 0;
    entry.flags = 0;
    entry.asid = 0;
  }
  tlb_pointer_ = 0;
}

void MMU::set_hart(Hart *hart) { hart_ = hart; }

void MMU::dump_tlb() const {
  std::cout << "\n=== TLB Contents (" << TLB_SIZE << " entries) ===\n";
  int valid_count = 0;

  for (size_t i = 0; i < TLB_SIZE; i++) {
    if (tlb_[i].valid) {
      valid_count++;
      std::cout << "  [" << i << "] VPN=0x" << std::hex << tlb_[i].virtual_page
                << " -> PPN=0x" << tlb_[i].physical_page
                << ", ASID=" << std::dec << tlb_[i].asid << ", flags=0x"
                << std::hex << tlb_[i].flags << " "
                << (tlb_[i].flags & PTE_R ? "R" : "-")
                << (tlb_[i].flags & PTE_W ? "W" : "-")
                << (tlb_[i].flags & PTE_X ? "X" : "-")
                << (tlb_[i].flags & PTE_G ? "G" : "-") << std::dec << "\n";
    }
  }

  std::cout << "Valid entries: " << valid_count << "/" << TLB_SIZE << "\n";
}

bool MMU::translate(uint32_t vaddr, uint32_t &paddr, uint32_t access_type,
                    bool &page_fault) {
  page_fault = false;

  if (mode_ == 0) {
    paddr = vaddr;
    return true;
  }

  if (!hart_ || !hart_->mem_) {
    page_fault = true;
    return false;
  }

  uint32_t offset = vaddr & 0xFFF;
  uint32_t vpn0 = (vaddr >> 12) & 0x3FF;
  uint32_t vpn1 = (vaddr >> 22) & 0x3FF;
  uint32_t full_vpn = (vpn1 << 10) | vpn0;

#ifdef DEBUG_MMU
  std::cout << "MMU translate:" << " pc:" << std::hex << hart_->pc
            << " va: " << vaddr << "\n";
#endif

  uint32_t current_asid = (satp_ >> 22) & 0x1FF;
  for (size_t i = 0; i < TLB_SIZE; i++) {
    if (tlb_[i].valid && tlb_[i].virtual_page == full_vpn &&
        ((tlb_[i].flags & PTE_G) || tlb_[i].asid == current_asid)) {

      tlb_hits_++;
      paddr = (tlb_[i].physical_page << 12) | offset;

#ifdef DEBUG_MMU
      std::cout << "TLB HIT: 0x" << std::hex << vaddr << " -> 0x" << paddr
                << std::dec << "\n";
#endif

      return true;
    }
  }

  tlb_misses_++;

#ifdef DEBUG_MMU
  std::cout << "TLB MISS, walking page tables...\n";
#endif

  uint32_t root_ppn = satp_ & 0x3FFFFF;
  uint32_t pte1_addr = (root_ppn << 12) + (vpn1 * 4);

  PageTableEntry pte1;
  if (!read_pte(pte1_addr, pte1)) {
    page_fault = true;
    page_faults_++;
    return false;
  }

#ifdef DEBUG_MMU
  std::cout << "PTE1 read from 0x" << std::hex << pte1_addr << ": ppn=0x"
            << pte1.ppn << ", flags=0x" << pte1.flags
            << ", valid=" << pte1.valid << std::dec << "\n";
#endif

  if (!pte1.valid) {
#ifdef DEBUG_MMU
    std::cout << "Page fault: PTE1 invalid\n";
#endif
    page_fault = true;
    page_faults_++;
    return false;
  }

  bool is_leaf = (pte1.flags & (PTE_R | PTE_W | PTE_X)) != 0;

  if (is_leaf) {
    if (!check_permissions(pte1.flags, access_type)) {
      page_fault = true;
      page_faults_++;
      return false;
    }

    pte1.flags |= PTE_A;
    if (access_type == 0x2) {
      pte1.flags |= PTE_D;
    }
    write_pte(pte1_addr, pte1);
    tlb_add(vaddr, pte1.ppn, pte1.flags, current_asid);

    paddr = ((pte1.ppn << 12) & 0xFFC00000) | (vaddr & 0x3FFFFF);

#ifdef DEBUG_MMU
    std::cout << "Translation success (4MB): 0x" << std::hex << vaddr
              << " -> 0x" << paddr << std::dec << "\n";
#endif

    return true;
  } else {
    uint32_t level2_table_addr = pte1.ppn << 12;
    uint32_t pte0_addr = level2_table_addr + (vpn0 * 4);

    PageTableEntry pte0;
    if (!read_pte(pte0_addr, pte0) || !pte0.valid) {
#ifdef DEBUG_MMU
      std::cout << "Page fault: PTE0 invalid\n";
#endif
      page_fault = true;
      page_faults_++;
      return false;
    }
#ifdef DEBUG_MMU
    std::cout << "PTE0 read from 0x" << std::hex << pte0_addr << ": ppn=0x"
              << pte0.ppn << ", flags=0x" << pte0.flags
              << ", valid=" << pte0.valid << std::dec << "\n";
#endif

    if (!check_permissions(pte0.flags, access_type)) {
      page_fault = true;
      page_faults_++;
      return false;
    }

    pte0.flags |= PTE_A;
    if (access_type == 0x2) {
      pte0.flags |= PTE_D;
    }
    write_pte(pte0_addr, pte0);

    tlb_add(vaddr, pte0.ppn, pte0.flags, current_asid);
    paddr = (pte0.ppn << 12) | offset;

#ifdef DEBUG_MMU
    std::cout << "Translation success (4KB): 0x" << std::hex << vaddr
              << " -> 0x" << paddr << std::dec << "\n";
#endif

    return true;
  }

  page_fault = true;
  return false;
}

void MMU::tlb_add(uint32_t vaddr, uint32_t ppn, uint32_t flags, uint32_t asid) {
  uint32_t vpn0 = (vaddr >> 12) & 0x3FF;
  uint32_t vpn1 = (vaddr >> 22) & 0x3FF;
  uint32_t full_vpn = (vpn1 << 10) | vpn0;

  for (size_t i = 0; i < TLB_SIZE; i++) {
    if (tlb_[i].valid && tlb_[i].virtual_page == full_vpn &&
        tlb_[i].asid == asid) {
      tlb_[i].physical_page = ppn;
      tlb_[i].flags = flags;
      return;
    }
  }

  tlb_[tlb_pointer_].valid = true;
  tlb_[tlb_pointer_].virtual_page = full_vpn;
  tlb_[tlb_pointer_].physical_page = ppn;
  tlb_[tlb_pointer_].flags = flags;
  tlb_[tlb_pointer_].asid = asid;

  tlb_pointer_ = (tlb_pointer_ + 1) % TLB_SIZE;
}

void MMU::tlb_clear() {
  for (auto &entry : tlb_) {
    entry.valid = false;
  }
  tlb_pointer_ = 0;
}

bool MMU::read_pte(uint32_t pte_addr, PageTableEntry &pte) {
  uint32_t pte_value;

  pte_value = hart_->mem_->read_physical_word(pte_addr);
  pte.valid = (pte_value & 1) != 0;
  pte.ppn = (pte_value >> 10) & 0x3FFFFF;
  pte.flags = pte_value & 0x3FF;

  return true;
}

bool MMU::write_pte(uint32_t pte_addr, const PageTableEntry &pte) {
  uint32_t pte_value = 0;

  if (pte.valid) {
    pte_value |= 1;
  }

  pte_value |= (pte.ppn & 0x3FFFFF) << 10;

  pte_value |= pte.flags & 0x3FF;

  (*hart_->mem_)[pte_addr] = static_cast<uint8_t>(pte_value & 0xFF);
  (*hart_->mem_)[pte_addr + 1] = static_cast<uint8_t>((pte_value >> 8) & 0xFF);
  (*hart_->mem_)[pte_addr + 2] = static_cast<uint8_t>((pte_value >> 16) & 0xFF);
  (*hart_->mem_)[pte_addr + 3] = static_cast<uint8_t>((pte_value >> 24) & 0xFF);

  return true;
}

void MMU::set_satp(uint32_t value) {
  satp_ = value;

  uint32_t satp_mode = (value >> 31);
  if (satp_mode == 0) {
    mode_ = 0; // Bare mode
  } else if (satp_mode == 1) {
    mode_ = 1; // Sv32
  }
}

uint32_t MMU::get_satp() const { return satp_; }

void MMU::tlb_remove(uint32_t vaddr, uint32_t asid) {
  uint32_t vpn = vaddr >> 12;

  for (size_t i = 0; i < TLB_SIZE; i++) {
    if (tlb_[i].valid && tlb_[i].virtual_page == vpn && tlb_[i].asid == asid) {
      tlb_[i].valid = false;
    }
  }
}

void MMU::dump_stats() const {
  std::cout << "MMU Statistics:\n";
  std::cout << "  TLB hits: " << tlb_hits_ << "\n";
  std::cout << "  TLB misses: " << tlb_misses_ << "\n";
  std::cout << "  Page faults: " << page_faults_ << "\n";

  if (tlb_hits_ + tlb_misses_ > 0) {
    double hit_rate = (double)tlb_hits_ / (tlb_hits_ + tlb_misses_) * 100;
    std::cout << "  TLB hit rate: " << hit_rate << "%\n";
  }
}

bool MMU::check_permissions(uint32_t pte_flags, uint32_t access_type) {
  switch (access_type) {
  case 0: // read
    return (pte_flags & PTE_R) != 0;
  case 1: // execute
    return (pte_flags & PTE_X) != 0;
  case 2: // write
    return (pte_flags & PTE_W) != 0 && (pte_flags & PTE_R) != 0;
  default:
    return false;
  }
}

}; // namespace sim