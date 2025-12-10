#include "loader.hpp"

namespace sim {
void Loader::read_elf(const std::filesystem::path &path) {
  std::vector<uint8_t> result;
  using namespace ELFIO;

  elfio reader;
  if (!reader.load(path.native())) {
    throw std::runtime_error("Cannot open file: " + path.string());
  }

  std::cout << "ELF-file properties" << std::endl;
  std::cout << "Path               : " << path << std::endl;

  std::cout << "ELF-file class     : ";
  if (reader.get_class() == ELFCLASS32)
    std::cout << "ELF32" << std::endl;
  else
    std::cout << "ELF64" << std::endl;

  std::cout << "ELF-file encoding  : ";
  if (reader.get_encoding() == ELFDATA2LSB)
    std::cout << "Little endian" << std::endl;
  else
    std::cout << "Big endian" << std::endl;

  std::cout << "Type               : " << reader.get_type() << std::endl;
  std::cout << "Machine            : RISC-V (0x" << std::hex
            << reader.get_machine() << ")" << std::endl;
  std::cout << "Entry point        : 0x" << std::hex << reader.get_entry()
            << std::endl;
  std::cout << "Sections           : " << std::dec << reader.sections.size()
            << std::endl;
  std::cout << "Segments           : " << reader.segments.size() << std::endl;

  for (auto &seg : reader.segments) {
    std::cout << "Segment with virtual address 0x" << std::hex
              << seg->get_virtual_address() << std::endl;
    if (seg->get_type() == ELFIO::PT_LOAD) {
      machine_.memory_.set_virtual_address(seg->get_virtual_address());
      std::cout << "Load segment with virtual address 0x" << std::hex
                << seg->get_virtual_address() << std::endl;
      if (seg->get_data() != nullptr)
        machine_.add_data(seg->get_data(), seg->get_file_size(),
                          seg->get_virtual_address());
    }
  }
  std::cout << "pc:" << std::dec
            << reader.get_entry() - machine_.memory_.virtual_addr_ << std::endl;
  machine_.set_pc(reader.get_entry() - machine_.memory_.virtual_addr_);

  return;
}

void Loader::run() { machine_.run(); }
} // namespace sim