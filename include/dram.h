#ifndef DRAM_H
#define DRAM_H
#include <stdint.h>
#include <string.h>
// https://projectf.io/posts/riscv-load-store/
#define DRAM_SIZE (1024*1024) //1 Mega
//OpenSBI (RV boot loader) entry point set at 0x80000000
#define DRAM_BASE 0x80000000
typedef struct DRAM {
    uint8_t mem[DRAM_SIZE];
}DRAM;

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
 
void dram_load(DRAM* dram, uint32_t address, void* dest, unsigned int size){
    // this will copy a byte/ half word(2 byts)/ word(4 byts) / double word(8 byts)
    memcpy(dest,(dram->mem)+(address-DRAM_BASE),size);
}
void dram_store(DRAM* dram, uint32_t address, void* src, unsigned int size){

    memcpy((dram->mem)+(address-DRAM_BASE),src,size);
}


#endif