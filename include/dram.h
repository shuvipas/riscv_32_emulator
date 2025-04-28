#include <stdint.h>

#define DRAM_SIZE (1024*1024) //1 Mega
typedef struct DRAM {
    uint8_t mem[DRAM_SIZE];
}DRAM;