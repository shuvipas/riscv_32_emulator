#include <stdint.h>

#define DRAM_SIZE (1024*1024) //1 Mega
//OpenSBI (RV boot loader) entry point set at 0x80000000
#define DRAM_BASE 0x80000000
typedef struct DRAM {
    uint8_t mem[DRAM_SIZE];
}DRAM;