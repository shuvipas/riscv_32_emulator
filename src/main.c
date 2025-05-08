#include "cpu.h"
#include "dram.h"

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