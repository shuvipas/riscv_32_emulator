#include <stdio.h>
#include "cpu.h"
#include "dram.h"
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
void execute_r_type(CPU *cpu, DRAM *ram, R_TYPE *ins_format)
{
    word* rd_ptr  = &cpu->reg[ins_format->rd];
    
    byte funct3 = ins_format->funct3;
    word rs1 = cpu->reg[ins_format->rs1];
    word rs2 =cpu->reg[ins_format->rs2];
    byte funct7 =ins_format->funct7;


    if (funct7 == 0)
    {
        switch (funct3)
        {
        case 0: //add
            *rd_ptr = rs1 + rs2; 
            break;
        case 1:
            /* code */
            break;
        case 2:
            /* code */
            break;
        case 3:
            /* code */
            break;
        case 4:
            /* code */
            break;
        case 5:
            /* code */
            break;
        case 6:
            /* code */
            break;
        case 7:
            /* code */
            break;

        default:
            break;
        }
    }
    else if (ins_format->funct7 == 0b0100000)
    {
        switch (funct3)
        {
        case 0: //sub
            *rd_ptr = rs1 - rs2;  
            //todo check for negitive 
            break;
        case 1:
            /* code */
            break;
        case 2:
            /* code */
            break;
        case 3:
            /* code */
            break;
        case 4:
            /* code */
            break;
        case 5:
            /* code */
            break;
        case 6:
            /* code */
            break;
        case 7:
            /* code */
            break;

        default:
            break;
        }
    }
}
void execute_i_type(CPU *cpu, DRAM *ram, word ins) {}
void execute_s_type(CPU *cpu, DRAM *ram, word ins) {}
void execute_b_type(CPU *cpu, DRAM *ram, word ins) {}
void execute_u_type(CPU *cpu, DRAM *ram, word ins) {}
void execute_j_type(CPU *cpu, DRAM *ram, word ins) {}

void execute_instruction(CPU *cpu, DRAM *ram, word ins, INS_TYPE type)
{
    switch (type)
    {
    case I_LOAD:
    case I_IMMEDIATE_1:
    case I_IMMEDIATE_2:
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
    case R_1:
    case R_2:
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