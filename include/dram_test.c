#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "dram.h"
// gcc dram_test.c -o dram_test
// ./dram_test

void test_byte_store_load(DRAM* dram) {
    uint8_t value = 0xAB;
    uint8_t loaded = 0;
    uint32_t addr = DRAM_BASE + 100;

    dram_store(dram, addr, &value, sizeof(value));
    dram_load(dram, addr, &loaded, sizeof(loaded));

    assert(loaded == value);
    printf("Byte test passed\n");
}

void test_half_word_store_load(DRAM* dram) {
    uint16_t value = 0xBEEF;
    uint16_t loaded = 0;
    uint32_t addr = DRAM_BASE + 200;

    dram_store(dram, addr, &value, sizeof(value));
    dram_load(dram, addr, &loaded, sizeof(loaded));

    assert(loaded == value);
    printf("Half-word test passed\n");
}

void test_word_store_load(DRAM* dram) {
    uint32_t value = 0xDEADBEEF;
    uint32_t loaded = 0;
    uint32_t addr = DRAM_BASE + 300;

    dram_store(dram, addr, &value, sizeof(value));
    dram_load(dram, addr, &loaded, sizeof(loaded));

    assert(loaded == value);
    printf("Word test passed\n");
}

void test_double_word_store_load(DRAM* dram) {
    uint64_t value = 0x1122334455667788ULL;
    uint64_t loaded = 0;
    uint32_t addr = DRAM_BASE + 400;

    dram_store(dram, addr, &value, sizeof(value));
    dram_load(dram, addr, &loaded, sizeof(loaded));

    assert(loaded == value);
    printf("Double-word test passed\n");
}
void test_endianness(DRAM* dram) {
    uint32_t value = 0x11223344;
    uint32_t addr = DRAM_BASE + 500;
    uint8_t* mem = dram->mem + (addr - DRAM_BASE);

    dram_store(dram, addr, &value, sizeof(value));

    // Inspect the byte at offset 0 and offset 3
    uint8_t b0 = mem[0];
    uint8_t b3 = mem[3];

    if (b0 == 0x44 && b3 == 0x11) {
        printf("Detected little-endian layout (OK for RISC-V)\n");
    } else if (b0 == 0x11 && b3 == 0x44) {
        // If this ever triggers, your host is big-endian
        fprintf(stderr, "ERROR: Detected big-endian layout — RISC-V simulation expects little-endian\n");
        assert(0);
    } else {
        fprintf(stderr, "ERROR: Unexpected byte pattern: byte0=0x%02X, byte3=0x%02X\n", b0, b3);
        assert(0);
    }
}


int main() {
    DRAM dram = {0};
   
    test_byte_store_load(&dram);
    test_half_word_store_load(&dram);
    test_word_store_load(&dram);
    test_double_word_store_load(&dram);
    test_endianness(&dram);

    printf("All DRAM tests passed successfully!\n");
    return 0;
}
