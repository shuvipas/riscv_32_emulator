#include <stdio.h>
#include "instruction_set.h"


void execute_r_type(CPU *cpu, word *ins)
{
    R_TYPE *ins_format = (R_TYPE*) ins;
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
            *rd_ptr = (int32_t)rs1 + (int32_t)rs2; 
            break;
        case 1: // sll-shift left logical
            *rd_ptr = rs1 << rs2;
            break;
        case 2: //slt- set less than
            *rd_ptr = ((int32_t)rs1 < (int32_t)rs2)?1:0;
            break;
        case 3: //sltu - set less than unsigned
            *rd_ptr = (rs1 < rs2)?1:0;
            break;
        case 4: // xor
            *rd_ptr =rs1 ^ rs2;
            break;
        case 5: //srl -shift right logical
            *rd_ptr = rs1 >> rs2;
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
            *rd_ptr = (int32_t)rs1 - (int32_t)rs2;
            break;
        case 5: //sra - shift right arithmetic
        if ((int32_t)rs1 >= 0) {
            *rd_ptr = rs1 >> rs2;
        } else {
            *rd_ptr = (rs1 >> rs2) | ~(~0U >> rs2);
        }
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

// void execute_i_type(CPU *cpu, DRAM *ram, word ins) {}
// void execute_s_type(CPU *cpu, DRAM *ram, word ins) {}
// void execute_b_type(CPU *cpu, DRAM *ram, word ins) {}
// void execute_u_type(CPU *cpu, DRAM *ram, word ins) {}
// void execute_j_type(CPU *cpu, DRAM *ram, word ins) {}
