#include "memory.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>

namespace sim {
uint8_t &Memory::operator[](std::size_t index) {
  if (index >= mem_.size()) {
    throw std::out_of_range("Memory index out of range: " +
                            std::to_string(index));
  }
  return mem_[index];
}

const uint8_t &Memory::operator[](std::size_t index) const {
  if (index >= mem_.size()) {
    throw std::out_of_range("Memory index out of range: " +
                            std::to_string(index));
  }
  return mem_[index];
}

void Memory::store_data(const char *data, const std::uint64_t &size,
                        ELFIO::Elf64_Addr addr) {
  if (data == nullptr) {
    throw std::invalid_argument("Data pointer is null");
  }

  std::copy(data, data + size, mem_.begin() + addr - virtual_addr_);
  position_ += size;
}

void Memory::dump() const {
  std::cout << "Memory dump!" << std::endl;
  size_t counter = 0;
  for (const auto &byte : mem_) {
    if (counter % 16 == 0) {
      std::cout << std::endl;
    }
    std::cout << std::hex << static_cast<int>(byte) << " ";
    ++counter;
  }
  std::cout << std::endl;
}

uint32_t Memory::get_command(const std::uint64_t &addr) const {
  return (static_cast<uint32_t>(mem_[addr]) << 24) |
         (static_cast<uint32_t>(mem_[addr + 1]) << 16) |
         (static_cast<uint32_t>(mem_[addr + 2]) << 8) |
         (static_cast<uint32_t>(mem_[addr + 3]));
}

void Memory::set_virtual_address(ELFIO::Elf64_Addr virtual_addr) {
  if (virtual_addr_ > virtual_addr) {
    virtual_addr_ = virtual_addr;
  }
}

uint8_t Memory::read_byte(register_t addr) {
  std::uint64_t phys_addr = addr;
  if (phys_addr >= memory_size) {
    throw std::out_of_range("Memory read_byte: address out of range");
  }
  return mem_[phys_addr];
}

uint16_t Memory::read_halfword(register_t addr) {
  std::uint64_t phys_addr = addr;
  if (phys_addr + 1 >= memory_size) {
    throw std::out_of_range("Memory read_halfword: address out of range");
  }

  uint16_t value = 0;
  value = static_cast<uint16_t>(mem_[phys_addr]) |
          (static_cast<uint16_t>(mem_[phys_addr + 1]) << 8);
  return value;
}

uint32_t Memory::read_word(register_t addr) {
  std::uint64_t phys_addr = addr;
  if (phys_addr + 3 >= memory_size) {
    throw std::out_of_range("Memory read_word: address out of range");
  }

  uint32_t value = 0;
  value = static_cast<uint32_t>(mem_[phys_addr]) |
          (static_cast<uint32_t>(mem_[phys_addr + 1]) << 8) |
          (static_cast<uint32_t>(mem_[phys_addr + 2]) << 16) |
          (static_cast<uint32_t>(mem_[phys_addr + 3]) << 24);
  return value;
}

uint64_t Memory::read_doubleword(register_t addr) {
  std::uint64_t phys_addr = addr;
  if (phys_addr + 7 >= memory_size) {
    throw std::out_of_range("Memory read_word: address out of range");
  }

  uint64_t value = 0;
  value = static_cast<uint64_t>(mem_[phys_addr]) |
          (static_cast<uint64_t>(mem_[phys_addr + 1]) << 8) |
          (static_cast<uint64_t>(mem_[phys_addr + 2]) << 16) |
          (static_cast<uint64_t>(mem_[phys_addr + 3]) << 24) |
          (static_cast<uint64_t>(mem_[phys_addr + 4]) << 32) |
          (static_cast<uint64_t>(mem_[phys_addr + 5]) << 40) |
          (static_cast<uint64_t>(mem_[phys_addr + 6]) << 48) |
          (static_cast<uint64_t>(mem_[phys_addr + 7]) << 56);
  return value;
}

void Memory::write_byte(uint8_t value, const std::uint64_t &addr) {
  std::uint64_t phys_addr = addr;
  if (phys_addr >= memory_size) {
    std::cerr << "Memory write out of bounds: addr=" << addr
              << " (phys=" << phys_addr << ")" << std::endl;
    std::exit(1);
  }
  mem_[phys_addr] = value;
}

void Memory::write_halfword(uint16_t value, const std::uint64_t &addr) {
  std::uint64_t phys_addr = addr;
  if (phys_addr + 1 >= memory_size) {
    std::cerr << "Memory write out of bounds: addr=" << addr
              << " (phys=" << phys_addr << ")" << std::endl;
    std::exit(1);
  }

  mem_[phys_addr] = static_cast<uint8_t>(value & 0xFF);
  mem_[phys_addr + 1] = static_cast<uint8_t>((value >> 8) & 0xFF);
}

void Memory::write_word(uint32_t value, const std::uint64_t &addr) {
  std::uint64_t phys_addr = addr;
  if (phys_addr + 3 >= memory_size) {
    std::cerr << "Memory write out of bounds: addr=" << addr
              << " (phys=" << phys_addr << ")" << std::endl;
    std::exit(1);
  }

  mem_[phys_addr] = static_cast<uint8_t>(value & 0xFF);
  mem_[phys_addr + 1] = static_cast<uint8_t>((value >> 8) & 0xFF);
  mem_[phys_addr + 2] = static_cast<uint8_t>((value >> 16) & 0xFF);
  mem_[phys_addr + 3] = static_cast<uint8_t>((value >> 24) & 0xFF);
}

void Memory::write_doubleword(uint64_t value, const std::uint64_t &addr) {
  std::uint64_t phys_addr = addr;
  if (phys_addr + 7 >= memory_size) {
    std::cerr << "Memory write out of bounds: addr=" << addr
              << " (phys=" << phys_addr << ")" << std::endl;
    std::exit(1);
  }

  write_word(static_cast<uint32_t>(value & 0xFFFFFFFFULL), addr);
  write_word(static_cast<uint32_t>((value >> 32) & 0xFFFFFFFFULL), addr + 4);
}
} // namespace sim