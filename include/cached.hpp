#pragma once

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <utility>

namespace sim {
class Hart;

using register_t = uint32_t;
using InstructionHandler = std::function<void(Hart *)>;
using DecodedInstruction = std::pair<InstructionHandler, bool>;

DecodedInstruction decode(uint32_t instr);

class Cached final {
private:
  std::unordered_map<register_t, std::vector<InstructionHandler>> cached_;

public:
  Hart *hart_;

  bool cache_it(register_t pc);

  bool execute_from_cache(register_t &pc);
};
}; // namespace sim