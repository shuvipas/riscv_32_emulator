#include "cpu.h"
#include "dram.h"
#define BYTE 1
#define WORD 4

typedef uint8_t byte;
typedef uint32_t word;

word fetch_instruction(CPU* cpu,DRAM* ram, uint32_t* pc){
    word ins;
    dram_load(ram,*pc,&ins, sizeof(ins));
    *pc += WORD;
    return ins;
}

void initialize_cpu(CPU* cpu){
    cpu->reg[0] = 0x0; //x0 hardwired to 0;
    cpu->reg[2] = DRAM_BASE + DRAM_SIZE; //stack pointer
    cpu->pc = DRAM_BASE;
}

int main(){
    CPU cpu = {0};
    DRAM ram = {0};
    initialize_cpu(&cpu);

}