#pragma once

#include <elfio/elfio.hpp>
#include <limits>

namespace sim {
const int memory_size = 1000000;

using register_t = uint32_t;

class Memory final {
private:
  std::array<uint8_t, memory_size> mem_{};
  int position_ = 0;

public:
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

  void write_byte(uint8_t value, const std::uint64_t &addr);

  void write_halfword(uint16_t value, const std::uint64_t &addr);

  void write_word(uint32_t value, const std::uint64_t &addr);

  void write_doubleword(uint64_t value, const std::uint64_t &addr);
};
} // namespace sim