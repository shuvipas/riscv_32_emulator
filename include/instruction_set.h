#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H
#include "cpu.h"
#include "dram.h"

void execute_r_type(CPU *cpu, DRAM *ram, R_TYPE *ins_format);


#endif