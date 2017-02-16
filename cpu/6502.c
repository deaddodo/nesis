#include "6502.h"

#define FLAG_CARRY 0
#define FLAG_ZERO 1
#define FLAG_INTERRUPT 2
#define FLAG_DECIMAL 3
#define FLAG_BREAK 4
#define FLAG_UNUSED 5
#define FLAG_OVERFLOW 6
#define FLAG_NEGATIVE 7

void cpu_bus_read(cpu* cpu, unsigned short addr) {
    cpu->status = BUS_REQUEST;
    cpu->bus_request.type = READ;
    cpu->bus_request.addr = addr;
}

void cpu_bus_write(cpu* cpu, unsigned short addr, unsigned char data) {
    cpu->status = BUS_REQUEST;
    cpu->bus_request.type = WRITE;
    cpu->bus_request.addr = addr;
    cpu->bus_request.data = data;
}

void cpu_reset_bus(cpu* cpu) {
    cpu->status = NEXT_INSTRUCTION;
    cpu->bus_request.type = NIL;
    cpu->bus_request.addr = 0x0000;
    cpu->bus_request.data = 0x00;
    cpu->bus_request.set = false;
}

unsigned char cpu_execute_op(cpu* cpu) {
    switch(cpu->buffer[0]) {
        case 0x69: {//ADC imm
            unsigned short new_value = cpu->reg.a + cpu->buffer[1];
            cpu->reg.a = (unsigned char)new_value;
            if(new_value & 0x100) {
                SET_BIT(cpu->reg.status, FLAG_CARRY);
            }
            cpu->reg.pc += 2;
        } break;
        case 0x78: {//SEI
            SET_BIT(cpu->reg.status, FLAG_INTERRUPT);
            cpu->reg.pc += 1;
        } break;
        case 0xa9: {//LDA imm
            cpu->reg.a = cpu->buffer[1];
            cpu->reg.pc += 2;
        } break;
        case 0xd8: {//CLD
            CLEAR_BIT(cpu->reg.status, FLAG_DECIMAL);
            cpu->reg.pc += 1;
        } break;
        case 0xf8: {// SED
            SET_BIT(cpu->reg.status, FLAG_DECIMAL);
            cpu->reg.pc += 1;
        } break;
    }

    return SUCCESS;
}

void cpu_init(cpu* cpu) {
    cpu->status = NEXT_INSTRUCTION;
    cpu->bus_request.type = NIL;
    cpu->bus_request.set = false;

    cpu->reg.sp = 0x0100;
    cpu->reg.pc = cpu->reset;
    cpu->reg.a = 0x00;
    cpu->reg.x = 0x00;
    cpu->reg.y = 0x00;
    cpu->reg.status = 0x30; // initialize with unused and B flag set.
}

unsigned char cpu_tick(cpu* cpu) {
    // printf("%08X\n", 0x00010000);
    printf("Buffer: %02x %02x %02x\n", cpu->buffer[0], cpu->buffer[1], cpu->buffer[2]);
    printf("Addr converted: %04x\n", LE_BYTE_TO_SHORT(cpu->buffer[1], cpu->buffer[2]));
    unsigned char status = cpu_execute_op(cpu);
    
    return status;
}

// let's use a real decoder loop later
/*unsigned char cpu_decode_op(cpu* cpu) {
    unsigned char op_set = cpu->buffer[0] & 0x03;
    unsigned char addr_mode = (cpu->buffer[0] >> 2) & 0x07;
    unsigned char op_code = (cpu->buffer[0] >> 5) & 0x07;
    unsigned char bytes = 1;
    unsigned char data = 0x00;

    if(op_set == 0x01 || op_set == 0x02) {
        switch(addr_mode) {
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x04:
        case 0x05:
            bytes += 1;
            data = cpu->buffer[1];
            break;
        case 0x03:
        case 0x06:
        case 0x07:
            bytes += 2;
            break;
        }
    }

    switch(op_set) {
    case 0x01:
        switch(op_code) {
        case 0x03:
            printf("ADC\n");
            break;
        }
        break;
    case 0x02:
        switch(op_code) {
        case 0x02:
            printf("LSR\n");
            break;
        }
        break;
    default:
        printf("Unsupported OP\n");
    }

    cpu->PC += bytes;

    return 0;
}*/