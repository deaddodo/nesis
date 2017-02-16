#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "memory/memory.h"
#include "cpu/6502.h"
#include "shared/utils.h"

void print_cpu_status(cpu* cpu) {
    printf("---------------------\n");
    printf("Program Counter: %04x\n", cpu->reg.pc);
    printf("Stack Pointer: %04x\n", cpu->reg.sp);
    printf("Status: %02x\n", cpu->reg.status);
    printf("Accumulator: %02x\n", cpu->reg.a);
    printf("X: %02x\n", cpu->reg.x);
    printf("Y: %02x\n", cpu->reg.y);
    printf("NMI: %04x\n", cpu->nmi);
    printf("Reset: %04x\n", cpu->reset);
    printf("IRQ: %04x\n", cpu->irq);
    printf("---------------------\n");
}

int main(int argc, char* argv[]) {
    cpu cpu;
    memory memory;
    bool error = false;
    unsigned int ops_to_run = 0;
    unsigned int ops_run = 0;

    if(argc < 2) {
        printf("ROM file needed!\n");
        error = true;
    }

    if(!error) {
        ops_to_run = (unsigned int)atoi(argv[2]);

        error = memory_init(&memory, argv[1]);

        if(!error) {
            cpu.nmi = LE_BYTE_TO_SHORT(read_byte(&memory, 0xfffa), read_byte(&memory, 0xfffb));
            cpu.reset = LE_BYTE_TO_SHORT(read_byte(&memory, 0xfffc), read_byte(&memory, 0xfffd));
            cpu.irq = LE_BYTE_TO_SHORT(read_byte(&memory, 0xfffe), read_byte(&memory, 0xffff));
            cpu_init(&cpu);

            print_cpu_status(&cpu);

            while(ops_run < ops_to_run) {
                cpu.buffer[0] = read_byte(&memory, cpu.reg.pc);
                cpu.buffer[1] = read_byte(&memory, cpu.reg.pc + 1);
                cpu.buffer[2] = read_byte(&memory, cpu.reg.pc + 2);

                while(cpu_tick(&cpu)) {
                    // Did the CPU make a bus request? Execute that.
                    if(cpu.status == BUS_REQUEST) {
                        if(cpu.bus_request.type == READ) {
                            cpu.bus_request.data = read_byte(&memory, cpu.bus_request.addr);
                            cpu.bus_request.set = true;
                        } else if (cpu.bus_request.type == WRITE) {
                            write_byte(&memory, cpu.bus_request.addr, cpu.bus_request.data);
                            cpu.bus_request.set = true;
                        }
                    } else {
                        printf("Failed!");
                    }
                }

                ops_run += 1;
            }

            print_cpu_status(&cpu);

            memory_destroy(&memory);
        }
    }

    return !error;
}