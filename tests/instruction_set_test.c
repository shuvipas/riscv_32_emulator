/**
 * instruction_set_test.c
 *
 * Unit tests for RISC-V R-type and S-type instruction execution functions.
 * 
 * Compilation:
 *   make instruction_set_test 
 *   or make test torun all tests
 * 
 * Usage:
 *   ./instruction_set_test
 */
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "cpu.h"
#include "dram.h"
#include "instruction_set.h"



/* =============================================================================
 *                            S-TYPE INSTRUCTION TESTS
 * =============================================================================
 * 
 * Tests for:
 *   - sb (store byte)
 *   - sh (store halfword)
 *   - sw (store word)
 * 
 * Each test sets up a CPU and DRAM instance, encodes a store instruction,
 * executes it, and checks that the memory was updated correctly.
 */
void setup_s_type_test(CPU *cpu, DRAM *ram, SB_TYPE *ins, int rs1, int rs2,
    uint32_t base_addr, uint32_t val, uint32_t imm,
    uint8_t funct3) {
memset(ram->mem, 0, DRAM_SIZE); // Clear memory
cpu->reg[rs1] = base_addr;
cpu->reg[rs2] = val;

ins->rs1 = rs1;
ins->rs2 = rs2;
ins->funct3 = funct3;
ins->imm_0 = imm & 0x1F;
ins->imm_1 = (imm >> 5) & 0x7F;
}

void test_sb() {
CPU cpu = {0};
DRAM ram = {0};
SB_TYPE ins = {0};

setup_s_type_test(&cpu, &ram, &ins, 1, 2, 0x100, 0xAA, 0x4, 0b000);
execute_s_type(&cpu, &ram, (word *)&ins);

assert(ram.mem[0x104] == 0xAA);
printf("sb passed\n");
}

void test_sh() {
CPU cpu = {0};
DRAM ram = {0};
SB_TYPE ins = {0};

setup_s_type_test(&cpu, &ram, &ins, 1, 2, 0x100, 0xBEEF, 0x6, 0b001);
execute_s_type(&cpu, &ram, (word *)&ins);

uint16_t result;
memcpy(&result, &ram.mem[0x106], sizeof(uint16_t));
assert(result == 0xBEEF);
printf("sh passed\n");
}

void test_sw() {
CPU cpu = {0};
DRAM ram = {0};
SB_TYPE ins = {0};

setup_s_type_test(&cpu, &ram, &ins, 1, 2, 0x100, 0xDEADBEEF, 0x8, 0b010);
execute_s_type(&cpu, &ram, (word *)&ins);

uint32_t result;
memcpy(&result, &ram.mem[0x108], sizeof(uint32_t));
assert(result == 0xDEADBEEF);
printf("sw passed\n");
}

void execute_s_test() {
    printf("execute_s_test\n");
test_sb();
test_sh();
test_sw();
printf("All S-type instruction tests passed.\n\n");
}

// void test_sh() {
//     CPU cpu = {0};
//     DRAM ram = {0};
//     SB_TYPE ins = {0};

//     setup_s_type_test(&cpu, &ram, &ins, 1, 2, 0x100, 0xBEEF, 0x6, 0b001);
//     execute_s_type(&cpu, &ram, (word *)&ins);

//     uint16_t result;
//     memcpy(&result, &ram.mem[0x106], sizeof(uint16_t));
//     assert(result == 0xBEEF);
//     printf("sh passed\n");
// }

// void test_sw() {
//     CPU cpu = {0};
//     DRAM ram = {0};
//     SB_TYPE ins = {0};

//     setup_s_type_test(&cpu, &ram, &ins, 1, 2, 0x100, 0xDEADBEEF, 0x8, 0b010);
//     execute_s_type(&cpu, &ram, (word *)&ins);

//     uint32_t result;
//     memcpy(&result, &ram.mem[0x108], sizeof(uint32_t));
//     assert(result == 0xDEADBEEF);
//     printf("sw passed\n");
// }

// void execute_s_test() {
//     test_sb();
//     test_sh();
//     test_sw();
//     printf("All S-type instruction tests passed.\n\n");
// }

/* =============================================================================
 *                            R-TYPE INSTRUCTION TESTS
 * =============================================================================
 * 
 * Tests for:
 *   - add, sub, sll, slt, sltu
 *   - xor, srl, sra, or, and
 * 
 * Each test sets up registers, encodes the instruction, runs it,
 * and asserts the correct result was written to the destination register.
 */



void setup_r_type_test(CPU *cpu, R_TYPE *ins, int rd, int rs1, int rs2,
    int32_t val1, int32_t val2, uint8_t funct3, uint8_t funct7)
{
   cpu->reg[rs1] = val1;
   cpu->reg[rs2] = val2;
   ins->rd = rd;
   ins->rs1 = rs1;
   ins->rs2 = rs2;
   ins->funct3 = funct3;
   ins->funct7 = funct7;
}

void test_add()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 10, 20, 0b000, 0b0000000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == 30);
    printf("add passed\n");
}

void test_sub()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 50, 30, 0b000, 0b0100000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == 20);
    printf("sub passed\n");
}

void test_sll()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 4, 2, 0b001, 0b0000000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == (4 << 2));
    printf("sll passed\n");
}

void test_slt()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, -1, 2, 0b010, 0b0000000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == 1);
    printf("slt passed\n");
}

void test_sltu()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 5, 10, 0b011, 0b0000000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == 1);
    printf("sltu passed\n");
}

void test_xor()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 0b1100, 0b1010, 0b100, 0b0000000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == (0b1100 ^ 0b1010));
    printf("xor passed\n");
}

void test_srl()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 0b100000, 3, 0b101, 0b0000000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == (0b100000 >> 3));
    printf("srl passed\n");
}

void test_sra()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, (int32_t)0xFFFFFFF0, 2, 0b101, 0b0100000);
    execute_r_type(&cpu, (word*)&ins);
    assert((int32_t)cpu.reg[ins.rd] == ((int32_t)0xFFFFFFF0 >> 2));
    printf("sra passed\n");
}

void test_or()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 0b0101, 0b1100, 0b110, 0b0000000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == (0b0101 | 0b1100));
    printf("or passed\n");
}

void test_and()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 0b0111, 0b1010, 0b111, 0b0000000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == (0b0111 & 0b1010));
    printf("and passed\n");
}


void execute_r_test()
{
    printf("execute_r_test\n");
    test_add();
    test_sub();
    test_sll();
    test_slt();
    test_sltu();
    test_xor();
    test_srl();
    test_sra();
    test_or();
    test_and();
    printf("All R-type instruction tests passed.\n\n");
}



int main()
{
    execute_r_test();
    execute_s_test();
    return 0;
}
