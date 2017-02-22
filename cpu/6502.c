#include "6502.h"

/*
 * Pull these out into 6502_ops when you figure out how to do forward declarations correctly
 * ...dumbass
 */
// GENERIC OPS
void op_bit(cpu* cpu) {}
void op_jmp(cpu* cpu) {}
void op_jmpa(cpu* cpu) {}
void op_cpy(cpu* cpu) {}
void op_cpx(cpu* cpu) {}
void op_ora(cpu* cpu) {}
void op_and(cpu* cpu) {}
void op_eor(cpu* cpu) {}
void op_adc(cpu* cpu) {
    strcpy(cpu->op.lhs, "adc");
    unsigned short new_value = cpu->reg.a + cpu->op.data;
    cpu->reg.a = (unsigned char)new_value;
    if(new_value & 0x100) {
        SET_BIT(cpu->reg.status, FLAG_CARRY);
    }
    cpu->reg.pc += 2;
}
// store operations
void _op_store(cpu* cpu, const char* lhs, unsigned char* target) {
    strcpy(cpu->op.lhs, lhs);
    cpu->reg.pc += 2;
    *target = cpu->op.data;
}
void op_sta(cpu* cpu) {
    strcpy(cpu->op.lhs, "sta");
    cpu->reg.pc += 2;
    cpu->reg.a = cpu->op.data;
}
void op_sty(cpu* cpu) {
    strcpy(cpu->op.lhs, "stx");
    cpu->reg.pc += 2;
    cpu->reg.x = cpu->op.data;
}
void op_stx(cpu* cpu) {
    strcpy(cpu->op.lhs, "sty");
    cpu->reg.pc += 2;
    cpu->reg.y = cpu->op.data;
}
// load operations
void _op_load(cpu* cpu, const char* lhs, unsigned char* target) {
    strcpy(cpu->op.lhs, lhs);

    // set flags
    ((signed char)cpu->op.data < 0) ? (SET_BIT(cpu->status, FLAG_NEGATIVE)) : (CLEAR_BIT(cpu->status, FLAG_NEGATIVE));
    (cpu->op.data == 0) ? (SET_BIT(cpu->status, FLAG_ZERO)) : ((CLEAR_BIT(cpu->status, FLAG_ZERO)));

    *target = cpu->op.data;
    cpu->reg.pc += 2;
}
void op_lda(cpu* cpu) {
    _op_load(cpu, "lda", &cpu->reg.a);
}
void op_ldx(cpu* cpu) {
    _op_load(cpu, "ldx", &cpu->reg.x);
}
void op_ldy(cpu* cpu) {
    _op_load(cpu, "ldy", &cpu->reg.y);
}
void op_cmp(cpu* cpu) {}
void op_sbc(cpu* cpu) {}
void op_asl(cpu* cpu) {}
void op_rol(cpu* cpu) {}
void op_lsr(cpu* cpu) {}
void op_ror(cpu* cpu) {}
void op_dec(cpu* cpu) {}
void op_inc(cpu* cpu) {}
// ONE OFF OPS
void op_brk(cpu* cpu) {}
void op_jsr(cpu* cpu) {}
void op_rti(cpu* cpu) {}
void op_rts(cpu* cpu) {}
void op_php(cpu* cpu) {}
void op_plp(cpu* cpu) {}
void op_pha(cpu* cpu) {}
void op_pla(cpu* cpu) {}
// TODO: branch needs to be fixed to be relative and work with signed values
void op_bvs(cpu* cpu) {
    strcpy(cpu->op.lhs, "bvs");
    bool flag = GET_BIT(cpu->reg.status, FLAG_OVERFLOW);
    if(flag) {
        unsigned short addr = cpu->reg.pc;
        cpu->reg.pc += (signed char)cpu->op.data;
        cpu->op.cycles = ((addr & 0xF0) == (cpu->reg.pc & 0xF0)) ? 3 : 2;
    } else {
        cpu->op.length = 2;
        cpu->op.cycles = 2;
    }
}
void op_bcc(cpu* cpu) {
    strcpy(cpu->op.lhs, "bcc");
    bool flag = GET_BIT(cpu->reg.status, FLAG_CARRY);
    if(!flag) {
        unsigned short addr = cpu->reg.pc;
        cpu->reg.pc += (signed char)cpu->op.data;
        cpu->op.cycles = ((addr & 0xF0) == (cpu->reg.pc & 0xF0)) ? 3 : 2;
    } else {
        cpu->op.length = 2;
        cpu->op.cycles = 2;
    }
}
void op_bcs(cpu* cpu) {
    strcpy(cpu->op.lhs, "bcs");
    bool flag = GET_BIT(cpu->reg.status, FLAG_CARRY);
    if(flag) {
        unsigned short addr = cpu->reg.pc;
        cpu->reg.pc += (signed char)cpu->op.data;
        cpu->op.length = 0;
        cpu->op.cycles = ((addr & 0xF0) == (cpu->reg.pc & 0xF0)) ? 3 : 2;
    } else {
        cpu->op.length = 2;
        cpu->op.cycles = 2;
    }
}
void op_bne(cpu* cpu) {
    strcpy(cpu->op.lhs, "bne");
    bool flag = GET_BIT(cpu->reg.status, FLAG_ZERO);
    if(!flag) {
        unsigned short addr = cpu->reg.pc;
        cpu->reg.pc += (signed char)cpu->op.data;
        cpu->op.length = 0;
        cpu->op.cycles = ((addr & 0xF0) == (cpu->reg.pc & 0xF0)) ? 3 : 2;
    } else {
        cpu->op.length = 2;
        cpu->op.cycles = 2;
    }
}
void op_beq(cpu* cpu) {
    strcpy(cpu->op.lhs, "beq");
    bool flag = GET_BIT(cpu->reg.status, FLAG_ZERO);
    if(flag) {
        unsigned short addr = cpu->reg.pc;
        cpu->reg.pc += (signed char)cpu->op.data;
        cpu->op.length = 0;
        cpu->op.cycles = ((addr & 0xF0) == (cpu->reg.pc & 0xF0)) ? 3 : 2;
    } else {
        cpu->op.length = 2;
        cpu->op.cycles = 2;
    }
}
void op_bvc(cpu* cpu) {
    strcpy(cpu->op.lhs, "bvc");
    bool flag = GET_BIT(cpu->reg.status, FLAG_OVERFLOW);
    if(!flag) {
        unsigned short addr = cpu->reg.pc;
        cpu->reg.pc += (signed char)cpu->op.data;
        cpu->op.length = 0;
        cpu->op.cycles = ((addr & 0xF0) == (cpu->reg.pc & 0xF0)) ? 3 : 2;
    } else {
        cpu->op.length = 2;
        cpu->op.cycles = 2;
    }
}
void op_bmi(cpu* cpu) {
    strcpy(cpu->op.lhs, "bmi");
    bool flag = GET_BIT(cpu->reg.status, FLAG_NEGATIVE);
    if(flag) {
        unsigned short addr = cpu->reg.pc;
        cpu->reg.pc += (signed char)cpu->op.data;
        cpu->op.length = 0;
        cpu->op.cycles = ((addr & 0xF0) == (cpu->reg.pc & 0xF0)) ? 3 : 2;
    } else {
        cpu->op.length = 2;
        cpu->op.cycles = 2;
    }
}
void op_bpl(cpu* cpu) {
    strcpy(cpu->op.lhs, "bvc");
    bool flag = GET_BIT(cpu->reg.status, FLAG_NEGATIVE);
    if(!flag) {
        unsigned short addr = cpu->reg.pc;
        cpu->reg.pc += (signed char)cpu->op.data;
        cpu->op.length = 0;
        cpu->op.cycles = ((addr & 0xF0) == (cpu->reg.pc & 0xF0)) ? 3 : 2;
    } else {
        cpu->op.length = 2;
        cpu->op.cycles = 2;
    }
}
void op_clc(cpu* cpu) {
    strcpy(cpu->op.lhs, "clc");
    CLEAR_BIT(cpu->reg.status, FLAG_CARRY);
    cpu->op.cycles = 2;
    cpu->op.length = 1;
}
void op_cld(cpu* cpu) {
    strcpy(cpu->op.lhs, "cld");
    CLEAR_BIT(cpu->reg.status, FLAG_DECIMAL);
    cpu->op.cycles = 2;
    cpu->op.length = 1;
}
void op_cli(cpu* cpu) {
    strcpy(cpu->op.lhs, "cli");
    CLEAR_BIT(cpu->reg.status, FLAG_INTERRUPT);
    cpu->op.cycles = 2;
    cpu->op.length = 1;
}
void op_clv(cpu* cpu) {
    strcpy(cpu->op.lhs, "clv");
    CLEAR_BIT(cpu->reg.status, FLAG_OVERFLOW);
    cpu->op.cycles = 2;
    cpu->op.length = 1;
}
void op_sec(cpu* cpu) {
    strcpy(cpu->op.lhs, "sec");
    SET_BIT(cpu->reg.status, FLAG_CARRY);
    cpu->op.cycles = 2;
    cpu->op.length = 1;
}
void op_sei(cpu* cpu) {
    strcpy(cpu->op.lhs, "sei");
    SET_BIT(cpu->reg.status, FLAG_INTERRUPT);
    cpu->op.cycles = 2;
    cpu->op.length = 1;
}
void op_sed(cpu* cpu) {
    strcpy(cpu->op.lhs, "sed");
    SET_BIT(cpu->reg.status, FLAG_DECIMAL);
    cpu->op.cycles = 2;
    cpu->op.length = 1;
}
void op_dey(cpu* cpu) {}
void op_tya(cpu* cpu) {}
void op_tay(cpu* cpu) {}
void op_iny(cpu* cpu) {}
void op_inx(cpu* cpu) {}
void op_txa(cpu* cpu) {}
void op_txs(cpu* cpu) {}
void op_tax(cpu* cpu) {}
void op_tsx(cpu* cpu) {}
void op_dex(cpu* cpu) {}
void op_nop(cpu* cpu) {
    cpu->op.length = 1;
    cpu->op.cycles = 2;
}
void op_lpy(cpu* cpu) {}
// ILLEGAL OP
void op_ill(cpu* cpu) {
    sprintf(cpu->op.lhs, "%02x", cpu->buffer[0]);
    printf("illegal op: %02x\n", cpu->buffer[0]);
}

void (*op[256])(cpu* cpu) = {
        op_brk, op_ora, op_ill, op_ill, op_ill, op_ora, op_asl, op_ill, op_php, op_ora, op_asl, op_ill, op_ill, op_ora, op_asl, op_ill,
        op_bpl, op_ora, op_ill, op_ill, op_ill, op_ora, op_asl, op_ill, op_clc, op_ora, op_ill, op_ill, op_ill, op_ora, op_asl, op_ill,
        op_jsr, op_and, op_ill, op_ill, op_bit, op_and, op_rol, op_ill, op_plp, op_and, op_rol, op_ill, op_bit, op_and, op_rol, op_ill,
        op_bmi, op_and, op_ill, op_ill, op_ill, op_and, op_rol, op_ill, op_sec, op_and, op_ill, op_ill, op_ill, op_and, op_rol, op_ill,
        op_rti, op_eor, op_ill, op_ill, op_ill, op_eor, op_lsr, op_ill, op_pha, op_eor, op_lsr, op_ill, op_jmpa, op_eor, op_lsr, op_ill,
        op_bvc, op_eor, op_ill, op_ill, op_ill, op_eor, op_lsr, op_ill, op_cli, op_eor, op_ill, op_ill, op_ill, op_eor, op_lsr, op_ill,
        op_rts, op_adc, op_ill, op_ill, op_ill, op_adc, op_ror, op_ill, op_pla, op_adc, op_ror, op_ill, op_jmp, op_adc, op_ror, op_ill,
        op_bvs, op_adc, op_ill, op_ill, op_ill, op_adc, op_ror, op_ill, op_sei, op_adc, op_ill, op_ill, op_ill, op_adc, op_ror, op_ill,
        op_ill, op_sta, op_ill, op_ill, op_sty, op_sta, op_stx, op_ill, op_dey, op_ill, op_txa, op_ill, op_sty, op_sta, op_stx, op_ill,
        op_bcc, op_sta, op_ill, op_ill, op_sty, op_sta, op_stx, op_ill, op_tya, op_sta, op_txs, op_ill, op_ill, op_sta, op_ill, op_ill,
        op_ldy, op_lda, op_ldx, op_ill, op_ldy, op_lda, op_ldx, op_ill, op_tay, op_lda, op_tax, op_ill, op_ldy, op_lda, op_ldx, op_ill,
        op_bcs, op_lda, op_ill, op_ill, op_ldy, op_lda, op_ldx, op_ill, op_clv, op_lda, op_tsx, op_ill, op_lpy, op_lda, op_ldx, op_ill,
        op_cpy, op_cmp, op_ill, op_ill, op_cpy, op_cmp, op_dec, op_ill, op_iny, op_cmp, op_dex, op_ill, op_cpy, op_cmp, op_dec, op_ill,
        op_bne, op_cmp, op_ill, op_ill, op_ill, op_cmp, op_dec, op_ill, op_cld, op_cmp, op_ill, op_ill, op_ill, op_cmp, op_dec, op_ill,
        op_cpx, op_sbc, op_ill, op_ill, op_cpx, op_sbc, op_inc, op_ill, op_inx, op_sbc, op_nop, op_ill, op_cpx, op_sbc, op_inc, op_ill,
        op_beq, op_sbc, op_ill, op_ill, op_ill, op_sbc, op_inc, op_ill, op_sed, op_sbc, op_ill, op_ill, op_ill, op_sbc, op_inc, op_ill,
};

typedef enum {
    AM_A,
    AM_ABS,
    AM_ABSX,
    AM_ABSY,
    AM_IMM,
    AM_IMP,
    AM_IND,
    AM_INDX,
    AM_INDY,
    AM_REL,
    AM_ZPG,
    AM_ZPGX,
    AM_ZPGY,
    AM_NIL
} cpu_addr_mode;

cpu_addr_mode op_addr[256] = {
//      00      01       02      03      04       05       06       07      08      09       0a      0b      0c       0d       0e       0f
        AM_IMP, AM_INDX, AM_NIL, AM_NIL, AM_NIL,  AM_ZPG,  AM_ZPG,  AM_NIL, AM_IMP, AM_IMM,  AM_A,   AM_NIL, AM_NIL,  AM_ABS,  AM_ABS,  AM_NIL,
        AM_REL, AM_INDY, AM_NIL, AM_NIL, AM_NIL,  AM_ZPGX, AM_ZPGX, AM_NIL, AM_IMP, AM_ABSY, AM_NIL, AM_NIL, AM_NIL,  AM_ABSX, AM_ABSX, AM_NIL,
        AM_ABS, AM_INDX, AM_NIL, AM_NIL, AM_ZPG,  AM_ZPG,  AM_ZPG,  AM_NIL, AM_IMP, AM_IMM,  AM_A,   AM_NIL, AM_ABS,  AM_ABS,  AM_ABS,  AM_NIL,
        AM_REL, AM_INDY, AM_NIL, AM_NIL, AM_NIL,  AM_ZPGX, AM_ZPGX, AM_NIL, AM_IMP, AM_ABSY, AM_NIL, AM_NIL, AM_NIL,  AM_ABSX, AM_ABSX, AM_NIL,
        AM_IMP, AM_INDX, AM_NIL, AM_NIL, AM_NIL,  AM_ZPG,  AM_ZPG,  AM_NIL, AM_IMP, AM_IMM,  AM_A,   AM_NIL, AM_ABS,  AM_ABS,  AM_ABS,  AM_NIL,
        AM_REL, AM_INDY, AM_NIL, AM_NIL, AM_NIL,  AM_ZPGX, AM_ZPGX, AM_NIL, AM_IMP, AM_ABSY, AM_NIL, AM_NIL, AM_NIL,  AM_ABSX, AM_ABSX, AM_NIL,
        AM_IMP, AM_INDX, AM_NIL, AM_NIL, AM_NIL,  AM_ZPG,  AM_ZPG,  AM_NIL, AM_IMP, AM_IMM,  AM_A,   AM_NIL, AM_IND,  AM_ABS,  AM_ABS,  AM_NIL,
        AM_REL, AM_INDY, AM_NIL, AM_NIL, AM_NIL,  AM_ZPGX, AM_ZPGX, AM_NIL, AM_IMP, AM_ABSY, AM_NIL, AM_NIL, AM_NIL,  AM_ABSX, AM_ABSX, AM_NIL,
        AM_NIL, AM_INDX, AM_NIL, AM_NIL, AM_ZPG,  AM_ZPG,  AM_ZPG,  AM_NIL, AM_IMP, AM_IMM,  AM_IMP, AM_NIL, AM_ABS,  AM_ABS,  AM_ABS,  AM_NIL,
        AM_REL, AM_INDY, AM_NIL, AM_NIL, AM_ZPGX, AM_ZPGX, AM_ZPGY, AM_NIL, AM_IMP, AM_ABSY, AM_IMP, AM_NIL, AM_NIL,  AM_ABSX, AM_NIL,  AM_NIL,
        AM_IMM, AM_INDX, AM_IMM, AM_NIL, AM_ZPG,  AM_ZPG,  AM_ZPG,  AM_NIL, AM_IMP, AM_IMM,  AM_IMP, AM_NIL, AM_ABS,  AM_ABS,  AM_ABS,  AM_NIL,
        AM_REL, AM_INDY, AM_NIL, AM_NIL, AM_ZPGX, AM_ZPGX, AM_ZPGY, AM_NIL, AM_IMP, AM_ABSY, AM_IMP, AM_NIL, AM_ABSX, AM_ABSX, AM_ABSY, AM_NIL,
        AM_IMM, AM_INDX, AM_NIL, AM_NIL, AM_ZPG,  AM_ZPG,  AM_ZPG,  AM_NIL, AM_IMP, AM_IMM,  AM_IMP, AM_NIL, AM_ABS,  AM_ABS,  AM_ABS,  AM_NIL,
        AM_REL, AM_INDY, AM_NIL, AM_NIL, AM_NIL,  AM_ZPGX, AM_ZPGX, AM_NIL, AM_IMP, AM_ABSY, AM_NIL, AM_NIL, AM_NIL,  AM_ABSX, AM_ABSX, AM_NIL,
        AM_IMM, AM_INDX, AM_NIL, AM_NIL, AM_ZPG,  AM_ZPG,  AM_ZPG,  AM_NIL, AM_IMP, AM_IMM,  AM_IMP, AM_NIL, AM_ABS,  AM_ABS,  AM_ABS,  AM_NIL,
        AM_REL, AM_INDY, AM_NIL, AM_NIL, AM_NIL,  AM_ZPGX, AM_ZPGX, AM_NIL, AM_IMP, AM_ABSY, AM_NIL, AM_NIL, AM_NIL,  AM_ABSX, AM_ABSX, AM_NIL,
};
/*
 * End of 6502_ops
 */

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

unsigned char cpu_execute_op(cpu* cpu) {
    (*(cpu->op.call))(cpu); // call the op function
    cpu->reg.pc += cpu->op.length;

    return SUCCESS;
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

void cpu_init(cpu* cpu) {
    cpu_reset_bus(cpu);

    cpu->reg.sp = 0x0100;
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
        printf("%s %s\n", cpu->op.lhs, cpu->op.rhs);
        cpu_reset_op(cpu);
        cpu_reset_bus(cpu);
    }
    
    return cpu->status;
}