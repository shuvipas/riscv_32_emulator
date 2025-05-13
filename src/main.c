#include <stdio.h>
#include "cpu.h"
#include "dram.h"
#include "instruction_set.h"
#define BYTE 1
#define WORD 4

word fetch_instruction(CPU *cpu, DRAM *ram)
{
    word ins;
    dram_load(ram, &cpu->pc, &ins, sizeof(ins));
    cpu->pc += WORD;
    return ins;
}

void initialize_cpu(CPU *cpu)
{
    cpu->reg[0] = 0x0;                   // x0 hardwired to 0;
    cpu->reg[2] = DRAM_BASE + DRAM_SIZE; // stack pointer
    cpu->pc = DRAM_BASE;
}
INS_TYPE decode_instruction(word ins)
{
    word mask = 0x7f; // firs 7 bits hi
    ins = ins & mask;
    return (INS_TYPE)ins;
}


void execute_instruction(CPU *cpu, DRAM *ram, word ins, INS_TYPE type)
{
    switch (type)
    {
    case I_LOAD:
    case I_IMMEDIATE:
    case I_JUMP:
        execute_i_type(cpu, ram, &ins);
        break;
    case U_ADD:
    case U_LOAD:
        execute_u_type(cpu, ram, ins);
        break;
    case S:
        execute_s_type(cpu, ram, ins);
        break;
    case B:
        execute_b_type(cpu, ram, ins);
        break;
    case R:
        execute_r_type(cpu, ram, ins);
        break;
    case J:
        execute_j_type(cpu, ram, ins);
        break;
    default:
        printf("ERROR: execute_instruction bad opcode: %d\n", type);
        break;
    }
}
int main()
{
    CPU cpu = {0};
    DRAM ram = {0};
    initialize_cpu(&cpu);
    word ins = 0b0110011;
    printf("%x\n", decode_instruction(ins));
}