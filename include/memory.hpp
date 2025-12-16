#pragma once

#include <elfio/elfio.hpp>
#include <limits>

namespace sim {
class Hart;

const long int memory_size = 400000000;

using register_t = uint32_t;

class Memory final {
private:
  Hart *hart_;
  uint8_t *mem_;
  int position_ = 0;
  bool mmu_enable_;

public:
  Memory();
  ~Memory();

  std::uint64_t virtual_addr_{std::numeric_limits<int64_t>::max()};

  uint8_t &operator[](std::size_t index);

  const uint8_t &operator[](std::size_t index) const;

  void store_data(const char *data, const std::uint64_t &size,
                  ELFIO::Elf64_Addr addr);

  void dump() const;

  uint32_t get_command(const std::uint64_t &virtual_addr) const;

  void set_virtual_address(ELFIO::Elf64_Addr virtual_addr);

  uint8_t read_byte(register_t addr);

  uint16_t read_halfword(register_t addr);

  uint32_t read_word(register_t addr);

  uint64_t read_doubleword(register_t addr);

  bool write_byte(uint8_t value, const std::uint64_t &addr);

  bool write_halfword(uint16_t value, const std::uint64_t &addr);

  bool write_word(uint32_t value, const std::uint64_t &addr);

  bool write_doubleword(uint64_t value, const std::uint64_t &addr);

  void write_physical_byte(uint32_t paddr, uint8_t value);
  void write_physical_word(uint32_t paddr, uint32_t value);
  uint8_t read_physical_byte(uint32_t paddr) const;
  uint32_t read_physical_word(uint32_t paddr) const;

  void set_hart(Hart *hart);
};
} // namespace sim