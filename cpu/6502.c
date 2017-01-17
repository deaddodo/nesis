#include "6502.h"

void cpu_init(cpu* cpu) {
    (*cpu).status = PASS;
    (*cpu).PC = 0;
}

void cpu_tick(unsigned char data) {
    printf("%08X\n", 0x00010000);
    return 0x00000000;
}