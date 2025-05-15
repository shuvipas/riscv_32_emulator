#include <stdio.h>
#include "cpu.h"




void print_registers(CPU *cpu) {
    const char* abi[] = {
        "zero", "ra",  "sp",  "gp",
        "tp",   "t0",  "t1",  "t2",
        "s0",   "s1",  "a0",  "a1",
        "a2",   "a3",  "a4",  "a5",
        "a6",   "a7",  "s2",  "s3",
        "s4",   "s5",  "s6",  "s7",
        "s8",   "s9",  "s10", "s11",
        "t3",   "t4",  "t5",  "t6",
    };

    printf("   PC: %#-11x\n", cpu->pc);
    for (int i = 0; i < 8; i++) {
        printf("x%02d (%4s): %#-11x  ", i,      abi[i],     cpu->reg[i]);
        printf("x%02d (%4s): %#-11x  ", i+8,    abi[i+8],   cpu->reg[i+8]);
        printf("x%02d (%4s): %#-11x  ", i+16,   abi[i+16],  cpu->reg[i+16]);
        printf("x%02d (%4s): %#-11x\n", i+24,   abi[i+24],  cpu->reg[i+24]);
    }
    printf("   PC (pc): %#-11x\n", cpu->pc);
}
