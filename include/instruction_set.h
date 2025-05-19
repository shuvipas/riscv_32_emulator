#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H
#include "cpu.h"
#include "dram.h"
//instructions refrence https://five-embeddev.com/riscv-user-isa-manual/Priv-v1.12/instr-table.html
void execute_r_type(CPU *cpu, word *ins);
void execute_s_type(CPU *cpu, DRAM *dram, word *ins);
void execute_j_type(CPU *cpu, word *ins);
void execute_u_type(CPU *cpu, word *ins);
void execute_b_type(CPU * cpu, word *ins);
void execute_i_type(CPU *cpu, word *ins);
void execute_i_immediate(byte funct3,byte funct7,word imm,word *rd_ptr,word rs1);
void execute_i_load(byte funct3,word imm,word *rd_ptr,word rs1);
/*
    opcode from the book:
Digital Design and Computer Architecture: RISC-V Edition
Sarah L. Harris and David Harris

*/
typedef enum
{
    I_LOAD = 3,
    I_IMMEDIATE = 19,
    I_JUMP = 103,

    U_ADD = 23,
    U_LOAD = 55,

    S = 35,

    R = 51,
 
    B = 99,

    J = 111
} INS_TYPE;

typedef struct
{
    byte opcode : 8;
    byte rd : 5;
    byte funct3 : 3;
    byte rs1 : 5;
    byte rs2 : 5;
    byte funct7 : 7;
} R_TYPE;
typedef struct
{
    byte opcode : 8;
    byte rd : 5;
    byte funct3 : 3;
    byte rs1 : 5;
    word imm : 12;
} I_TYPE;
typedef struct
{
    byte opcode : 8;
    byte imm_3 : 1;
    byte imm_1 : 4;
    byte funct3 : 3;
    byte rs1 : 5;
    byte rs2 : 5;
    byte imm_2 : 6;
    byte imm_4 : 1;
} B_TYPE;
typedef struct
{
    byte opcode : 8;
    byte imm_0 : 5;
    byte funct3 : 3;
    byte rs1 : 5;
    byte rs2 : 5;
    byte imm_1 : 7;
} S_TYPE;

typedef struct
{
    byte opcode : 8;
    byte rd : 5;
    word imm : 19;
} U_TYPE;
typedef struct
{
    byte opcode : 8;
    byte rd : 5;
    word imm_3 : 8;
    word imm_2 : 1;
    word imm_1 : 10;
    word imm_4 : 1;
} J_TYPE;

#endif