#pragma once

#include <cstdint>

namespace sim {
enum class RiscvRegisters : uint8_t {
  zero, // x0 - Hard-wired zero
  ra,   // x1 - Return address
  sp,   // x2 - Stack pointer
  gp,   // x3 - Global pointer
  tp,   // x4 - Thread pointer
  t0,   // x5 - Temporary
  t1,   // x6 - Temporary
  t2,   // x7 - Temporary
  s0,   // x8 - Saved register / Frame pointer
  s1,   // x9 - Saved register
  a0,   // x10 - Function argument / Return value
  a1,   // x11 - Function argument / Return value
  a2,   // x12 - Function argument
  a3,   // x13 - Function argument
  a4,   // x14 - Function argument
  a5,   // x15 - Function argument
  a6,   // x16 - Function argument
  a7,   // x17 - Function argument
  s2,   // x18 - Saved register
  s3,   // x19 - Saved register
  s4,   // x20 - Saved register
  s5,   // x21 - Saved register
  s6,   // x22 - Saved register
  s7,   // x23 - Saved register
  s8,   // x24 - Saved register
  s9,   // x25 - Saved register
  s10,  // x26 - Saved register
  s11,  // x27 - Saved register
  t3,   // x28 - Temporary
  t4,   // x29 - Temporary
  t5,   // x30 - Temporary
  t6    // x31 - Temporary
};
}