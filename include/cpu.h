#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef uint8_t byte;
typedef uint32_t word;

/*
Register Categories
Type	        Registers	        ABI Names
Zero Register	    x0	            zero
Return Address	    x1	            ra
Stack Pointer       x2              sp
Global Pointer      x3              gp
Thread Pointer      x4              tp
Temporaries         x5–x7, x28–x31  t0–t6
Saved Reg(frame ptr)x8              s0/fp
Saved Registers	    x9, x18–x27	    s1–s11
Function arguments	x10–x17	        a0–a7
*/

typedef struct
{
    uint32_t reg[32];
    uint32_t pc;
} CPU;
/*
    opcode from the book:
Digital Design and Computer Architecture: RISC-V Edition
Sarah L. Harris and David Harris

*/
typedef enum
{
    I_LOAD = 3,
    I_IMMEDIATE_1 = 19,
    I_IMMEDIATE_2 = 27,
    I_JUMP = 103,

    U_ADD = 23,
    U_LOAD = 55,

    S = 35,

    R_1 = 51,
    R_2 = 59,

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
    byte imm_1 : 5;
    byte funct3 : 3;
    byte rs1 : 5;
    byte rs2 : 5;
    byte imm_2 : 7;
} SB_TYPE;
typedef struct
{
    byte opcode : 8;
    byte rd : 5;
    word imm : 19;
} UJ_TYPE;

#endif