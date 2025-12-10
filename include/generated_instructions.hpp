#pragma once

#include <cstdint>

namespace sim {

class Hart;

using register_t = uint32_t;

template <typename T> T sign_extend(T value, int bits) {
  T sign_bit = (value >> (bits - 1)) & 1;
  if (sign_bit) {
    T mask = (static_cast<T>(1) << bits) - 1;
    return value | ~mask;
  }
  return value;
}

void exec_add(Hart *hart, uint32_t instr);
void exec_sub(Hart *hart, uint32_t instr);
void exec_sll(Hart *hart, uint32_t instr);
void exec_slt(Hart *hart, uint32_t instr);
void exec_sltu(Hart *hart, uint32_t instr);
void exec_xor(Hart *hart, uint32_t instr);
void exec_srl(Hart *hart, uint32_t instr);
void exec_sra(Hart *hart, uint32_t instr);
void exec_or(Hart *hart, uint32_t instr);
void exec_and(Hart *hart, uint32_t instr);
void exec_addi(Hart *hart, uint32_t instr);
void exec_slti(Hart *hart, uint32_t instr);
void exec_sltiu(Hart *hart, uint32_t instr);
void exec_xori(Hart *hart, uint32_t instr);
void exec_ori(Hart *hart, uint32_t instr);
void exec_andi(Hart *hart, uint32_t instr);
void exec_slli(Hart *hart, uint32_t instr);
void exec_srli(Hart *hart, uint32_t instr);
void exec_srai(Hart *hart, uint32_t instr);
void exec_lb(Hart *hart, uint32_t instr);
void exec_lh(Hart *hart, uint32_t instr);
void exec_lw(Hart *hart, uint32_t instr);
void exec_lbu(Hart *hart, uint32_t instr);
void exec_lhu(Hart *hart, uint32_t instr);
void exec_sb(Hart *hart, uint32_t instr);
void exec_sh(Hart *hart, uint32_t instr);
void exec_sw(Hart *hart, uint32_t instr);
void exec_beq(Hart *hart, uint32_t instr);
void exec_bne(Hart *hart, uint32_t instr);
void exec_blt(Hart *hart, uint32_t instr);
void exec_bge(Hart *hart, uint32_t instr);
void exec_bltu(Hart *hart, uint32_t instr);
void exec_bgeu(Hart *hart, uint32_t instr);
void exec_jal(Hart *hart, uint32_t instr);
void exec_jalr(Hart *hart, uint32_t instr);
void exec_lui(Hart *hart, uint32_t instr);
void exec_auipc(Hart *hart, uint32_t instr);
void exec_fence(Hart *hart, uint32_t instr);
void exec_fence_i(Hart *hart, uint32_t instr);
void exec_csrrw(Hart *hart, uint32_t instr);
void exec_csrrs(Hart *hart, uint32_t instr);
void exec_csrrc(Hart *hart, uint32_t instr);
void exec_csrrwi(Hart *hart, uint32_t instr);
void exec_csrrsi(Hart *hart, uint32_t instr);
void exec_csrrci(Hart *hart, uint32_t instr);
void exec_ecall(Hart *hart, uint32_t instr);
void exec_ebreak(Hart *hart, uint32_t instr);
void exec_uret(Hart *hart, uint32_t instr);
void exec_sret(Hart *hart, uint32_t instr);
void exec_mret(Hart *hart, uint32_t instr);
void exec_wfi(Hart *hart, uint32_t instr);
void exec_sfence_vma(Hart *hart, uint32_t instr);
void exec_addiw(Hart *hart, uint32_t instr);
void exec_slliw(Hart *hart, uint32_t instr);
void exec_srliw(Hart *hart, uint32_t instr);
void exec_sraiw(Hart *hart, uint32_t instr);
void exec_addw(Hart *hart, uint32_t instr);
void exec_subw(Hart *hart, uint32_t instr);
void exec_sllw(Hart *hart, uint32_t instr);
void exec_srlw(Hart *hart, uint32_t instr);
void exec_sraw(Hart *hart, uint32_t instr);
void exec_sltw(Hart *hart, uint32_t instr);
void exec_sltuw(Hart *hart, uint32_t instr);
void exec_xorw(Hart *hart, uint32_t instr);
void exec_orw(Hart *hart, uint32_t instr);
void exec_andw(Hart *hart, uint32_t instr);
void exec_ld(Hart *hart, uint32_t instr);
void exec_lwu(Hart *hart, uint32_t instr);
void exec_sd(Hart *hart, uint32_t instr);
void exec_ill(Hart *hart, uint32_t instr);
} // namespace sim
