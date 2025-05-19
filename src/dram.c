#include <stdlib.h>
#include <stdio.h>
#include "dram.h"

void dram_load(DRAM* dram, uint32_t address, void* dest, unsigned int size){
    // this will copy a byte/ half word(2 byts)/ word(4 byts) / double word(8 byts)
    if (address < DRAM_BASE || address + size > DRAM_BASE + DRAM_SIZE) {
        printf( "dram_store: address %d out of bounds\n", address);
        exit(1);
    }
    
    memcpy(dest,(dram->mem)+(address-DRAM_BASE),size);
}
void dram_store(DRAM* dram, uint32_t address, void* src, unsigned int size){
    if (address < DRAM_BASE || address + size > DRAM_BASE + DRAM_SIZE) {
        printf( "dram_store: address %d out of bounds\n", address);
        exit(1);
    }
    memcpy((dram->mem)+(address-DRAM_BASE),src,size);
}
// uint32_t dram_load_byte(DRAM* dram, uint32_t address){
//     return (uint32_t) dram->mem[address-DRAM_BASE];
// }
// uint32_t dram_load_half_word(DRAM* dram, uint32_t address){
//     return (uint32_t) dram->mem[address-DRAM_BASE]
//             | (uint32_t) dram->mem[address-DRAM_BASE + 1]<< 8;
// }
// uint32_t dram_load_word(DRAM* dram, uint32_t address){
//     return (uint32_t) dram->mem[address-DRAM_BASE]
//             | (uint32_t) dram->mem[address-DRAM_BASE + 1]<< 8
//             | (uint32_t) dram->mem[address-DRAM_BASE + 2]<< 16;
// }
// uint64_t dram_load_double_word(DRAM* dram, uint32_t address){
//     return (uint64_t) dram->mem[address-DRAM_BASE]
//             | (uint64_t) dram->mem[address-DRAM_BASE + 1]<< 8
//             | (uint64_t) dram->mem[address-DRAM_BASE + 2]<< 16
//             | (uint64_t) dram->mem[address-DRAM_BASE + 3]<< 24;
// }
 

