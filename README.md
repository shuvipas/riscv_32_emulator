# riscv_32_emulator
riscv_emulator/
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

Core Components
    - Memory (dram 1 Mib)
    - cpu 
        - 32 regs
        - pc
    
Work Flow
- Core Components
- cpu reset
    - init sp and pc
- main loop
        - initialize  the cpu
        - Instruction Fetch/Decode/Execute+Memory

- Fetch stage
    - get the current Instruction from the  pc address (need to get the next 4 bytes) and pc++ 
- Decode stage
- Execute+Memory stage

