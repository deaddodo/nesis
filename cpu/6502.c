#include "6502.h"

/*
 * Pull these out into 6502_ops when you figure out how to do forward declarations correctly
 * ...dumbass
 */
// GENERIC OPS
void op_bit(cpu* cpu) {}
void op_jmp(cpu* cpu) {}
void op_jmpa(cpu* cpu) {}
void op_sty(cpu* cpu) {}
void op_ldy(cpu* cpu) {}
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
void op_sta(cpu* cpu) {}
void op_lda(cpu* cpu) {
    strcpy(cpu->op.lhs, "lda");
    cpu->reg.a = cpu->op.data;
    cpu->op.length = 2;
}
void op_cmp(cpu* cpu) {}
void op_sbc(cpu* cpu) {}
void op_asl(cpu* cpu) {}
void op_rol(cpu* cpu) {}
void op_lsr(cpu* cpu) {}
void op_ror(cpu* cpu) {}
void op_stx(cpu* cpu) {}
void op_ldx(cpu* cpu) {}
void op_dec(cpu* cpu) {}
void op_inc(cpu* cpu) {}
// ONE OFF OPS
void op_brk(cpu* cpu) {}
void op_bpl(cpu* cpu) {}
void op_jsr(cpu* cpu) {}
void op_bmi(cpu* cpu) {}
void op_rti(cpu* cpu) {}
void op_bvc(cpu* cpu) {}
void op_rts(cpu* cpu) {}
void op_bvs(cpu* cpu) {}
void op_bcc(cpu* cpu) {}
void op_bcs(cpu* cpu) {}
void op_bne(cpu* cpu) {}
void op_beq(cpu* cpu) {}
void op_php(cpu* cpu) {}
void op_clc(cpu* cpu) {}
void op_plp(cpu* cpu) {}
void op_pha(cpu* cpu) {}
void op_cli(cpu* cpu) {}
void op_pla(cpu* cpu) {}
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
void op_clv(cpu* cpu) {}
void op_iny(cpu* cpu) {}
void op_cld(cpu* cpu) {}
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
/*
 * End of 6502_ops
 */

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
    cpu->bus_request.single_page = true;
}

// TODO: convert this to using a map
unsigned char cpu_decode_op(cpu* cpu) {
    unsigned char op_set = cpu->buffer[0] & (unsigned char)0x03;
    unsigned char addr_mode = (cpu->buffer[0] >> 2) & (unsigned char)0x07;
    unsigned char op_code = (cpu->buffer[0] >> 5) & (unsigned char)0x07;

    cpu->op.call = op[cpu->buffer[0]];

    switch(addr_mode) {
        case 0x00: //
            cpu_bus_read(cpu, cpu->buffer[1] + cpu->reg.x);
            break;
        case 0x01:
            break;
        case 0x02:
            cpu->op.data = cpu->buffer[1];
            cpu->op.ready = true;
            sprintf(cpu->op.rhs, "#$%02x", cpu->op.data);
            break;
        case 0x03:
            break;
        case 0x04:
            break;
        case 0x05:
            break;
        case 0x06:
            break;
        case 0x07:
            break;
    }

    return 0;
}

unsigned char cpu_execute_op(cpu* cpu) {
    (*(cpu->op.call))(cpu); // call the op function
    cpu->reg.pc += cpu->op.length;

    return SUCCESS;
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
    } else {
        cpu_execute_op(cpu);
    }
    unsigned char status = cpu_execute_op(cpu);
    
    return status;
}