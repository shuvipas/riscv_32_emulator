#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef uint8_t byte;
typedef uint16_t half_word;
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

void print_registers(CPU *cpu);
#endif