#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H
#include "cpu.h"
#include "dram.h"

void execute_r_type(CPU *cpu, word *ins);
void execute_s_type(CPU *cpu, DRAM *dram, word *ins);

#endif