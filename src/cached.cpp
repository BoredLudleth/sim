#include "cached.hpp"
#include "generated_instructions.hpp"
#include "hart.hpp"
#include "memory.hpp"

namespace sim {
bool Cached::cache_it(register_t pc) {
  std::vector<InstructionHandler> block;
  register_t cur_pc = pc;

  while (true) {
    uint32_t instr = hart_->mem_->read_word(cur_pc);
    DecodedInstruction decoded = decode(instr);

    block.push_back(decoded.first);

    if (decoded.second) {
      cached_[pc] = block;
      return true;
    }

    cur_pc += 4;

    if (block.size() >= 100) {
      cached_[pc] = block;
      return true;
    }
  }
}

bool Cached::execute_from_cache(register_t &pc) {
  auto it = cached_.find(pc);
  if (it != cached_.end()) {

    for (auto &handler : it->second) {
      ++hart_->n_instructions;
      handler(hart_);
      pc += 4;
    }
    return true;
  }
  return false;
}

DecodedInstruction decode(uint32_t instr) {
  uint8_t opcode = instr & 0x7F;
  uint8_t funct3 = (instr >> 12) & 0x7;
  uint8_t funct7 = (instr >> 25) & 0x7F;

  bool is_control_flow = false;
  InstructionHandler handler;

  switch (opcode) {
  case 0x03: {
    switch (funct3) {
    case 0x0:
      handler = [instr](Hart *hart) { exec_lb(hart, instr); };
      break;
    case 0x1:
      handler = [instr](Hart *hart) { exec_lh(hart, instr); };
      break;
    case 0x2:
      handler = [instr](Hart *hart) { exec_lw(hart, instr); };
      break;
    case 0x3:
      handler = [instr](Hart *hart) { exec_ld(hart, instr); };
      break;
    case 0x4:
      handler = [instr](Hart *hart) { exec_lbu(hart, instr); };
      break;
    case 0x5:
      handler = [instr](Hart *hart) { exec_lhu(hart, instr); };
      break;
    case 0x6:
      handler = [instr](Hart *hart) { exec_lwu(hart, instr); };
      break;
    default:
      throw std::runtime_error("Illegal instruction (LOAD)");
    }
    break;
  }
  case 0x0F: {
    switch (funct3) {
    case 0x0:
      handler = [instr](Hart *hart) { exec_fence(hart, instr); };
      break;
    case 0x1:
      handler = [instr](Hart *hart) { exec_fence_i(hart, instr); };
      break;
    default:
      throw std::runtime_error("Illegal instruction (FENCE)");
    }
    break;
  }
  case 0x13: {
    switch (funct3) {
    case 0x0:
      handler = [instr](Hart *hart) { exec_addi(hart, instr); };
      break;
    case 0x1: {
      if (funct7 == 0x0) {
        handler = [instr](Hart *hart) { exec_slli(hart, instr); };
      } else {
        throw std::runtime_error("Illegal instruction (wrong funct7)");
      }
      break;
    }
    case 0x2:
      handler = [instr](Hart *hart) { exec_slti(hart, instr); };
      break;
    case 0x3:
      handler = [instr](Hart *hart) { exec_sltiu(hart, instr); };
      break;
    case 0x4:
      handler = [instr](Hart *hart) { exec_xori(hart, instr); };
      break;
    case 0x5: {
      switch (funct7) {
      case 0x0:
        handler = [instr](Hart *hart) { exec_srli(hart, instr); };
        break;
      case 0x20:
        handler = [instr](Hart *hart) { exec_srai(hart, instr); };
        break;
      default:
        throw std::runtime_error("Illegal instruction (no funct7 match)");
      }
      break;
    }
    case 0x6:
      handler = [instr](Hart *hart) { exec_ori(hart, instr); };
      break;
    case 0x7:
      handler = [instr](Hart *hart) { exec_andi(hart, instr); };
      break;
    default:
      throw std::runtime_error("Illegal instruction (OP-IMM)");
    }
    break;
  }
  case 0x17: {
    handler = [instr](Hart *hart) { exec_auipc(hart, instr); };
    break;
  }
  case 0x1B: {
    switch (funct3) {
    case 0x0:
      handler = [instr](Hart *hart) { exec_addiw(hart, instr); };
      break;
    case 0x1: {
      if (funct7 == 0x0) {
        handler = [instr](Hart *hart) { exec_slliw(hart, instr); };
      } else {
        throw std::runtime_error("Illegal instruction (wrong funct7)");
      }
      break;
    }
    case 0x5: {
      switch (funct7) {
      case 0x0:
        handler = [instr](Hart *hart) { exec_srliw(hart, instr); };
        break;
      case 0x20:
        handler = [instr](Hart *hart) { exec_sraiw(hart, instr); };
        break;
      default:
        throw std::runtime_error("Illegal instruction (no funct7 match)");
      }
      break;
    }
    default:
      throw std::runtime_error("Illegal instruction (OP-IMM-32)");
    }
    break;
  }
  case 0x23: {
    switch (funct3) {
    case 0x0:
      handler = [instr](Hart *hart) { exec_sb(hart, instr); };
      break;
    case 0x1:
      handler = [instr](Hart *hart) { exec_sh(hart, instr); };
      break;
    case 0x2:
      handler = [instr](Hart *hart) { exec_sw(hart, instr); };
      break;
    case 0x3:
      handler = [instr](Hart *hart) { exec_sd(hart, instr); };
      break;
    default:
      throw std::runtime_error("Illegal instruction (STORE)");
    }
    break;
  }
  case 0x33: {
    switch (funct3) {
    case 0x0: {
      switch (funct7) {
      case 0x0:
        handler = [instr](Hart *hart) { exec_add(hart, instr); };
        break;
      case 0x20:
        handler = [instr](Hart *hart) { exec_sub(hart, instr); };
        break;
      default:
        throw std::runtime_error("Illegal instruction (no funct7 match)");
      }
      break;
    }
    case 0x1: {
      if (funct7 == 0x0) {
        handler = [instr](Hart *hart) { exec_sll(hart, instr); };
      } else {
        throw std::runtime_error("Illegal instruction (wrong funct7)");
      }
      break;
    }
    case 0x2: {
      if (funct7 == 0x0) {
        handler = [instr](Hart *hart) { exec_slt(hart, instr); };
      } else {
        throw std::runtime_error("Illegal instruction (wrong funct7)");
      }
      break;
    }
    case 0x3: {
      if (funct7 == 0x0) {
        handler = [instr](Hart *hart) { exec_sltu(hart, instr); };
      } else {
        throw std::runtime_error("Illegal instruction (wrong funct7)");
      }
      break;
    }
    case 0x4: {
      if (funct7 == 0x0) {
        handler = [instr](Hart *hart) { exec_xor(hart, instr); };
      } else {
        throw std::runtime_error("Illegal instruction (wrong funct7)");
      }
      break;
    }
    case 0x5: {
      switch (funct7) {
      case 0x0:
        handler = [instr](Hart *hart) { exec_srl(hart, instr); };
        break;
      case 0x20:
        handler = [instr](Hart *hart) { exec_sra(hart, instr); };
        break;
      default:
        throw std::runtime_error("Illegal instruction (no funct7 match)");
      }
      break;
    }
    case 0x6: {
      if (funct7 == 0x0) {
        handler = [instr](Hart *hart) { exec_or(hart, instr); };
      } else {
        throw std::runtime_error("Illegal instruction (wrong funct7)");
      }
      break;
    }
    case 0x7: {
      if (funct7 == 0x0) {
        handler = [instr](Hart *hart) { exec_and(hart, instr); };
      } else {
        throw std::runtime_error("Illegal instruction (wrong funct7)");
      }
      break;
    }
    default:
      throw std::runtime_error("Illegal instruction (OP)");
    }
    break;
  }
  case 0x37: {
    handler = [instr](Hart *hart) { exec_lui(hart, instr); };
    break;
  }
  case 0x3B: {
    switch (funct3) {
    case 0x0: {
      switch (funct7) {
      case 0x0:
        handler = [instr](Hart *hart) { exec_addw(hart, instr); };
        break;
      case 0x20:
        handler = [instr](Hart *hart) { exec_subw(hart, instr); };
        break;
      default:
        throw std::runtime_error("Illegal instruction (no funct7 match)");
      }
      break;
    }
    case 0x1: {
      if (funct7 == 0x0) {
        handler = [instr](Hart *hart) { exec_sllw(hart, instr); };
      } else {
        throw std::runtime_error("Illegal instruction (wrong funct7)");
      }
      break;
    }
    case 0x2: {
      if (funct7 == 0x0) {
        handler = [instr](Hart *hart) { exec_sltw(hart, instr); };
      } else {
        throw std::runtime_error("Illegal instruction (wrong funct7)");
      }
      break;
    }
    case 0x3: {
      if (funct7 == 0x0) {
        handler = [instr](Hart *hart) { exec_sltuw(hart, instr); };
      } else {
        throw std::runtime_error("Illegal instruction (wrong funct7)");
      }
      break;
    }
    case 0x4: {
      if (funct7 == 0x0) {
        handler = [instr](Hart *hart) { exec_xorw(hart, instr); };
      } else {
        throw std::runtime_error("Illegal instruction (wrong funct7)");
      }
      break;
    }
    case 0x5: {
      switch (funct7) {
      case 0x0:
        handler = [instr](Hart *hart) { exec_srlw(hart, instr); };
        break;
      case 0x20:
        handler = [instr](Hart *hart) { exec_sraw(hart, instr); };
        break;
      default:
        throw std::runtime_error("Illegal instruction (no funct7 match)");
      }
      break;
    }
    case 0x6: {
      if (funct7 == 0x0) {
        handler = [instr](Hart *hart) { exec_orw(hart, instr); };
      } else {
        throw std::runtime_error("Illegal instruction (wrong funct7)");
      }
      break;
    }
    case 0x7: {
      if (funct7 == 0x0) {
        handler = [instr](Hart *hart) { exec_andw(hart, instr); };
      } else {
        throw std::runtime_error("Illegal instruction (wrong funct7)");
      }
      break;
    }
    default:
      throw std::runtime_error("Illegal instruction (OP-32)");
    }
    break;
  }
  case 0x63: {
    is_control_flow = true;
    switch (funct3) {
    case 0x0:
      handler = [instr](Hart *hart) { exec_beq(hart, instr); };
      break;
    case 0x1:
      handler = [instr](Hart *hart) { exec_bne(hart, instr); };
      break;
    case 0x4:
      handler = [instr](Hart *hart) { exec_blt(hart, instr); };
      break;
    case 0x5:
      handler = [instr](Hart *hart) { exec_bge(hart, instr); };
      break;
    case 0x6:
      handler = [instr](Hart *hart) { exec_bltu(hart, instr); };
      break;
    case 0x7:
      handler = [instr](Hart *hart) { exec_bgeu(hart, instr); };
      break;
    default:
      throw std::runtime_error("Illegal instruction (BRANCH)");
    }
    break;
  }
  case 0x67: {
    is_control_flow = true;
    switch (funct3) {
    case 0x0:
      handler = [instr](Hart *hart) { exec_jalr(hart, instr); };
      break;
    default:
      throw std::runtime_error("Illegal instruction (JALR)");
    }
    break;
  }
  case 0x6F: {
    is_control_flow = true;
    handler = [instr](Hart *hart) { exec_jal(hart, instr); };
    break;
  }
  case 0x73: {
    switch (funct3) {
    case 0x0: {
      switch (funct7) {
      case 0x09:
        handler = [instr](Hart *hart) { exec_sfence_vma(hart, instr); };
        break;
      default:
        throw std::runtime_error("Illegal instruction (no funct7 match)");
      }
      break;
    }
    case 0x1:
      handler = [instr](Hart *hart) { exec_csrrw(hart, instr); };
      break;
    case 0x2:
      handler = [instr](Hart *hart) { exec_csrrs(hart, instr); };
      break;
    case 0x3:
      handler = [instr](Hart *hart) { exec_csrrc(hart, instr); };
      break;
    case 0x5:
      handler = [instr](Hart *hart) { exec_csrrwi(hart, instr); };
      break;
    case 0x6:
      handler = [instr](Hart *hart) { exec_csrrsi(hart, instr); };
      break;
    case 0x7:
      handler = [instr](Hart *hart) { exec_csrrci(hart, instr); };
      break;
    default:
      throw std::runtime_error("Illegal instruction (SYSTEM)");
    }

    uint32_t csr_addr = (instr >> 20) & 0xFFF;
    if (funct3 == 0x0 || csr_addr == 0x302 || csr_addr == 0x304 ||
        csr_addr == 0x305 || csr_addr == 0x341 || csr_addr == 0x342) {
      is_control_flow = true;
    }
    break;
  }
  default:
    handler = [instr](Hart *hart) { exec_ill(hart, instr); };
    break;
    is_control_flow = true;
  }

  return std::make_pair(handler, is_control_flow);
}
}; // namespace sim
