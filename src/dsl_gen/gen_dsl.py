#!/usr/bin/env python3

import re
import sys
from dataclasses import dataclass
from typing import Dict, List, Optional, Tuple
from enum import Enum

class InstructionType(Enum):
    R_TYPE = "R"
    I_TYPE = "I"
    S_TYPE = "S"
    B_TYPE = "B"
    U_TYPE = "U"
    J_TYPE = "J"
    UNKNOWN = "UNKNOWN"

@dataclass
class Instruction:
    name: str
    opcode: Optional[str] = None
    funct3: Optional[str] = None
    funct7: Optional[str] = None
    type: InstructionType = InstructionType.UNKNOWN
    code: str = ""
    has_imm: bool = False
    is_shift: bool = False
    is_load: bool = False
    is_store: bool = False
    is_branch: bool = False
    is_jump: bool = False
    is_system: bool = False
    is_csr: bool = False
    is_w_instruction: bool = False

def clean_code(code: str) -> str:
    lines = []
    for line in code.split('\n'):
        if '#' in line:
            line = line[:line.index('#')]
        line = line.strip()
        if line:
            lines.append(line)
    return '\n'.join(lines)

def parse_encoding(encoding_str: str) -> tuple:
    encoding_str = encoding_str.strip()
    
    if '*format_r_alu' in encoding_str:
        instr_type = InstructionType.R_TYPE
        match = re.search(r'format_r_alu\(:(\w+)\)', encoding_str)
        if match:
            op_name = match.group(1)
            funct3_map = {
                'add': '0x0', 'sub': '0x0', 'sll': '0x1',
                'slt': '0x2', 'sltu': '0x3', 'xor': '0x4',
                'srl': '0x5', 'sra': '0x5', 'or': '0x6',
                'and': '0x7'
            }
            funct3 = funct3_map.get(op_name, '0x0')
            if op_name in ['sub', 'sra']:
                funct7 = '0x20'
            else:
                funct7 = '0x0'
            return instr_type, '0x33', funct3, funct7
    
    elif '*format_i' in encoding_str:
        instr_type = InstructionType.I_TYPE
        match = re.search(r'format_i\((\w+),\s*(\w+)\)', encoding_str)
        if match:
            return instr_type, match.group(1), match.group(2), None
    
    elif '*format_s' in encoding_str:
        instr_type = InstructionType.S_TYPE
        match = re.search(r'format_s\((\w+),\s*(\w+)\)', encoding_str)
        if match:
            return instr_type, match.group(1), match.group(2), None
    
    elif '*format_b' in encoding_str:
        instr_type = InstructionType.B_TYPE
        match = re.search(r'format_b\((\w+),\s*(\w+)\)', encoding_str)
        if match:
            return instr_type, match.group(1), match.group(2), None
    
    elif '*format_u' in encoding_str:
        instr_type = InstructionType.U_TYPE
        match = re.search(r'format_u\((\w+)\)', encoding_str)
        if match:
            return instr_type, match.group(1), None, None
    
    elif '*format_j' in encoding_str:
        instr_type = InstructionType.J_TYPE
        match = re.search(r'format_j\((\w+)\)', encoding_str)
        if match:
            return instr_type, match.group(1), None, None
    
    elif ':R,' in encoding_str or ':I,' in encoding_str or ':S,' in encoding_str:
        if ':R,' in encoding_str:
            instr_type = InstructionType.R_TYPE
        elif ':I,' in encoding_str:
            instr_type = InstructionType.I_TYPE
        elif ':S,' in encoding_str:
            instr_type = InstructionType.S_TYPE
        
        opcode = None
        funct3 = None
        funct7 = None
        
        fields = re.findall(r'field\(:(\w+),\s*(\d+),\s*(\d+),\s*([:\w]+)\)', encoding_str)
        
        if not fields:
            fields = re.findall(r'field\(:(\w+),\s*(\d+),\s*(\d+),\s*([\w]+)\)', encoding_str)
        
        for field_name, high, low, value in fields:
            if field_name == 'opcode':
                opcode = value
            elif field_name == 'funct3':
                funct3 = value
            elif field_name == 'funct7':
                funct7 = value
        
        return instr_type, opcode, funct3, funct7
    
    return InstructionType.UNKNOWN, None, None, None

def parse_pseudocode(pseudocode: str) -> List[Instruction]:
    instructions = []
    
    pattern = r'Instruction\(:(\w+)\)\s*\{\s*encoding\s*(.*?)\s*code\s*\{(.*?)\}\s*\}'
    
    matches = re.findall(pattern, pseudocode, re.DOTALL)
    
    for idx, match in enumerate(matches):
        name = match[0]
        encoding = match[1].strip()
        code = clean_code(match[2])
        
        instr_type, opcode, funct3, funct7 = parse_encoding(encoding)
        
        instr = Instruction(
            name=name,
            opcode=opcode,
            funct3=funct3,
            funct7=funct7,
            type=instr_type,
            code=code
        )
        
        if instr.type == InstructionType.I_TYPE:
            instr.has_imm = True
            if name in ['slli', 'srli', 'srai', 'slliw', 'srliw', 'sraiw']:
                instr.is_shift = True
            elif name.startswith('l') and name != 'lui':
                instr.is_load = True
            elif name.startswith('csrr'):
                instr.is_csr = True
                instr.is_system = True
            elif name == 'jalr':
                instr.is_jump = True
            elif name.startswith('fence'):
                instr.is_system = True
                
        elif instr.type == InstructionType.S_TYPE:
            instr.has_imm = True
            instr.is_store = True
            
        elif instr.type == InstructionType.B_TYPE:
            instr.has_imm = True
            instr.is_branch = True
            
        elif instr.type == InstructionType.U_TYPE:
            instr.has_imm = True
            
        elif instr.type == InstructionType.J_TYPE:
            instr.has_imm = True
            instr.is_jump = True
            
        if name in ['ecall', 'ebreak', 'uret', 'sret', 'mret', 'wfi', 'sfence_vma']:
            instr.is_system = True
            
        if name.endswith('w'):
            instr.is_w_instruction = True
        
        instructions.append(instr)
    
    return instructions

def generate_header_file(instructions: List[Instruction]) -> str:
    header = """#pragma once

#include <cstdint>
#include "hart.hpp"

namespace sim {
    using register_t = uint32_t;
    
    template<typename T>
    T sign_extend(T value, int bits) {
        T sign_bit = (value >> (bits - 1)) & 1;
        if (sign_bit) {
            T mask = (static_cast<T>(1) << bits) - 1;
            return value | ~mask;
        }
        return value;
    }
    
"""

    generated_count = 0
    for instr in instructions:
        if instr.opcode:
            header += f"    void exec_{instr.name}(Hart* hart, uint32_t instr);\n"
            generated_count += 1
    
    header += """
    bool decode_and_execute(Hart* hart, uint32_t instr);
    
} 
"""
    return header, generated_count

def extract_imm_for_i_type(instr_name: str) -> str:
    if instr_name in ['slli', 'srli', 'srai', 'slliw', 'srliw', 'sraiw']:
        return "    uint8_t shamt = (instr >> 20) & 0x1F;"
    else:
        return """    int32_t imm = static_cast<int32_t>(instr & 0xFFF00000) >> 20;
    if (imm & 0x800) {
        imm |= 0xFFFFF000;
    }"""

def generate_extraction_code(instr: Instruction) -> str:
    code = ""
    
    if instr.type == InstructionType.R_TYPE:
        code += "    uint8_t rd = (instr >> 7) & 0x1F;\n"
        code += "    uint8_t rs1 = (instr >> 15) & 0x1F;\n"
        code += "    uint8_t rs2 = (instr >> 20) & 0x1F;\n"
        if instr.funct7:
            code += f"    uint32_t funct7 = (instr >> 25) & 0x7F;\n"
        
    elif instr.type == InstructionType.I_TYPE:
        code += "    uint8_t rd = (instr >> 7) & 0x1F;\n"
        code += "    uint8_t rs1 = (instr >> 15) & 0x1F;\n"
        code += extract_imm_for_i_type(instr.name)
        
    elif instr.type == InstructionType.S_TYPE:
        code += "    uint8_t rs1 = (instr >> 15) & 0x1F;\n"
        code += "    uint8_t rs2 = (instr >> 20) & 0x1F;\n"
        code += "    int32_t imm = ((instr >> 25) << 5) | ((instr >> 7) & 0x1F);\n"
        code += "    if (imm & 0x800) imm |= 0xFFFFF000;\n"
        
    elif instr.type == InstructionType.B_TYPE:
        code += "    uint8_t rs1 = (instr >> 15) & 0x1F;\n"
        code += "    uint8_t rs2 = (instr >> 20) & 0x1F;\n"
        code += "    int32_t imm = ((instr & 0x80000000) >> 19) |"
        code += " ((instr >> 20) & 0x7E0) |"
        code += " ((instr >> 7) & 0x1E) |"
        code += " ((instr & 0x80) << 4);"
        code += "\n    if (imm & 0x1000) imm |= 0xFFFFE000;\n"
        
    elif instr.type == InstructionType.U_TYPE:
        code += "    uint8_t rd = (instr >> 7) & 0x1F;\n"
        code += "    int32_t imm = instr & 0xFFFFF000;\n"
        
    elif instr.type == InstructionType.J_TYPE:
        code += "    uint8_t rd = (instr >> 7) & 0x1F;\n"
        code += "    int32_t imm = ((instr & 0x80000000) >> 11) |"
        code += " ((instr >> 20) & 0x7FE) |"
        code += " ((instr & 0x100000) >> 9) |"
        code += " ((instr & 0x7FE00000) >> 20);"
        code += "\n    if (imm & 0x100000) imm |= 0xFFE00000;\n"
    
    return code

def memory_read_code(addr_expr: str, bits: str, sign_extend: bool = True) -> str:
    if bits == '7':
        if sign_extend:
            return f"static_cast<int8_t>(hart->mem_->read_byte({addr_expr}))"
        else:
            return f"static_cast<uint8_t>(hart->mem_->read_byte({addr_expr}))"
    elif bits == '15':
        if sign_extend:
            return f"static_cast<int16_t>(hart->mem_->read_halfword({addr_expr}))"
        else:
            return f"static_cast<uint16_t>(hart->mem_->read_halfword({addr_expr}))"
    elif bits == '31':
        return f"hart->mem_->read_word({addr_expr})"
    elif bits == '63':
        return f"hart->mem_->read_doubleword({addr_expr})"
    else:
        return f"0"

def memory_write_code(addr_expr: str, value_expr: str, bits: str) -> str:
    if bits == '7':
        return f"hart->mem_->write_byte({value_expr} & 0xFF, {addr_expr});"
    elif bits == '15':
        return f"hart->mem_->write_halfword({value_expr} & 0xFFFF, {addr_expr});"
    elif bits == '31':
        return f"hart->mem_->write_word({value_expr}, {addr_expr});"
    elif bits == '63':
        return f"hart->mem_->write_doubleword({value_expr}, {addr_expr});"
    else:
        return f""

def translate_pseudocode_to_cpp(code: str, instr: Instruction) -> Tuple[str, bool]:
    original_code = code
    needs_result_var = False
    
    memory_write_pattern = r'memory\[(.*?)\] = rs2\[(\d+)\]'
    
    def replace_memory_write(match):
        addr_expr = match.group(1)
        bits = match.group(2)
        
        addr_expr = re.sub(r'imm_11_5', 'imm', addr_expr)
        addr_expr = re.sub(r'imm_4_0', 'imm', addr_expr)
        addr_expr = re.sub(r'\brs1\b', 'rs1_val', addr_expr)
        
        return memory_write_code(addr_expr, 'rs2_val', bits)
    
    if re.search(memory_write_pattern, code):
        code = re.sub(memory_write_pattern, replace_memory_write, code)
        return code, False
    
    memory_read_pattern = r'rd\[\]\s*=\s*memory\[(.*?)\]\[(\d+)\]'
    
    def replace_memory_read(match):
        addr_expr = match.group(1)
        bits = match.group(2)
        
        addr_expr = re.sub(r'imm_11_0', 'imm', addr_expr)
        addr_expr = re.sub(r'\brs1\b', 'rs1_val', addr_expr)
        
        sign_extend = True
        if instr.name in ['lbu', 'lhu', 'lwu']:
            sign_extend = False
        
        return memory_read_code(addr_expr, bits, sign_extend)
    
    if re.search(memory_read_pattern, code):
        code = re.sub(memory_read_pattern, replace_memory_read, code)
        needs_result_var = True
        return code, needs_result_var
    
    to_i_pattern = r'(\w+)\s*=\s*memory\[(.*?)\]\[(\d+)\]\.to_i\((\d+),\s*(true|false)\)'
    to_i_match = re.search(to_i_pattern, code)
    if to_i_match:
        var_name = to_i_match.group(1)
        addr_expr = to_i_match.group(2)
        bits = to_i_match.group(3)
        result_bits = to_i_match.group(4)
        signed_str = to_i_match.group(5)
        
        addr_expr = re.sub(r'imm_11_0', 'imm', addr_expr)
        addr_expr = re.sub(r'\brs1\b', 'rs1_val', addr_expr)
        
        sign_extend = (signed_str == 'true')
        read_code = memory_read_code(addr_expr, bits, sign_extend)
        
        if result_bits != bits:
            if sign_extend:
                read_code = f"sign_extend<register_t>({read_code}, {result_bits})"
            else:
                read_code = f"{read_code} & ((1ULL << {result_bits}) - 1)"
        
        code = f"{var_name} = {read_code}"
        needs_result_var = True
        return code, needs_result_var
    
    if instr.is_branch:
        match = re.search(r'if\s+(.*?)\s+pc\s*=\s*pc\s*\+\s*(.*?)\s+end', code, re.DOTALL)
        if match:
            condition = match.group(1).strip()
            imm_expr = match.group(2).strip()
            
            condition = condition.replace('rs1', 'rs1_val')
            condition = condition.replace('rs2', 'rs2_val')
            
            if instr.name in ['blt', 'bge']:
                if ' < ' in condition:
                    condition = f"((int32_t)rs1_val < (int32_t)rs2_val)"
                elif ' >= ' in condition:
                    condition = f"((int32_t)rs1_val >= (int32_t)rs2_val)"
            elif instr.name in ['bltu', 'bgeu']:
                if ' < ' in condition:
                    condition = f"((uint32_t)rs1_val < (uint32_t)rs2_val)"
                elif ' >= ' in condition:
                    condition = f"((uint32_t)rs1_val >= (uint32_t)rs2_val)"
            elif '==' in condition:
                condition = condition.replace('==', '==')
            elif '!=' in condition:
                condition = condition.replace('!=', '!=')
            
            return f"if ({condition}) {{\n    hart->pc += imm;\n}}", False
    
    if instr.name == 'jal':
        return code, True
    elif instr.name == 'jalr':
        return "// JALR handled specially", True
    
    if instr.name in ['lui', 'auipc']:
        return code, True
    
    if instr.is_system:
        if instr.name == 'ecall':
            return 'std::cerr << "ECALL instruction" << std::endl; std::exit(1);', False
        elif instr.name == 'ebreak':
            return 'std::cerr << "EBREAK instruction" << std::endl; std::exit(1);', False
        elif instr.name == 'uret':
            return 'std::cerr << "URET instruction (not implemented)" << std::endl; std::exit(1);', False
        elif instr.name == 'sret':
            return '// Return from exception/trap', False
        elif instr.name == 'mret':
            return '// Return from exception/trap', False
        elif instr.name in ['fence', 'fence_i', 'wfi', 'sfence_vma']:
            return '// No-op in basic simulator', False
    
    replacements = [
        (r'rd\[\]', 'result'),
        (r'shamt', 'shamt'),
        (r'imm_11_0', 'imm'),
        (r'imm_31_12', 'imm'),
        (r'imm_11_5', 'imm'),
        (r'imm_4_0', 'imm'),
        (r'imm_12', 'imm'),
        (r'imm_11', 'imm'),
        (r'imm_10_5', 'imm'),
        (r'imm_4_1', 'imm'),
        (r'imm_20', 'imm'),
        (r'imm_19_12', 'imm'),
        (r'imm_10_1', 'imm'),
    ]
    
    for pattern, replacement in replacements:
        code = re.sub(pattern, replacement, code, flags=re.IGNORECASE)
    
    code = re.sub(r'\brs1\b', 'rs1_val', code)
    code = re.sub(r'\brs2\b', 'rs2_val', code)
    
    code = re.sub(r'sign_extend\(([^,]+),\s*(\d+)\)', r'sign_extend<register_t>(\1, \2)', code)
    
    code = re.sub(r'zero_extend\(([^,]+),\s*(\d+)\)', r'\1 & ((1ULL << \2) - 1)', code)
    
    if 'result' in code or 'rd[]' in original_code:
        needs_result_var = True
    
    if instr.is_csr:
        code = re.sub(r'csr\[imm_11_0\]', 'hart->csr_[imm & 0xFFF]', code)
        code = re.sub(r'csr\[0x(\w+)\]', r'hart->csr_[0x\1]', code)
        code = re.sub(r'csr\[imm\]', 'hart->csr_[imm & 0xFFF]', code)
        code = re.sub(r'csr\[(\w+)\]', r'hart->csr_[\1 & 0xFFF]', code)
        if instr.name in ['csrrwi', 'csrrsi', 'csrrci']:
            code = re.sub(r'\brs1\b', '(rs1 & 0x1F)', code)
    
    return code, needs_result_var

def generate_cpp_function(instr: Instruction) -> str:
    func_name = f"exec_{instr.name}"
    
    code = f"\nvoid exec_{instr.name}(Hart* hart, uint32_t instr) {{\n"
    
    code += generate_extraction_code(instr)
    
    if instr.type in [InstructionType.R_TYPE, InstructionType.I_TYPE, 
                     InstructionType.B_TYPE, InstructionType.S_TYPE]:
        code += "\n"
        if instr.type in [InstructionType.R_TYPE, InstructionType.I_TYPE, 
                         InstructionType.B_TYPE, InstructionType.S_TYPE]:
            code += f"    register_t rs1_val = hart->gpr_[rs1];\n"
        if instr.type in [InstructionType.R_TYPE, InstructionType.B_TYPE, 
                         InstructionType.S_TYPE]:
            code += f"    register_t rs2_val = hart->gpr_[rs2];\n"
    
    cpp_code, needs_result_var = translate_pseudocode_to_cpp(instr.code, instr)
    
    if instr.is_load:
        if cpp_code and not cpp_code.startswith("//"):
            if 'register_t result = ' in cpp_code or 'result = ' in cpp_code:
                code += f"    {cpp_code}\n"
            else:
                code += f"    register_t result = {cpp_code};\n"
            code += "    if (rd != 0) hart->gpr_[rd] = result;\n"
        else:
            code += "    \n"
    
    elif instr.is_store:
        if cpp_code and not cpp_code.startswith("//"):
            code += f"    {cpp_code}\n"
    
    elif instr.name in ['jal', 'jalr']:
        if instr.name == 'jal':
            code += "    register_t result = hart->pc + 4;\n"
            code += "    hart->pc += imm;\n"
            code += "    if (rd != 0) hart->gpr_[rd] = result;\n"
        elif instr.name == 'jalr':
            code += "    register_t result = hart->pc + 4;\n"
            code += "    hart->pc = (rs1_val + imm) & ~1;\n"
            code += "    if (rd != 0) hart->gpr_[rd] = result;\n"
    
    elif instr.name in ['lui', 'auipc']:
        if instr.name == 'lui':
            code += "    register_t result = imm;\n"
        elif instr.name == 'auipc':
            code += "    register_t result = hart->pc + imm;\n"
        code += "    if (rd != 0) hart->gpr_[rd] = result;\n"
    
    elif instr.is_csr:
        if 'hart->csr_' in cpp_code or 'tmp =' in cpp_code:
            csr_lines = cpp_code.split('\n')
            for line in csr_lines:
                line = line.strip()
                if line:
                    if 'tmp =' in line:
                        code += f"    register_t tmp = {line.split('=')[1].strip()};\n"
                    elif 'hart->csr_' in line and not line.startswith('//'):
                        code += f"    {line};\n"
                    elif 'rd[] =' in line or 'result =' in line:
                        pass
            code += "    if (rd != 0) hart->gpr_[rd] = tmp;\n"
        else:
            code += "    \n"
    
    elif instr.is_system:
        if cpp_code and not cpp_code.startswith("//"):
            code += f"    {cpp_code}\n"
    
    elif instr.is_branch:
        if cpp_code and not cpp_code.startswith("//"):
            code += f"    {cpp_code}\n"
        else:
            code += "    \n"
    
    else:
        if needs_result_var:
            expr_match = re.search(r'rd\[\]\s*=\s*(.*)', instr.code)
            if expr_match:
                expr = expr_match.group(1)
                
                expr = re.sub(r'\brs1\b', 'rs1_val', expr)
                expr = re.sub(r'\brs2\b', 'rs2_val', expr)
                expr = re.sub(r'imm_11_0', 'imm', expr)
                expr = re.sub(r'shamt', 'shamt', expr)
                
                if ' < ' in expr and instr.name in ['slt', 'slti', 'sltu', 'sltiu', 'sltw', 'sltuw']:
                    left, right = expr.split(' < ')
                    left = left.strip()
                    right = right.strip()
                    
                    if instr.name in ['slt', 'slti', 'sltw']:
                        expr = f"((int32_t){left}) < (int32_t)({right})"
                    elif instr.name in ['sltu', 'sltiu', 'sltuw']:
                        expr = f"((uint32_t){left}) < (int32_t)({right})"
                elif ' >= ' in expr:
                    pass
                elif instr.name == 'sra':
                    expr = f"static_cast<int32_t>(rs1_val) >> (rs2_val & 0x1F)"
                elif instr.name == 'srai':
                    expr = f"static_cast<int32_t>(rs1_val) >> shamt"
                elif instr.name == 'srl':
                    expr = f"static_cast<uint32_t>(rs1_val) >> (rs2_val & 0x1F)"
                elif instr.name == 'srli':
                    expr = f"static_cast<uint32_t>(rs1_val) >> shamt"
                elif instr.name == 'sll':
                    expr = f"rs1_val << (rs2_val & 0x1F)"
                elif instr.name == 'slli':
                    expr = f"rs1_val << shamt"
                elif instr.name == 'addiw':
                    expr = 'static_cast<register_t>(static_cast<int64_t>(static_cast<int32_t>(rs1_val & 0xFFFFFFFF) + imm))'
                elif instr.name == 'slliw':
                    expr = f'static_cast<register_t>(static_cast<int64_t>(static_cast<int32_t>(rs1_val & 0xFFFFFFFF) << shamt))'
                elif instr.name == 'srliw':
                    expr = f'static_cast<register_t>(static_cast<uint32_t>(rs1_val & 0xFFFFFFFF) >> shamt)'
                elif instr.name == 'sraiw':
                    expr = f'static_cast<register_t>(static_cast<int64_t>(static_cast<int32_t>(rs1_val & 0xFFFFFFFF) >> shamt))'
                elif instr.name == 'addw':
                    expr = f'static_cast<register_t>(static_cast<int64_t>(static_cast<int32_t>(rs1_val & 0xFFFFFFFF) + static_cast<int32_t>(rs2_val & 0xFFFFFFFF)))'
                elif instr.name == 'subw':
                    expr = f'static_cast<register_t>(static_cast<int64_t>(static_cast<int32_t>(rs1_val & 0xFFFFFFFF) - static_cast<int32_t>(rs2_val & 0xFFFFFFFF)))'
                elif instr.name == 'sllw':
                    expr = f'static_cast<register_t>(static_cast<int64_t>(static_cast<int32_t>(rs1_val & 0xFFFFFFFF) << (rs2_val & 0x1F)))'
                elif instr.name == 'srlw':
                    expr = f'static_cast<register_t>(static_cast<uint32_t>(rs1_val & 0xFFFFFFFF) >> (rs2_val & 0x1F))'
                elif instr.name == 'sraw':
                    expr = f'static_cast<register_t>(static_cast<int64_t>(static_cast<int32_t>(rs1_val & 0xFFFFFFFF) >> (rs2_val & 0x1F)))'
                elif instr.name == 'xorw':
                    expr = f'static_cast<register_t>(static_cast<int64_t>(static_cast<uint32_t>(rs1_val & 0xFFFFFFFF) ^ static_cast<uint32_t>(rs2_val & 0xFFFFFFFF)))'
                elif instr.name == 'orw':
                    expr = f'static_cast<register_t>(static_cast<int64_t>(static_cast<uint32_t>(rs1_val & 0xFFFFFFFF) | static_cast<uint32_t>(rs2_val & 0xFFFFFFFF)))'
                elif instr.name == 'andw':
                    expr = f'static_cast<register_t>(static_cast<int64_t>(static_cast<uint32_t>(rs1_val & 0xFFFFFFFF) & static_cast<uint32_t>(rs2_val & 0xFFFFFFFF)))'
                elif instr.name in ['addi', 'add', 'sub', 'xor', 'or', 'and']:
                    pass
                
                code += f"    register_t result = {expr};\n"
            else:
                if 'register_t result = ' in cpp_code:
                    code += f"    {cpp_code}\n"
                else:
                    code += f"    register_t result = {cpp_code};\n"
            
            code += "    if (rd != 0) hart->gpr_[rd] = result;\n"
        else:
            if cpp_code and not cpp_code.startswith("//"):
                if not cpp_code.endswith(';'):
                    cpp_code += ';'
                code += f"    {cpp_code}\n"
    
    code += "}\n"
    return code

def transform_generated_code(impl_text: str) -> str:
    lines = impl_text.split('\n')
    transformed_lines = []
    i = 0
    
    while i < len(lines):
        line = lines[i]
        
        if 'int32_t imm = static_cast<int32_t>(instr << 20) >> 20;' in line:
            transformed_lines.append(line)
            transformed_lines.append('    if (imm & 0x800) {')
            transformed_lines.append('        imm |= 0xFFFFF000;')
            transformed_lines.append('    }')
            i += 1
            continue
            
        if '((int32_t)(rs1_val) < (int32_t)(rs2_val) ? 1 : 0)' in line:
            line = line.replace('((int32_t)(rs1_val) < (int32_t)(rs2_val) ? 1 : 0)',
                               '((int32_t)(rs1_val < (int32_t)rs2_val) ? 1 : 0)')
        elif '((int32_t)(rs1_val) < (int32_t)(imm) ? 1 : 0)' in line:
            line = line.replace('((int32_t)(rs1_val) < (int32_t)(imm) ? 1 : 0)',
                               '((int32_t)(rs1_val < (int32_t)imm) ? 1 : 0)')
        elif '((uint32_t)(rs1_val) < (int32_t)(rs2_val) ? 1 : 0)' in line:
            line = line.replace('((uint32_t)(rs1_val) < (int32_t)(rs2_val) ? 1 : 0)',
                               '((uint32_t)(rs1_val < (uint32_t)rs2_val) ? 1 : 0)')
        elif '((uint32_t)(rs1_val) < (int32_t)(imm) ? 1 : 0)' in line:
            line = line.replace('((uint32_t)(rs1_val) < (int32_t)(imm) ? 1 : 0)',
                               '((uint32_t)(rs1_val < (uint32_t)imm) ? 1 : 0)')
        
        if 'hart->pc += imm;' in line and not 'hart->pc += imm - 4;' in lines[i-1] and not 'hart->pc += imm - 4;' in lines[i+1]:
            if any(x in ''.join(lines[max(0, i-5):i]) for x in ['exec_beq', 'exec_bne', 'exec_blt', 'exec_bge', 
                                                                'exec_bltu', 'exec_bgeu', 'exec_jal', 'exec_jalr']):
                line = line.replace('hart->pc += imm;', 'hart->pc += imm - 4;')
        
        if 'rs1_val + imm + imm' in line:
            line = line.replace('rs1_val + imm + imm', 'rs1_val + imm')
        
        if 'exec_jal(hart, instr)' in line and i > 0 and 'case' in lines[i-1]:
            for j in range(i, max(0, i-50), -1):
                if 'void exec_jal(Hart* hart, uint32_t instr)' in lines[j]:
                    start_j = j
                    while lines[start_j].strip() != '}':
                        start_j += 1
                    
                    transformed_lines.extend(lines[:j])
                    
                    jal_fixed = '''void exec_jal(Hart* hart, uint32_t instr) {
    uint8_t rd = (instr >> 7) & 0x1F;
    
    uint32_t imm_20    = (instr >> 31) & 0x1;
    uint32_t imm_10_1  = (instr >> 21) & 0x3FF;
    uint32_t imm_11    = (instr >> 20) & 0x1;
    uint32_t imm_19_12 = (instr >> 12) & 0xFF;
    
    uint32_t imm_unsigned = (imm_20    << 20) |
                            (imm_19_12 << 12) |
                            (imm_11    << 11) |
                            (imm_10_1  << 1);

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
}'''
                    transformed_lines.append(jal_fixed)
                    
                    i = start_j + 1
                    continue
        
        if 'exec_jalr(hart, instr)' in line and i > 0 and 'case' in lines[i-1]:
            for j in range(i, max(0, i-50), -1):
                if 'void exec_jalr(Hart* hart, uint32_t instr)' in lines[j]:
                    start_j = j
                    while lines[start_j].strip() != '}':
                        start_j += 1
                    
                    transformed_lines.extend(lines[:j])
                    
                    jalr_fixed = '''void exec_jalr(Hart* hart, uint32_t instr) {
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
}'''
                    transformed_lines.append(jalr_fixed)
                    
                    i = start_j + 1
                    continue
        
        if 'exec_sltw' in line or 'exec_sltuw' in line:
            for j in range(i, len(lines)):
                if 'void exec_sltw' in lines[j] or 'void exec_sltuw' in lines[j]:
                    func_start = j
                    func_end = j
                    while func_end < len(lines) and lines[func_end].strip() != '}':
                        func_end += 1
                    
                    func_lines = lines[func_start:func_end+1]
                    func_text = '\n'.join(func_lines)
                    
                    if 'exec_sltw' in line:
                        fixed_sltw = '''void exec_sltw(Hart* hart, uint32_t instr) {
    uint8_t rd = (instr >> 7) & 0x1F;
    uint8_t rs1 = (instr >> 15) & 0x1F;
    uint8_t rs2 = (instr >> 20) & 0x1F;
    uint32_t funct7 = (instr >> 25) & 0x7F;

    register_t rs1_val = hart->gpr_[rs1];
    register_t rs2_val = hart->gpr_[rs2];
    register_t result = (static_cast<int32_t>((rs1_val & 0xFFFFFFFF) < static_cast<int32_t>(rs2_val) ? 1 : 0 & 0xFFFFFFFF)) ? 1 : 0;
    if (rd != 0) hart->gpr_[rd] = result;
}'''
                        transformed_lines.append(fixed_sltw)
                    else:
                        fixed_sltuw = '''void exec_sltuw(Hart* hart, uint32_t instr) {
    uint8_t rd = (instr >> 7) & 0x1F;
    uint8_t rs1 = (instr >> 15) & 0x1F;
    uint8_t rs2 = (instr >> 20) & 0x1F;
    uint32_t funct7 = (instr >> 25) & 0x7F;

    register_t rs1_val = hart->gpr_[rs1];
    register_t rs2_val = hart->gpr_[rs2];
    register_t result = (static_cast<uint32_t>((rs1_val & 0xFFFFFFFF) < static_cast<uint32_t>(rs2_val) ? 1 : 0 & 0xFFFFFFFF)) ? 1 : 0;
    if (rd != 0) hart->gpr_[rd] = result;
}'''
                        transformed_lines.append(fixed_sltuw)
                    
                    i = func_end + 1
                    continue
        
        if 'int32_t imm = instr & 0xFFFFF000;' in line and not '// imm[31:12] << 12' in line:
            line = line.replace('int32_t imm = instr & 0xFFFFF000;',
                               'int32_t imm = instr & 0xFFFFF000;  // imm[31:12] << 12')
        
        if any(x in line for x in ['rs1_signed', 'rs2_signed', 'rs1_32', 'rs2_32']):
            line = line.replace('rs1_signed', 'rs1_val')
            line = line.replace('rs2_signed', 'rs2_val')
            line = line.replace('rs1_32', 'rs1_val')
            line = line.replace('rs2_32', 'rs2_val')
        
        if '.to_i(64)' in line:
            line = line.replace('.to_i(64)', '')
        
        transformed_lines.append(line)
        i += 1
    
    result = '\n'.join(transformed_lines)
    
    decode_pattern = r'case 0x0:[\s\n]*exec_(\w+)'
    def add_comment_to_case(match):
        instr_name = match.group(1)
        if instr_name in ['srli', 'srai', 'srl', 'sra', 'add', 'sub', 'addw', 'subw', 
                         'srlw', 'sraw', 'srliw', 'sraiw']:
            return f'case 0x0: // {instr_name}'
        return match.group(0)
    
    result = re.sub(decode_pattern, add_comment_to_case, result)
    
    decode_pattern2 = r'case 0x20:[\s\n]*exec_(\w+)'
    def add_comment_to_case2(match):
        instr_name = match.group(1)
        if instr_name in ['srai', 'sra', 'sub', 'sraw', 'sraiw', 'subw']:
            return f'case 0x20: // {instr_name}'
        return match.group(0)
    
    result = re.sub(decode_pattern2, add_comment_to_case2, result)
    
    decode_pattern3 = r'case 0x09:[\s\n]*exec_(\w+)'
    def add_comment_to_case3(match):
        instr_name = match.group(1)
        if instr_name == 'sfence_vma':
            return f'case 0x09: // {instr_name}'
        return match.group(0)
    
    result = re.sub(decode_pattern3, add_comment_to_case3, result)
    
    return result

def generate_implementation_file(instructions: List[Instruction]) -> str:
    impl = """#include "generated_instructions.hpp"
#include <cstdint>
#include <iostream>
#include <cstdlib>

namespace sim {

"""

    generated_count = 0
    for instr in instructions:
        if instr.opcode:
            impl += generate_cpp_function(instr)
            generated_count += 1
    
    impl += """
bool decode_and_execute(Hart* hart, uint32_t instr) {
    if (!hart || !hart->mem_) return false;
    
    uint8_t opcode = instr & 0x7F;
    uint8_t funct3 = (instr >> 12) & 0x7;
    uint8_t funct7 = (instr >> 25) & 0x7F;
    
    switch (opcode) {
"""
    
    opcode_groups = {}
    for instr in instructions:
        if instr.opcode:
            if instr.opcode.startswith('0x'):
                opcode_key = instr.opcode
            else:
                try:
                    opcode_key = f'0x{int(instr.opcode):02X}'
                except ValueError:
                    opcode_key = f'0x{instr.opcode}'
            
            if opcode_key not in opcode_groups:
                opcode_groups[opcode_key] = []
            opcode_groups[opcode_key].append(instr)
    
    for opcode_key in sorted(opcode_groups.keys()):
        instr_list = opcode_groups[opcode_key]
        impl += f"        case {opcode_key}: {{\n"
        
        if len(instr_list) == 1 and instr_list[0].funct3 is None:
            instr = instr_list[0]
            impl += f"            exec_{instr.name}(hart, instr);\n"
            impl += f"            break;\n"
            impl += f"            }}\n"
            continue
        
        impl += f"            switch (funct3) {{\n"
        
        funct3_groups = {}
        for instr in instr_list:
            if instr.funct3 is not None:
                if instr.funct3.startswith('0x'):
                    funct3_key = instr.funct3
                else:
                    try:
                        funct3_key = f'0x{int(instr.funct3):01X}'
                    except ValueError:
                        funct3_key = f'0x{instr.funct3}'
                
                if funct3_key not in funct3_groups:
                    funct3_groups[funct3_key] = []
                funct3_groups[funct3_key].append(instr)
        
        if not funct3_groups:
            impl += "                default:\n"
            impl += '                    std::cerr << "Illegal instruction (no funct3 match)" << std::endl;\n'
            impl += "                    std::exit(1);\n"
        else:
            for funct3_key in sorted(funct3_groups.keys()):
                sub_instr_list = funct3_groups[funct3_key]
                impl += f"                case {funct3_key}: {{\n"
                
                if len(sub_instr_list) == 1:
                    instr = sub_instr_list[0]
                    if instr.funct7 is None:
                        impl += f"                    exec_{instr.name}(hart, instr);\n"
                    else:
                        if instr.funct7.startswith('0x'):
                            funct7_val = instr.funct7
                        else:
                            try:
                                funct7_val = f'0x{int(instr.funct7):02X}'
                            except ValueError:
                                funct7_val = f'0x{instr.funct7}'
                        
                        impl += f"                    if (funct7 == {funct7_val}) {{\n"
                        impl += f"                        exec_{instr.name}(hart, instr);\n"
                        impl += f"                    }} else {{\n"
                        impl += f'                        std::cerr << "Illegal instruction (wrong funct7)" << std::endl;\n'
                        impl += f"                        std::exit(1);\n"
                        impl += f"                    }}\n"
                else:
                    impl += "                    switch (funct7) {\n"
                    for instr in sub_instr_list:
                        if instr.funct7 is not None:
                            if instr.funct7.startswith('0x'):
                                funct7_val = instr.funct7
                            else:
                                try:
                                    funct7_val = f'0x{int(instr.funct7):02X}'
                                except ValueError:
                                    funct7_val = f'0x{instr.funct7}'
                            
                            impl += f"                        case {funct7_val}: \n"
                            impl += f"                            exec_{instr.name}(hart, instr);\n"
                            impl += f"                            break;\n"
                    impl += "                        default:\n"
                    impl += '                            std::cerr << "Illegal instruction (no funct7 match)" << std::endl;\n'
                    impl += "                            std::exit(1);\n"
                    impl += "                    }\n"
                
                impl += "                    break;\n"
                impl += "                }\n"
        
        impl += "            }\n"
        impl += "            break;\n"
        impl += "        }\n"
    
    impl += """        default:
            return false;
    }
    
    return true;
}

} 
"""
    
    return impl, generated_count

def main():
    if len(sys.argv) > 1:
        with open(sys.argv[1], 'r') as f:
            pseudocode = f.read()
    else:
        pseudocode = sys.stdin.read()
    
    instructions = parse_pseudocode(pseudocode)
    
    valid_instructions = [instr for instr in instructions if instr.opcode]
    
    header, header_count = generate_header_file(valid_instructions)
    impl, impl_count = generate_implementation_file(valid_instructions)
    
    transformed_impl = transform_generated_code(impl)
    
    with open("generated_instructions.hpp", "w") as f:
        f.write(header)
    
    with open("generated_instructions.cpp", "w") as f:
        f.write(transformed_impl)
    
    print(f"\nGenerated in header: {header_count}")
    print(f"Generated in .cpp: {impl_count}")

if __name__ == "__main__":
    main()