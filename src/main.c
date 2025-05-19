#include <stdio.h>
#include "cpu.h"
#include "dram.h"
#include "instruction_set.h"
#define BYTE 1
#define WORD_LEN 4

word fetch_instruction(CPU *cpu, DRAM *ram)
{
    word ins;
    dram_load(ram, cpu->pc, &ins, sizeof(ins));
    cpu->pc += WORD_LEN;
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

int execute_instruction(CPU *cpu, DRAM *ram, word ins, INS_TYPE type)
{
    switch (type)
    {
    case I_LOAD:
    case I_IMMEDIATE:
    case I_JUMP:
        execute_i_type(cpu, &ins);
        break;
    case U_ADD:
    case U_LOAD:
        execute_u_type(cpu, ins);
        break;
    case S:
        execute_s_type(cpu, ram, &ins);
        break;
    case B:
        execute_b_type(cpu,ins);
        break;
    case R:
        execute_r_type(cpu, &ins);
        break;
    case J:
        execute_j_type(cpu, ins);
        break;
    default:
        printf("ERROR: execute_instruction opcode = %d is not sopported\n", type);
        return 1;
        break;
    }
    return 0;
}
void read_file(DRAM *ram, char *filename)
{
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: missing binary <filename>\n");
        return 1;
    }
    CPU cpu = {0};
    DRAM ram = {0};
    initialize_cpu(&cpu);
    read_file(&ram, argv[1]);
    int cpu_stop = 0;

    while (!cpu_stop)
    {
        word ins = fetch_instruction(&cpu, &ram);
        INS_TYPE ins_type = decode_instruction(ins);
        cpu_stop = execute_instruction(&cpu, &ram, ins, ins_type);
        print_registers(&cpu);
    }
    word ins = 0b0110011;
    printf("%x\n", decode_instruction(ins));
}