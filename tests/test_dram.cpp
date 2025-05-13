#include "gtest/gtest.h"

#include <cstring>

TEST(DRAMTest, StoreAndLoad64BitValue) {
    DRAM dram = {};
    uint64_t value = 0x0123456789ABCDEF;
    uint64_t loaded = 0;

    uint64_t addr = DRAM_BASE + 16;
    dram_store_64(&dram, addr, value);
    dram_load(&dram, addr, sizeof(uint64_t), &loaded);

    EXPECT_EQ(loaded, value);
}

TEST(DRAMTest, StoreAndLoadDifferentSizes) {
    DRAM dram = {};
    uint32_t value = 0xAABBCCDD;
    uint32_t loaded = 0;
    uint32_t addr = DRAM_BASE + 32;

    // store manually
    dram.mem[addr - DRAM_BASE] = 0xDD;
    dram.mem[addr - DRAM_BASE + 1] = 0xCC;
    dram.mem[addr - DRAM_BASE + 2] = 0xBB;
    dram.mem[addr - DRAM_BASE + 3] = 0xAA;

    dram_load(&dram, addr, sizeof(uint32_t), &loaded);
    EXPECT_EQ(loaded, value);
}
