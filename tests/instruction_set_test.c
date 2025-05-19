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
 *                            B-TYPE INSTRUCTION TESTS
 * =============================================================================
 *
 * Tests for:
 *   - beq   (branch if equal)
 *   - bne   (branch if not equal)
 *   - blt   (branch if less than, signed)
 *   - bge   (branch if greater than or equal, signed)
 *   - bltu  (branch if less than, unsigned)
 *   - bgeu  (branch if greater than or equal, unsigned)
 *
 * Each test sets up a CPU instance and a B-type instruction with an immediate
 * offset. The program counter (PC) is checked after execution to verify
 * whether the branch was taken correctly.
 */

 void setup_b_type_test(CPU *cpu, B_TYPE *ins, int rs1, int rs2, word val1, word val2,
    int imm, uint32_t pc, uint8_t funct3) {
cpu->pc = pc;
cpu->reg[rs1] = val1;
cpu->reg[rs2] = val2;

ins->funct3 = funct3;
ins->rs1 = rs1;
ins->rs2 = rs2;

// Encode the immediate into its B-type split parts
ins->imm_1 = (imm >> 1) & 0x1F;
ins->imm_2 = (imm >> 5) & 0x3F;
ins->imm_3 = (imm >> 11) & 0x1;
ins->imm_4 = (imm >> 12) & 0x1;
}

void test_beq_taken() {
CPU cpu = {0};
B_TYPE ins = {0};
// rs1 == rs2, branch should be taken (PC = PC + imm - 4)
setup_b_type_test(&cpu, &ins, 1, 2, 0x10, 0x10, 8, 0x1000, 0);
execute_b_type(&cpu, (word *)&ins);
assert(cpu.pc == 0x1004);  // 0x1000 + 8 - 4
}

void test_bne_taken() {
CPU cpu = {0};
B_TYPE ins = {0};
// rs1 != rs2, branch should be taken
setup_b_type_test(&cpu, &ins, 1, 2, 0x1, 0x2, 12, 0x2000, 1);
execute_b_type(&cpu, (word *)&ins);
assert(cpu.pc == 0x2008);  // 0x2000 + 12 - 4
}

void test_blt_signed_taken() {
CPU cpu = {0};
B_TYPE ins = {0};
// rs1 < rs2 (signed), branch taken
setup_b_type_test(&cpu, &ins, 3, 4, -10, 5, 4, 0x3000, 4);
execute_b_type(&cpu, (word *)&ins);
assert(cpu.pc == 0x3000 + 4 - 4);  // 0x3000
}

void test_bge_signed_taken() {
CPU cpu = {0};
B_TYPE ins = {0};
// rs1 >= rs2 (signed), branch taken
setup_b_type_test(&cpu, &ins, 1, 2, 20, -10, 16, 0x4000, 5);
execute_b_type(&cpu, (word *)&ins);
assert(cpu.pc == 0x4010);  // 0x4000 + 16 - 4
}

void test_bltu_taken() {
CPU cpu = {0};
B_TYPE ins = {0};
// rs1 < rs2 (unsigned), branch taken
setup_b_type_test(&cpu, &ins, 5, 6, 0x00000001, 0xFFFFFFFE, 6, 0x5000, 6);
execute_b_type(&cpu, (word *)&ins);
assert(cpu.pc == 0x5002);  // 0x5000 + 6 - 4
}

void test_bgeu_taken() {
CPU cpu = {0};
B_TYPE ins = {0};
// rs1 >= rs2 (unsigned), branch taken
setup_b_type_test(&cpu, &ins, 7, 8, 0xFFFFFFFF, 0x0, 10, 0x6000, 7);
execute_b_type(&cpu, (word *)&ins);
assert(cpu.pc == 0x6006);  // 0x6000 + 10 - 4
}

void execute_b_test() {
printf("execute_b_test\n");
test_beq_taken();
test_bne_taken();
test_blt_signed_taken();
test_bge_signed_taken();
test_bltu_taken();
test_bgeu_taken();
printf("All B-type instruction tests passed.\n\n");
}

/* =============================================================================
 *                            U-TYPE INSTRUCTION TESTS
 * =============================================================================
 *
 * Tests for:
 *   - lui   (load upper immediate)
 *   - auipc (add upper immediate to PC)
 *
 * Each test sets up a CPU instance, encodes a U-type instruction,
 * executes it, and verifies that the result is correctly written
 * to the destination register.
 */

 void setup_u_type_test(CPU *cpu, U_TYPE *ins, int rd, uint32_t imm, uint32_t pc, uint8_t opcode) {
    cpu->pc = pc;
    ins->rd = rd;
    ins->imm = imm;
    ins->opcode = opcode;
}

void test_lui() {
    CPU cpu = {0};
    U_TYPE ins = {0};

    // Immediate = 0xABCD, should result in rd = 0xABCD000
    setup_u_type_test(&cpu, &ins, 5, 0xABCD, 0, U_LOAD);
    execute_u_type(&cpu, (word *)&ins);

    assert(cpu.reg[5] == 0xABCD000);
    //printf("lui passed\n");
}

void test_auipc() {
    CPU cpu = {0};
    U_TYPE ins = {0};

    // Immediate = 0x1000, PC = 0x200, expected rd = (0x1000 << 12) + 0x200 = 0x1000000 + 0x200 = 0x1000200
    setup_u_type_test(&cpu, &ins, 7, 0x1000, 0x200, U_ADD);
    execute_u_type(&cpu, (word *)&ins);

    assert(cpu.reg[7] == 0x1000200);
    //printf("auipc passed\n");
}

void execute_u_test() {
    printf("execute_u_test\n");
    test_lui();
    test_auipc();
    printf("All U-type instruction tests passed.\n\n");
}

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
void setup_s_type_test(CPU *cpu, DRAM *ram, S_TYPE *ins, int rs1, int rs2,
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
S_TYPE ins = {0};

setup_s_type_test(&cpu, &ram, &ins, 1, 2, DRAM_BASE + 0x100, 0xAA, 0x4, 0b000);
execute_s_type(&cpu, &ram, (word *)&ins);

assert(ram.mem[0x104] == 0xAA);
//printf("sb passed\n");
}

void test_sh() {
CPU cpu = {0};
DRAM ram = {0};
S_TYPE ins = {0};

setup_s_type_test(&cpu, &ram, &ins, 1, 2, DRAM_BASE + 0x100, 0xBEEF, 0x6, 0b001);
execute_s_type(&cpu, &ram, (word *)&ins);

uint16_t result;
memcpy(&result, &ram.mem[0x106], sizeof(uint16_t));
assert(result == 0xBEEF);
//printf("sh passed\n");
}

void test_sw() {
CPU cpu = {0};
DRAM ram = {0};
S_TYPE ins = {0};

setup_s_type_test(&cpu, &ram, &ins, 1, 2,DRAM_BASE + 0x100, 0xDEADBEEF, 0x8, 0b010);
execute_s_type(&cpu, &ram, (word *)&ins);

uint32_t result;
memcpy(&result, &ram.mem[0x108], sizeof(uint32_t));
assert(result == 0xDEADBEEF);
//printf("sw passed\n");
}

void execute_s_test() {
printf("execute_s_test\n");
test_sb();
test_sh();
test_sw();
printf("All S-type instruction tests passed.\n\n");
}



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
    //printf("add passed\n");
}

void test_sub()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 50, 30, 0b000, 0b0100000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == 20);
    //printf("sub passed\n");
}

void test_sll()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 4, 2, 0b001, 0b0000000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == (4 << 2));
    //printf("sll passed\n");
}

void test_slt()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, -1, 2, 0b010, 0b0000000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == 1);
    //printf("slt passed\n");
}

void test_sltu()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 5, 10, 0b011, 0b0000000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == 1);
    //printf("sltu passed\n");
}

void test_xor()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 0b1100, 0b1010, 0b100, 0b0000000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == (0b1100 ^ 0b1010));
    //printf("xor passed\n");
}

void test_srl()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 0b100000, 3, 0b101, 0b0000000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == (0b100000 >> 3));
    //printf("srl passed\n");
}

void test_sra()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, (int32_t)0xFFFFFFF0, 2, 0b101, 0b0100000);
    execute_r_type(&cpu, (word*)&ins);
    assert((int32_t)cpu.reg[ins.rd] == ((int32_t)0xFFFFFFF0 >> 2));
    //printf("sra passed\n");
}

void test_or()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 0b0101, 0b1100, 0b110, 0b0000000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == (0b0101 | 0b1100));
    //printf("or passed\n");
}

void test_and()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 0b0111, 0b1010, 0b111, 0b0000000);
    execute_r_type(&cpu, (word*)&ins);
    assert(cpu.reg[ins.rd] == (0b0111 & 0b1010));
    //printf("and passed\n");
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
    execute_u_test();
    execute_b_test();
    return 0;
}
