#include "generated_instructions.hpp"
#include "hart.hpp"
#include <cstdint>
#include <cstdlib>
#include <iostream>

namespace sim {

// ADD instruction
void exec_add(Hart *hart, uint32_t instr) {
  // std::cout << "ADD instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = rs1_val + rs2_val;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SUB instruction
void exec_sub(Hart *hart, uint32_t instr) {
  // std::cout << "SUB instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = rs1_val - rs2_val;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SLL instruction
void exec_sll(Hart *hart, uint32_t instr) {
  // std::cout << "SLL instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = rs1_val << (rs2_val & 0x1F);
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SLT instruction
void exec_slt(Hart *hart, uint32_t instr) {
  // std::cout << "SLT instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = ((int32_t)(rs1_val < (int32_t)rs2_val) ? 1 : 0) ? 1 : 0;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SLTU instruction
void exec_sltu(Hart *hart, uint32_t instr) {
  // std::cout << "SLTU instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = ((uint32_t)(rs1_val < (uint32_t)rs2_val) ? 1 : 0) ? 1 : 0;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// XOR instruction
void exec_xor(Hart *hart, uint32_t instr) {
  // std::cout << "XOR instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = rs1_val ^ rs2_val;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SRL instruction
void exec_srl(Hart *hart, uint32_t instr) {
  // std::cout << "SRL instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = static_cast<uint32_t>(rs1_val) >> (rs2_val & 0x1F);
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SRA instruction
void exec_sra(Hart *hart, uint32_t instr) {
  // std::cout << "SRA instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = static_cast<int32_t>(rs1_val) >> (rs2_val & 0x1F);
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// OR instruction
void exec_or(Hart *hart, uint32_t instr) {
  // std::cout << "OR instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = rs1_val | rs2_val;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// AND instruction
void exec_and(Hart *hart, uint32_t instr) {
  // std::cout << "AND instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = rs1_val & rs2_val;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// ADDI instruction
void exec_addi(Hart *hart, uint32_t instr) {
  // std::cout << "ADDI instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }
  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = rs1_val + imm;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SLTI instruction
void exec_slti(Hart *hart, uint32_t instr) {
  // std::cout << "SLTI instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = ((int32_t)(rs1_val < (int32_t)imm) ? 1 : 0) ? 1 : 0;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SLTIU instruction
void exec_sltiu(Hart *hart, uint32_t instr) {
  // std::cout << "SLTIU instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = ((uint32_t)(rs1_val < (uint32_t)imm) ? 1 : 0) ? 1 : 0;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// XORI instruction
void exec_xori(Hart *hart, uint32_t instr) {
  // std::cout << "XORI instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = rs1_val ^ imm;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// ORI instruction
void exec_ori(Hart *hart, uint32_t instr) {
  // std::cout << "ORI instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = rs1_val | imm;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// ANDI instruction
void exec_andi(Hart *hart, uint32_t instr) {
  // std::cout << "ANDI instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = rs1_val & imm;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SLLI instruction
void exec_slli(Hart *hart, uint32_t instr) {
  // std::cout << "SLLI instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t shamt = (instr >> 20) & 0x1F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = rs1_val << shamt;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SRLI instruction
void exec_srli(Hart *hart, uint32_t instr) {
  // std::cout << "SRLI instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t shamt = (instr >> 20) & 0x1F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = static_cast<uint32_t>(rs1_val) >> shamt;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SRAI instruction
void exec_srai(Hart *hart, uint32_t instr) {
  // std::cout << "SRAI instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t shamt = (instr >> 20) & 0x1F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = static_cast<int32_t>(rs1_val) >> shamt;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// LB instruction
void exec_lb(Hart *hart, uint32_t instr) {
  // std::cout << "LB instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = static_cast<int8_t>(hart->mem_->read_byte(rs1_val + imm));
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// LH instruction
void exec_lh(Hart *hart, uint32_t instr) {
  // std::cout << "LH instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result =
      static_cast<int16_t>(hart->mem_->read_halfword(rs1_val + imm));
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// LW instruction
void exec_lw(Hart *hart, uint32_t instr) {
  // std::cout << "LW instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }
  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = hart->mem_->read_word(rs1_val + imm);
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// LBU instruction
void exec_lbu(Hart *hart, uint32_t instr) {
  // std::cout << "LBU instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result =
      static_cast<uint8_t>(hart->mem_->read_byte(rs1_val + imm));
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// LHU instruction
void exec_lhu(Hart *hart, uint32_t instr) {
  // std::cout << "LHU instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result =
      static_cast<uint16_t>(hart->mem_->read_halfword(rs1_val + imm));
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SB instruction
void exec_sb(Hart *hart, uint32_t instr) {
  // std::cout << "SB instruction" << std::endl;
  // S-type instruction fields
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  int32_t imm = ((instr >> 25) << 5) | ((instr >> 7) & 0x1F);
  if (imm & 0x800)
    imm |= 0xFFFFF000;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  hart->mem_->write_byte(rs2_val & 0xFF, rs1_val + imm);
}

// SH instruction
void exec_sh(Hart *hart, uint32_t instr) {
  // std::cout << "SH instruction" << std::endl;
  // S-type instruction fields
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  int32_t imm = ((instr >> 25) << 5) | ((instr >> 7) & 0x1F);
  if (imm & 0x800)
    imm |= 0xFFFFF000;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  hart->mem_->write_halfword(rs2_val & 0xFFFF, rs1_val + imm);
}

// SW instruction
void exec_sw(Hart *hart, uint32_t instr) {
  // std::cout << "SW instruction" << std::endl;
  // S-type instruction fields
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  int32_t imm = ((instr >> 25) << 5) | ((instr >> 7) & 0x1F);
  if (imm & 0x800)
    imm |= 0xFFFFF000;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  hart->mem_->write_word(rs2_val, rs1_val + imm);
}

// BEQ instruction
void exec_beq(Hart *hart, uint32_t instr) {
  // std::cout << "BEQ instruction" << std::endl;
  // B-type instruction fields
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  int32_t imm = ((instr & 0x80000000) >> 19) | ((instr >> 20) & 0x7E0) |
                ((instr >> 7) & 0x1E) | ((instr & 0x80) << 4);
  if (imm & 0x1000)
    imm |= 0xFFFFE000;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  if (rs1_val == rs2_val) {
    hart->pc += imm - 4;
  }
}

// BNE instruction
void exec_bne(Hart *hart, uint32_t instr) {
  // std::cout << "BNE instruction" << std::endl;
  // B-type instruction fields
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  int32_t imm = ((instr & 0x80000000) >> 19) | ((instr >> 20) & 0x7E0) |
                ((instr >> 7) & 0x1E) | ((instr & 0x80) << 4);
  if (imm & 0x1000)
    imm |= 0xFFFFE000;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  if (rs1_val != rs2_val) {
    hart->pc += imm - 4;
  }
}

// BLT instruction
void exec_blt(Hart *hart, uint32_t instr) {
  // std::cout << "BLT instruction" << std::endl;
  // B-type instruction fields
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  int32_t imm = ((instr & 0x80000000) >> 19) | ((instr >> 20) & 0x7E0) |
                ((instr >> 7) & 0x1E) | ((instr & 0x80) << 4);
  if (imm & 0x1000)
    imm |= 0xFFFFE000;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  if (((int32_t)rs1_val < (int32_t)rs2_val)) {
    hart->pc += imm - 4;
  }
}

// BGE instruction
void exec_bge(Hart *hart, uint32_t instr) {
  // std::cout << "BGE instruction" << std::endl;
  // B-type instruction fields
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  int32_t imm = ((instr & 0x80000000) >> 19) | ((instr >> 20) & 0x7E0) |
                ((instr >> 7) & 0x1E) | ((instr & 0x80) << 4);
  if (imm & 0x1000)
    imm |= 0xFFFFE000;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  if (((int32_t)rs1_val >= (int32_t)rs2_val)) {
    hart->pc += imm - 4;
  }
}

// BLTU instruction
void exec_bltu(Hart *hart, uint32_t instr) {
  // std::cout << "BLTU instruction" << std::endl;
  // B-type instruction fields
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  int32_t imm = ((instr & 0x80000000) >> 19) | ((instr >> 20) & 0x7E0) |
                ((instr >> 7) & 0x1E) | ((instr & 0x80) << 4);
  if (imm & 0x1000)
    imm |= 0xFFFFE000;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  if (((uint32_t)rs1_val < (uint32_t)rs2_val)) {
    hart->pc += imm - 4;
  }
}

// BGEU instruction
void exec_bgeu(Hart *hart, uint32_t instr) {
  // std::cout << "BGEU instruction" << std::endl;
  // B-type instruction fields
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  int32_t imm = ((instr & 0x80000000) >> 19) | ((instr >> 20) & 0x7E0) |
                ((instr >> 7) & 0x1E) | ((instr & 0x80) << 4);
  if (imm & 0x1000)
    imm |= 0xFFFFE000;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  if (((uint32_t)rs1_val >= (uint32_t)rs2_val)) {
    hart->pc += imm - 4;
  }
}

// JAL instruction
void exec_jal(Hart *hart, uint32_t instr) {
  // std::cout << "JAL instruction" << std::endl;

  uint8_t rd = (instr >> 7) & 0x1F;

  uint32_t imm_20 = (instr >> 31) & 0x1;
  uint32_t imm_10_1 = (instr >> 21) & 0x3FF;
  uint32_t imm_11 = (instr >> 20) & 0x1;
  uint32_t imm_19_12 = (instr >> 12) & 0xFF;

  uint32_t imm_unsigned =
      (imm_20 << 20) | (imm_19_12 << 12) | (imm_11 << 11) | (imm_10_1 << 1);

  int32_t imm;
  if (imm_20) {
    imm = imm_unsigned | 0xFFE00000;
  } else {
    imm = imm_unsigned;
  }

  register_t result = hart->pc;
  hart->pc += imm - 4;

  if (rd != 0) {
    hart->gpr_[rd] = result;
  }
}

// JALR instruction
void exec_jalr(Hart *hart, uint32_t instr) {
  // std::cout << "JALR instruction" << std::endl;

  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;

  uint32_t imm_11_0 = (instr >> 20) & 0xFFF;

  uint32_t imm_unsigned = imm_11_0;

  int32_t imm;
  if (imm_11_0 & 0x800) {
    imm = imm_unsigned | 0xFFFFF000;
  } else {
    imm = imm_unsigned;
  }

  register_t rs1_val = hart->gpr_[rs1];
  register_t result = hart->pc;
  hart->pc = (rs1_val + imm) & ~1;

  if (rd != 0) {
    hart->gpr_[rd] = result;
  }
}

// LUI instruction
void exec_lui(Hart *hart, uint32_t instr) {
  // std::cout << "LUI instruction" << std::endl;
  // U-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  int32_t imm = instr & 0xFFFFF000; // imm[31:12] << 12
  register_t result = imm;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// AUIPC instruction
void exec_auipc(Hart *hart, uint32_t instr) {
  // std::cout << "AUIPC instruction" << std::endl;
  // U-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  int32_t imm = instr & 0xFFFFF000; // imm[31:12] << 12
  register_t result = hart->pc + imm;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// FENCE instruction
void exec_fence(Hart *hart, uint32_t instr) {
  // std::cout << "FENCE instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
}

// FENCE_I instruction
void exec_fence_i(Hart *hart, uint32_t instr) {
  // std::cout << "FENCE_I instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
}

// CSRRW instruction
void exec_csrrw(Hart *hart, uint32_t instr) {
  // std::cout << "CSRRW instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t tmp = hart->csr_[imm & 0xFFF];
  hart->csr_[imm & 0xFFF] = rs1_val;
  if (rd != 0)
    hart->gpr_[rd] = tmp;
}

// CSRRS instruction
void exec_csrrs(Hart *hart, uint32_t instr) {
  // std::cout << "CSRRS instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t tmp = hart->csr_[imm & 0xFFF];
  hart->csr_[imm & 0xFFF] = tmp | rs1_val;
  if (rd != 0)
    hart->gpr_[rd] = tmp;
}

// CSRRC instruction
void exec_csrrc(Hart *hart, uint32_t instr) {
  // std::cout << "CSRRC instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t tmp = hart->csr_[imm & 0xFFF];
  hart->csr_[imm & 0xFFF] = tmp & ~rs1_val;
  if (rd != 0)
    hart->gpr_[rd] = tmp;
}

// CSRRWI instruction
void exec_csrrwi(Hart *hart, uint32_t instr) {
  // std::cout << "CSRRWI instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t tmp = hart->csr_[imm & 0xFFF];
  hart->csr_[imm & 0xFFF] = rs1_val;
  if (rd != 0)
    hart->gpr_[rd] = tmp;
}

// CSRRSI instruction
void exec_csrrsi(Hart *hart, uint32_t instr) {
  // std::cout << "CSRRSI instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t tmp = hart->csr_[imm & 0xFFF];
  hart->csr_[imm & 0xFFF] = tmp | rs1_val;
  if (rd != 0)
    hart->gpr_[rd] = tmp;
}

// CSRRCI instruction
void exec_csrrci(Hart *hart, uint32_t instr) {
  // std::cout << "CSRRCI instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t tmp = hart->csr_[imm & 0xFFF];
  hart->csr_[imm & 0xFFF] = tmp & ~rs1_val;
  if (rd != 0)
    hart->gpr_[rd] = tmp;
}

// ECALL instruction
void exec_ecall(Hart *hart, uint32_t instr) {
  // std::cout << "ECALL instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  std::cerr << "ECALL instruction" << std::endl;
  std::exit(1);
}

// EBREAK instruction
void exec_ebreak(Hart *hart, uint32_t instr) {
  // std::cout << "EBREAK instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  std::cerr << "EBREAK instruction" << std::endl;
  std::exit(1);
}

// URET instruction
void exec_uret(Hart *hart, uint32_t instr) {
  // std::cout << "URET instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  std::cerr << "URET instruction (not implemented)" << std::endl;
  std::exit(1);
}

// SRET instruction
void exec_sret(Hart *hart, uint32_t instr) {
  // std::cout << "SRET instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
}

// MRET instruction
void exec_mret(Hart *hart, uint32_t instr) {
  // std::cout << "MRET instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
}

// WFI instruction
void exec_wfi(Hart *hart, uint32_t instr) {
  // std::cout << "WFI instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
}

// SFENCE_VMA instruction
void exec_sfence_vma(Hart *hart, uint32_t instr) {
  // std::cout << "SFENCE_VMA instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
}

// ADDIW instruction
void exec_addiw(Hart *hart, uint32_t instr) {
  // std::cout << "ADDIW instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = static_cast<register_t>(
      static_cast<int64_t>(static_cast<int32_t>(rs1_val & 0xFFFFFFFF) + imm));
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SLLIW instruction
void exec_slliw(Hart *hart, uint32_t instr) {
  // std::cout << "SLLIW instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t shamt = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = static_cast<register_t>(static_cast<int64_t>(
      static_cast<int32_t>(rs1_val & 0xFFFFFFFF) << shamt));
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SRLIW instruction
void exec_srliw(Hart *hart, uint32_t instr) {
  // std::cout << "SRLIW instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t shamt = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = static_cast<register_t>(
      static_cast<uint32_t>(rs1_val & 0xFFFFFFFF) >> shamt);
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SRAIW instruction
void exec_sraiw(Hart *hart, uint32_t instr) {
  // std::cout << "SRAIW instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t shamt = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = static_cast<register_t>(static_cast<int64_t>(
      static_cast<int32_t>(rs1_val & 0xFFFFFFFF) >> shamt));
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// ADDW instruction
void exec_addw(Hart *hart, uint32_t instr) {
  // std::cout << "ADDW instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = static_cast<register_t>(
      static_cast<int64_t>(static_cast<int32_t>(rs1_val & 0xFFFFFFFF) +
                           static_cast<int32_t>(rs2_val & 0xFFFFFFFF)));
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SUBW instruction
void exec_subw(Hart *hart, uint32_t instr) {
  // std::cout << "SUBW instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = static_cast<register_t>(
      static_cast<int64_t>(static_cast<int32_t>(rs1_val & 0xFFFFFFFF) -
                           static_cast<int32_t>(rs2_val & 0xFFFFFFFF)));
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SLLW instruction
void exec_sllw(Hart *hart, uint32_t instr) {
  // std::cout << "SLLW instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = static_cast<register_t>(static_cast<int64_t>(
      static_cast<int32_t>(rs1_val & 0xFFFFFFFF) << (rs2_val & 0x1F)));
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SRLW instruction
void exec_srlw(Hart *hart, uint32_t instr) {
  // std::cout << "SRLW instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = static_cast<register_t>(
      static_cast<uint32_t>(rs1_val & 0xFFFFFFFF) >> (rs2_val & 0x1F));
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SRAW instruction
void exec_sraw(Hart *hart, uint32_t instr) {
  // std::cout << "SRAW instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = static_cast<register_t>(static_cast<int64_t>(
      static_cast<int32_t>(rs1_val & 0xFFFFFFFF) >> (rs2_val & 0x1F)));
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SLTW instruction
void exec_sltw(Hart *hart, uint32_t instr) {
  // std::cout << "SLTW instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = (static_cast<int32_t>(
                          (rs1_val & 0xFFFFFFFF) < static_cast<int32_t>(rs2_val)
                              ? 1
                              : 0 & 0xFFFFFFFF))
                          ? 1
                          : 0;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SLTUW instruction
void exec_sltuw(Hart *hart, uint32_t instr) {
  // std::cout << "SLTUW instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result =
      (static_cast<uint32_t>((rs1_val & 0xFFFFFFFF) <
                                     static_cast<uint32_t>(rs2_val)
                                 ? 1
                                 : 0 & 0xFFFFFFFF))
          ? 1
          : 0;
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// XORW instruction
void exec_xorw(Hart *hart, uint32_t instr) {
  // std::cout << "XORW instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = static_cast<register_t>(
      static_cast<int64_t>(static_cast<uint32_t>(rs1_val & 0xFFFFFFFF) ^
                           static_cast<uint32_t>(rs2_val & 0xFFFFFFFF)));
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// ORW instruction
void exec_orw(Hart *hart, uint32_t instr) {
  // std::cout << "ORW instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = static_cast<register_t>(
      static_cast<int64_t>(static_cast<uint32_t>(rs1_val & 0xFFFFFFFF) |
                           static_cast<uint32_t>(rs2_val & 0xFFFFFFFF)));
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// ANDW instruction
void exec_andw(Hart *hart, uint32_t instr) {
  // std::cout << "ANDW instruction" << std::endl;
  // R-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  uint32_t funct7 = (instr >> 25) & 0x7F;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  register_t result = static_cast<register_t>(
      static_cast<int64_t>(static_cast<uint32_t>(rs1_val & 0xFFFFFFFF) &
                           static_cast<uint32_t>(rs2_val & 0xFFFFFFFF)));
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// LD instruction
void exec_ld(Hart *hart, uint32_t instr) {
  // std::cout << "LD instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = hart->mem_->read_doubleword(rs1_val + imm);
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// LWU instruction
void exec_lwu(Hart *hart, uint32_t instr) {
  // std::cout << "LWU instruction" << std::endl;
  // I-type instruction fields
  uint8_t rd = (instr >> 7) & 0x1F;
  uint8_t rs1 = (instr >> 15) & 0x1F;
  int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
  if (imm & 0x800) {
    imm |= 0xFFFFF000; // Расширение знака для 32-битного значения
  }

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t result = hart->mem_->read_word(rs1_val + imm) & ((1ULL << 32) - 1);
  if (rd != 0)
    hart->gpr_[rd] = result;
}

// SD instruction
void exec_sd(Hart *hart, uint32_t instr) {
  // std::cout << "SD instruction" << std::endl;
  // S-type instruction fields
  uint8_t rs1 = (instr >> 15) & 0x1F;
  uint8_t rs2 = (instr >> 20) & 0x1F;
  int32_t imm = ((instr >> 25) << 5) | ((instr >> 7) & 0x1F);
  if (imm & 0x800)
    imm |= 0xFFFFF000;

  // Get register values
  register_t rs1_val = hart->gpr_[rs1];
  register_t rs2_val = hart->gpr_[rs2];
  hart->mem_->write_doubleword(rs2_val, rs1_val + imm);
}

void exec_ill(Hart *hart, uint32_t instr) { hart->pc = memory_size + 1; }
} // namespace sim