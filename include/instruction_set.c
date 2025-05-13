#include "instruction_set.h"


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
