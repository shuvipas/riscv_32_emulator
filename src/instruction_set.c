#include <stdio.h>
#include "instruction_set.h"
#include "dram.h"
word sign_extender(word num,  int sign_bit){
    if((num>>sign_bit) & 1){
        unsigned int mask = (1<<(sign_bit +1)) -1;
        mask = ~mask;
        return num|mask;
    }
    return num;
}

/* =============================================================================
 *                            I-TYPE INSTRUCTION 
 * =============================================================================
 */
void execute_i_load(byte funct3,word imm,word *rd_ptr,word rs1){
    word offset;
    switch (funct3)
    {
    case 0: // lb - load byte (rd =SignExt([Address]7:0))
        offset = sign_extender(imm,7);
        *rd_ptr = rs1 + offset;

        break;
    case 1: // lh -load half word
        offset = sign_extender(imm,11);

        *rd_ptr = rs1 + offset;
        break;
    case 2: // lw
        *rd_ptr = rs1 + imm;
        break;
    case 4: // lbu -load byte unsigned
        *rd_ptr = rs1 + (imm >> 4);
        break;
    case 5: // lhu -load half unsigned
        *rd_ptr = rs1 + imm;
        break;
        default:
        printf("ERROR: execute_i_type funct3 = %d is not sopported\n", funct3);
        break;
    }
}
void execute_i_immediate(byte funct3,byte funct7,word imm,word *rd_ptr,word rs1){
    word offset;
    word num;
    switch (funct3)
        {
        case 0: // addi - add immediate
            
            break;
        case 1: // lh -load half word
            if (funct7 == 0)// slli - shift left logical immediate            
            {
                *rd_ptr = rs1 << imm;
            }

            break;
        case 2: // slti-set less than immediate
            offset = sign_extender(imm, 11);    
            *rd_ptr = (rs1 < offset)? 1:0;
            break;
        case 3: // sltu - set less than unsigned
        *rd_ptr = (rs1 < imm)? 1:0;
            break;
        case 4: // xori 
             num = sign_extender(imm, 11);    
            *rd_ptr = rs1^num;
            break;
        case 5: // lhu -load half unsigned
            if (funct7 == 0) //srli -shift right logical immediate
            {
                *rd_ptr = rs1>>imm;
            }
            else if (funct7 == 0x20) //srai-shift right arithmetic imm.
            {
                *rd_ptr = ((int32_t)rs1)>>imm;
            }

            break;
        case 6: // ori
        num = sign_extender(imm, 11);    
        *rd_ptr = rs1|num;

            break;
        case 7: // andi
        num = sign_extender(imm, 11);    
        *rd_ptr = rs1&num;

            break;

        default:
            printf("ERROR: execute_i_type funct3 = %d is not sopported\n", funct3);
            break;
        }
}
void execute_i_type(CPU *cpu, word *ins)
{
    I_TYPE *ins_format = (I_TYPE *)ins;
    word *rd_ptr = &cpu->reg[ins_format->rd];

    byte funct3 = ins_format->funct3;
    word rs1 = cpu->reg[ins_format->rs1];
    word imm = ins_format->imm;

    byte funct7 = imm >> 5;
    switch (ins_format->opcode)
    {
        case (I_LOAD):
        execute_i_load( funct3,imm,rd_ptr, rs1);
        break;
        case (I_IMMEDIATE):
        execute_i_immediate(funct3,funct7,imm, rd_ptr, rs1);
            break;
        case (I_JUMP): //jalr - jump and link register
            *rd_ptr = cpu->pc +4;
            cpu->pc = imm + cpu->pc -4;
            break;
        default:
            printf("ERROR: execute_i_type opcode = %d is not sopported\n", ins_format->opcode);
            break;
        }
        
    }
    /* =============================================================================
 *                            S-TYPE INSTRUCTION 
 * =============================================================================
 */
    void execute_s_type(CPU * cpu, DRAM * dram, word * ins)
    {
        S_TYPE *ins_format = (S_TYPE *)ins;
        word *rs2_ptr = &cpu->reg[ins_format->rs2];

        byte funct3 = ins_format->funct3;
        word rs1 = cpu->reg[ins_format->rs1];

        word imm0 = ins_format->imm_0;
        word imm1 = ins_format->imm_1;
        word imm = imm1 << 5 | imm0;
        word addres = imm + rs1;
        switch (funct3)
        {
        case 0: // sb - store byte
            dram_store(dram, addres, rs2_ptr, sizeof(byte));
            break;
        case 1: // sh - store half
            dram_store(dram, addres, rs2_ptr, sizeof(half_word));
            break;
        case 2: // sw - store word
            dram_store(dram, addres, rs2_ptr, sizeof(word));
            break;
        default:
            printf("ERROR: execute_s_type funct3 = %d is not sopported\n", funct3);
            break;
        }
    }
    /* =============================================================================
 *                            R-TYPE INSTRUCTION 
 * =============================================================================
 */
    void execute_r_type(CPU * cpu, word * ins)
    {
        R_TYPE *ins_format = (R_TYPE *)ins;
        word *rd_ptr = &cpu->reg[ins_format->rd];

        byte funct3 = ins_format->funct3;
        word rs1 = cpu->reg[ins_format->rs1];
        word rs2 = cpu->reg[ins_format->rs2];
        byte funct7 = ins_format->funct7;

        if (funct7 == 0)
        {
            switch (funct3)
            {
            case 0: // add
                *rd_ptr = (int32_t)rs1 + (int32_t)rs2;
                break;
            case 1: // sll-shift left logical
                *rd_ptr = rs1 << rs2;
                break;
            case 2: // slt- set less than
                *rd_ptr = ((int32_t)rs1 < (int32_t)rs2) ? 1 : 0;
                break;
            case 3: // sltu - set less than unsigned
                *rd_ptr = (rs1 < rs2) ? 1 : 0;
                break;
            case 4: // xor
                *rd_ptr = rs1 ^ rs2;
                break;
            case 5: // srl -shift right logical
                *rd_ptr = rs1 >> rs2;
                break;
            case 6: // or
                *rd_ptr = rs1 | rs2;
                break;
            case 7: // and
                *rd_ptr = rs1 & rs2;
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
            case 0: // sub
                // todo check for negitive
                *rd_ptr = (int32_t)rs1 - (int32_t)rs2;
                break;
            case 5: // sra - shift right arithmetic
                if ((int32_t)rs1 >= 0)
                {
                    *rd_ptr = rs1 >> rs2;
                }
                else
                {
                    *rd_ptr = (rs1 >> rs2) | ~(~0U >> rs2);
                }
                break;
            default:
                printf("ERROR: execute_r_type funct3 = %d is not sopported\n", funct3);
                break;
            }
        }
        else
        {
            printf("ERROR: execute_r_type funct7 = %d is not sopported\n", funct7);
        }
    }

   
/* =============================================================================
 *                            U-TYPE INSTRUCTION 
 * =============================================================================
 */
    void execute_u_type(CPU * cpu, word *ins)
    {
        U_TYPE *ins_format = (U_TYPE *)ins;
        word *rd_ptr = &cpu->reg[ins_format->rd];
        word imm = ins_format->imm;
        switch (ins_format->opcode)
        {
        case U_ADD: // auipc - add upper immediate to PC (rd = {upimm, 12'b0} + PC)

            *rd_ptr = (imm << 12) + cpu->pc;
            break;
        case U_LOAD: // lui-load upper immediate (rd = {upimm, 12’b0})
            *rd_ptr = imm << 12;
            break;
        default:
            printf("ERROR: execute_u_type opcode = %d is not sopported\n", ins_format->opcode);
            break;
        }
    }
    /* =============================================================================
 *                            J-TYPE INSTRUCTION 
 * =============================================================================
 */
    void execute_j_type(CPU * cpu, word *ins)
    {
        J_TYPE *ins_format = (J_TYPE *)ins;
        word *rd_ptr = &cpu->reg[ins_format->rd];

        word imm1 = ins_format->imm_1 << 1;
        word imm2 = ins_format->imm_2 << 11;
        word imm3 = ins_format->imm_3 << 12;
        word imm4 = ins_format->imm_4 << 20;

        word imm = imm1 | imm2 | imm3 | imm4;

        switch (ins_format->opcode)
        {
        case J: // jal -jump and link (PC = JTA, rd = PC + 4)
            *rd_ptr = cpu->pc + 4;
            cpu->pc = cpu->pc + imm - 4;
            break;
        default:
            printf("ERROR: execute_j_type opcode = %d is not sopported\n", ins_format->opcode);
            break;
        }
    }
    /* =============================================================================
 *                            B-TYPE INSTRUCTION 
 * =============================================================================
 */
    void execute_b_type(CPU * cpu, word *ins)
    {
        B_TYPE *ins_format = (B_TYPE *)ins;

        byte funct3 = ins_format->funct3;
        word rs1 = cpu->reg[ins_format->rs1];
        word rs2 = cpu->reg[ins_format->rs2];

        word imm1 = ins_format->imm_1 << 1;
        word imm2 = ins_format->imm_2 << 5;
        word imm3 = ins_format->imm_3 << 11;
        word imm4 = ins_format->imm_4 << 12;

        word imm = (imm1 | imm2 | imm3 | imm4);
        word addres = imm + cpu->pc - 4;
        switch (funct3)

        {
        case 0: // beq - branch if = if (rs1 == rs2) PC = BTA
            if (rs1 == rs2)
                cpu->pc = addres;
            break;
        case 1: // bne - branch if !=
            if (rs1 != rs2)
                cpu->pc = addres;
            break;
        case 4: // blt - branch if <
            if ((int32_t)rs1 < (int32_t)rs2)
                cpu->pc = addres;
            break;
        case 5: // bge - branch if >=
            if ((int32_t)rs1 >= (int32_t)rs2)
                cpu->pc = addres;
            break;
        case 6: // bltu - branch if < unsigned
            if (rs1 < rs2)
                cpu->pc = addres;
            break;
        case 7: // bgeu - branch if >= unsigned
            if (rs1 >= rs2)
                cpu->pc = addres;
            break;
        default:
            printf("ERROR: execute_b_type funct3 = %d is not sopported\n", funct3);
            break;
        }
    }
