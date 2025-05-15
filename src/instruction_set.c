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
        case 1: // sll-shift left logical
        *rd_ptr = rs1 << rs2;
            break;
        case 2: //slt- set less than
            /* code */
            break;
        case 3: //sltu - set less than unsigned
            /* code */
            break;
        case 4: // xor
            *rd_ptr =rs1 ^ rs2;
            break;
        case 5: //srl -shift right logical
            /* code */
            break;
        case 6: //or
            *rd_ptr = rs1 | rs2;
            break;
        case 7: //and
            *rd_ptr = rs1 &rs2;
            break;

        default:
        printf("ERROR: execute_r_type funct3 = %d is not sopported\n", funct3);
            break;
        }
    }
    else if (ins_format->funct7 == 0b0100000)
    {
        switch (funct3)
        {
        case 0: //sub 
            //todo check for negitive 
            break;
        case 5: //sra - shift right arithmetic
            /* code */
            break;
        default:
        printf("ERROR: execute_r_type funct3 = %d is not sopported\n", funct3);
            break;
        }
    }
    else{
        printf("ERROR: execute_r_type funct7 = %d is not sopported\n", funct7);
    }
}

void execute_i_type(CPU *cpu, DRAM *ram, word ins) {}
void execute_s_type(CPU *cpu, DRAM *ram, word ins) {}
void execute_b_type(CPU *cpu, DRAM *ram, word ins) {}
void execute_u_type(CPU *cpu, DRAM *ram, word ins) {}
void execute_j_type(CPU *cpu, DRAM *ram, word ins) {}
