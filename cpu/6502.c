#include "6502_ops.h"

void cpu_bus_read(cpu* cpu, unsigned short addr, bool direct) {
    cpu->status = BUS_REQUEST;
    cpu->bus_request.type = READ;
    cpu->bus_request.addr = addr;
    cpu->bus_request.direct = direct;
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
    cpu->bus_request.single_page = true;
    cpu->bus_request.direct = true;
}

unsigned char cpu_decode_op(cpu* cpu) {
    cpu->op.call = op[cpu->buffer[0]];

    switch(op_addr[cpu->buffer[0]]) {
        case AM_A: {
            cpu->op.data = cpu->reg.a;
            cpu->op.ready = true;
            strcpy(cpu->op.rhs, "A");
        } break;
        case AM_ABS: {
            unsigned short addr = LE_BYTE_TO_SHORT(cpu->buffer[1], cpu->buffer[2]);
            cpu_bus_read(cpu, addr, true);
            sprintf(cpu->op.rhs, "$%04x", addr);
            cpu->reg.pc += 1;
        } break;
        case AM_ABSX: {
            unsigned short addr = LE_BYTE_TO_SHORT(cpu->buffer[1], cpu->buffer[2]);
            cpu_bus_read(cpu, addr + cpu->reg.x, true);
            sprintf(cpu->op.rhs, "$%04x,X", addr);
            cpu->reg.pc += 1;
        } break;
        case AM_ABSY: {
            unsigned short addr = LE_BYTE_TO_SHORT(cpu->buffer[1], cpu->buffer[2]);
            cpu_bus_read(cpu, addr + cpu->reg.y, true);
            sprintf(cpu->op.rhs, "$%04x,Y", addr);
            cpu->reg.pc += 1;
        } break;
        case AM_IMM: {
            cpu->op.data = cpu->buffer[1];
            cpu->op.ready = true;
            sprintf(cpu->op.rhs, "#$%02x", cpu->op.data);
        } break;
        case AM_IMP: {
            cpu->op.ready = true;
            strcpy(cpu->op.rhs, "");
        } break;
        case AM_IND: {
            unsigned short addr = LE_BYTE_TO_SHORT(cpu->buffer[1], cpu->buffer[2]);
            cpu_bus_read(cpu, addr, false);
            sprintf(cpu->op.rhs, "($%04x)", addr);
        } break;
        // these might be wrong
        case AM_INDX: {
            unsigned short addr = cpu->buffer[1] + cpu->reg.x; // Zero-page only w/o carry
            cpu_bus_read(cpu, addr, false);
            sprintf(cpu->op.rhs, "($%02x,X)", cpu->buffer[1]);
        } break;
        case AM_INDY: {
            unsigned short addr = (unsigned short)cpu->buffer[1] + cpu->reg.y; // Zero-page only, w/ carry
            cpu_bus_read(cpu, addr, false);
            sprintf(cpu->op.rhs, "($%02x),Y", cpu->buffer[1]);
        } break;
        // research them
        case AM_REL: {
            cpu->op.data = cpu->buffer[1];
            cpu->op.ready = true;
            sprintf(cpu->op.rhs, "$%02x", cpu->op.data);
        } break;
        case AM_ZPG: {
            unsigned short addr = cpu->buffer[1];
            cpu_bus_read(cpu, addr, true);
            sprintf(cpu->op.rhs, "$%02x", cpu->buffer[1]);
        } break;
        case AM_ZPGX: {
            unsigned short addr = cpu->buffer[1] + cpu->reg.x;
            cpu_bus_read(cpu, addr, true);
            sprintf(cpu->op.rhs, "$%02x,X", cpu->buffer[1]);
        } break;
        case AM_ZPGY: {
            unsigned short addr = cpu->buffer[1] + cpu->reg.y;
            cpu_bus_read(cpu, addr, true);
            sprintf(cpu->op.rhs, "$%02x,Y", cpu->buffer[1]);
        } break;
        case AM_NIL: {
            cpu->op.ready = true;
            strcpy(cpu->op.rhs, "???");
        } break;
    }

    return 0;
}

void cpu_reset_op(cpu* cpu) {
    cpu->op.call = 0;
    cpu->op.cycles = 0;
    cpu->op.length = 0;
    cpu->op.data = 0x00;
    cpu->op.ready = false;
    strcpy(cpu->op.lhs, "");
    strcpy(cpu->op.rhs, "");
}

unsigned char cpu_execute_op(cpu* cpu) {
    (*(cpu->op.call))(cpu); // call the op function
    printf("%s %s\n", cpu->op.lhs, cpu->op.rhs);
    cpu_reset_op(cpu);
    cpu_reset_bus(cpu);

    return SUCCESS;
}

void cpu_init(cpu* cpu) {
    cpu_reset_bus(cpu);

    cpu->reg.sp = 0x00;
    cpu->reg.pc = cpu->reset;
    cpu->reg.a = 0x00;
    cpu->reg.x = 0x00;
    cpu->reg.y = 0x00;
    cpu->reg.status = 0x30; // initialize with unused and B flag set.
}

unsigned char cpu_tick(cpu* cpu) {
    printf("Buffer: %02x %02x %02x\n", cpu->buffer[0], cpu->buffer[1], cpu->buffer[2]);
    if(!cpu->op.ready) {
        cpu_decode_op(cpu);
    }

    if(cpu->op.ready){
        cpu_execute_op(cpu);
    }
    
    return cpu->status;
}