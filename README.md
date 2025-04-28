# riscv_32_emulator
riscv_32_emulator/
├── include/
│   └── cpu.h           (CPU struct, prototypes)
│   └── memory.h        (Memory struct, prototypes)
├── src/
│   ├── cpu.c           (CPU step logic, execute instructions)
│   ├── memory.c        (Memory access, program loading)
│   ├── main.c          (Initialize CPU, load program, loop)
├── test/
│   └── program.bin     (first test program)
├── Makefile
└── README.md
