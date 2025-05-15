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

void dram_load(DRAM* dram, uint32_t address, void* dest, unsigned int size);
void dram_store(DRAM* dram, uint32_t address, void* src, unsigned int size);
#endif