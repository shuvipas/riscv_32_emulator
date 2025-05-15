#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "cpu.h"
#include "dram.h"
#include "instruction_set.h"

// gcc -o instruction_set_test instruction_set_test.c cpu.c instruction_set.c dram.c -Wall
// ./instruction_set_test
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
    execute_r_type(&cpu, NULL, &ins);
    assert(cpu.reg[ins.rd] == 30);
    printf("add passed\n");
}

void test_sub()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 50, 30, 0b000, 0b0100000);
    execute_r_type(&cpu, NULL, &ins);
    assert(cpu.reg[ins.rd] == 20);
    printf("sub passed\n");
}

void test_sll()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 4, 2, 0b001, 0b0000000);
    execute_r_type(&cpu, NULL, &ins);
    assert(cpu.reg[ins.rd] == (4 << 2));
    printf("sll passed\n");
}

void test_slt()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, -1, 2, 0b010, 0b0000000);
    execute_r_type(&cpu, NULL, &ins);
    assert(cpu.reg[ins.rd] == 1);
    printf("slt passed\n");
}

void test_sltu()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 5, 10, 0b011, 0b0000000);
    execute_r_type(&cpu, NULL, &ins);
    assert(cpu.reg[ins.rd] == 1);
    printf("sltu passed\n");
}

void test_xor()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 0b1100, 0b1010, 0b100, 0b0000000);
    execute_r_type(&cpu, NULL, &ins);
    assert(cpu.reg[ins.rd] == (0b1100 ^ 0b1010));
    printf("xor passed\n");
}

void test_srl()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 0b100000, 3, 0b101, 0b0000000);
    execute_r_type(&cpu, NULL, &ins);
    assert(cpu.reg[ins.rd] == (0b100000 >> 3));
    printf("srl passed\n");
}

void test_sra()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, (int32_t)0xFFFFFFF0, 2, 0b101, 0b0100000);
    execute_r_type(&cpu, NULL, &ins);
    assert((int32_t)cpu.reg[ins.rd] == ((int32_t)0xFFFFFFF0 >> 2));
    printf("sra passed\n");
}

void test_or()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 0b0101, 0b1100, 0b110, 0b0000000);
    execute_r_type(&cpu, NULL, &ins);
    assert(cpu.reg[ins.rd] == (0b0101 | 0b1100));
    printf("or passed\n");
}

void test_and()
{
    CPU cpu = {0};
    R_TYPE ins = {0};
    setup_r_type_test(&cpu, &ins, 3, 1, 2, 0b0111, 0b1010, 0b111, 0b0000000);
    execute_r_type(&cpu, NULL, &ins);
    assert(cpu.reg[ins.rd] == (0b0111 & 0b1010));
    printf("and passed\n");
}


void execute_r_test()
{
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
    printf("All R-type instruction tests passed.\n");
}
int main()
{
    execute_r_test();
    return 0;
}
