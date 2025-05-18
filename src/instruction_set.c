#include <stdio.h>
#include "instruction_set.h"
#include "dram.h"

void execute_s_type(CPU *cpu, DRAM *dram, word *ins)
{
    SB_TYPE *ins_format = (SB_TYPE *)ins;
    word *rs2_ptr = &cpu->reg[ins_format->rs2];

    byte funct3 = ins_format->funct3;
    word rs1 = cpu->reg[ins_format->rs1];

    word imm0 = ins_format->imm_0;
    word imm1 = ins_format->imm_1;
    word imm = imm1 << 5 | imm0;
    word addres = imm + rs1;
    switch (funct3)
    {
    case 0: // sb - store byte
        dram_store(dram, addres, rs2_ptr, sizeof(byte));
        break;
    case 1: // sh - store half
        dram_store(dram, addres, rs2_ptr, sizeof(half_word));
        break;
    case 2: // sw - store word
        dram_store(dram, addres, rs2_ptr, sizeof(word));
        break;
    default:
        printf("ERROR: execute_s_type funct3 = %d is not sopported\n", funct3);
        break;
    }
}
void execute_r_type(CPU *cpu, word *ins)
{
    R_TYPE *ins_format = (R_TYPE *)ins;
    word *rd_ptr = &cpu->reg[ins_format->rd];

    byte funct3 = ins_format->funct3;
    word rs1 = cpu->reg[ins_format->rs1];
    word rs2 = cpu->reg[ins_format->rs2];
    byte funct7 = ins_format->funct7;

    if (funct7 == 0)
    {
        switch (funct3)
        {
        case 0: // add
            *rd_ptr = (int32_t)rs1 + (int32_t)rs2;
            break;
        case 1: // sll-shift left logical
            *rd_ptr = rs1 << rs2;
            break;
        case 2: // slt- set less than
            *rd_ptr = ((int32_t)rs1 < (int32_t)rs2) ? 1 : 0;
            break;
        case 3: // sltu - set less than unsigned
            *rd_ptr = (rs1 < rs2) ? 1 : 0;
            break;
        case 4: // xor
            *rd_ptr = rs1 ^ rs2;
            break;
        case 5: // srl -shift right logical
            *rd_ptr = rs1 >> rs2;
            break;
        case 6: // or
            *rd_ptr = rs1 | rs2;
            break;
        case 7: // and
            *rd_ptr = rs1 & rs2;
            break;

        default:
            printf("ERROR: execute_r_type funct3 = %d is not sopported\n", funct3);
            break;
        }
    }
    else if (ins_format->funct7 == 0b0100000)
    {
        switch (funct3)
        {
        case 0: // sub
            // todo check for negitive
            *rd_ptr = (int32_t)rs1 - (int32_t)rs2;
            break;
        case 5: // sra - shift right arithmetic
            if ((int32_t)rs1 >= 0)
            {
                *rd_ptr = rs1 >> rs2;
            }
            else
            {
                *rd_ptr = (rs1 >> rs2) | ~(~0U >> rs2);
            }
            break;
        default:
            printf("ERROR: execute_r_type funct3 = %d is not sopported\n", funct3);
            break;
        }
    }
    else
    {
        printf("ERROR: execute_r_type funct7 = %d is not sopported\n", funct7);
    }
}

// void execute_i_type(CPU *cpu, DRAM *ram, word ins) {}
void execute_u_type(CPU *cpu, word ins)
{
    UJ_TYPE *ins_format = (UJ_TYPE *)ins;
    word *rd_ptr = &cpu->reg[ins_format->rd];
    word imm = ins_format->imm;
    switch (ins_format->opcode)
    {
    case U_ADD: // auipc - add upper immediate to PC (rd = {upimm, 12'b0} + PC)

        *rd_ptr = (imm << 12) + cpu->pc;
        break;
    case U_LOAD: // lui-load upper immediate (rd = {upimm, 12’b0})
        *rd_ptr = imm << 12;
        break;
    default:
        printf("ERROR: execute_u_type opcode = %d is not sopported\n", ins_format->opcode);
        break;
    }
}
void execute_j_type(CPU *cpu, word ins)
{
    UJ_TYPE *ins_format = (UJ_TYPE *)ins;
    word *rd_ptr = &cpu->reg[ins_format->rd];
    word imm = ins_format->imm;
    word j_imm = ((imm & 0x80000000) >> 11) | // Bit 31 to position 20
                 ((imm & 0x7FE00000) >> 20) | // Bits 30:21 to positions 10:1
                 ((imm & 0x00100000) >> 9) |  // Bit 20 to position 11
                 (imm & 0x000FF000);          // Bits 19:12 remain in positions 19:12
    if (j_imm & 0x00100000) j_imm = j_imm | 0xffe00000; // Sign extension.

    switch (ins_format->opcode)
    {
    case J: // jal -jump and link (PC = JTA, rd = PC + 4)
        *rd_ptr = cpu->pc + 4;
        cpu->pc = cpu->pc + j_imm -4 ;
        break;
    default:
        printf("ERROR: execute_j_type opcode = %d is not sopported\n", ins_format->opcode);
        break;
    }
}
// void execute_b_type(CPU *cpu, DRAM *ram, word ins) {}
// void execute_u_type(CPU *cpu, DRAM *ram, word ins) {}
