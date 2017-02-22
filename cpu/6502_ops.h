#ifndef __6502_OPS_H__
#define __6502_OPS_H__
#include "6502.h"
// GENERIC OPS
void op_bit(cpu* cpu) {}
// jump operations
void op_jmp(cpu* cpu) {}
void op_jmpa(cpu* cpu) {}
// comparison operations
void op_cpy(cpu* cpu) {}
void op_cpx(cpu* cpu) {}
void op_cmp(cpu* cpu) {}
// bitwise operations
void op_and(cpu* cpu) {
    strcpy(cpu->op.lhs, "and");
    cpu->reg.a = cpu->reg.a & cpu->op.data;
    cpu->reg.pc += 2;
}
void op_eor(cpu* cpu) {
    strcpy(cpu->op.lhs, "eor");
    cpu->reg.a = cpu->reg.a ^ cpu->op.data;
    cpu->reg.pc += 2;
}
void op_ora(cpu* cpu) {
    strcpy(cpu->op.lhs, "ora");
    cpu->reg.a = cpu->reg.a | cpu->op.data;
    cpu->reg.pc += 2;
}
// arithmetic operations
void op_adc(cpu* cpu) {
    strcpy(cpu->op.lhs, "adc");
    unsigned short new_value = cpu->reg.a + cpu->op.data;
    cpu->reg.a = (unsigned char)new_value;
    if(new_value & 0x100) {
        SET_BIT(cpu->reg.status, FLAG_CARRY);
    }
    cpu->reg.pc += 2;
}
void op_sbc(cpu* cpu) {}
// store operations
void _op_store(cpu* cpu, const char* lhs, unsigned char* target) {
    strcpy(cpu->op.lhs, lhs);
    cpu->reg.pc += 2;
    *target = cpu->op.data;
}
void op_sta(cpu* cpu) {
    _op_store(cpu, "sta", &cpu->reg.a);
}
void op_sty(cpu* cpu) {
    _op_store(cpu, "stx", &cpu->reg.x);
}
void op_stx(cpu* cpu) {
    _op_store(cpu, "sty", &cpu->reg.y);
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
// increment/decrement operations
void op_inc(cpu* cpu) {}
void op_iny(cpu* cpu) {}
void op_inx(cpu* cpu) {}
void op_dec(cpu* cpu) {}
void op_dey(cpu* cpu) {}
void op_dex(cpu* cpu) {}
// bitwise shift operations
void op_asl(cpu* cpu) {}
void op_rol(cpu* cpu) {}
void op_lsr(cpu* cpu) {}
void op_ror(cpu* cpu) {}
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
void _op_branch(cpu* cpu, const char* lhs, bool do_branch) {
    strcpy(cpu->op.lhs, lhs);
    if(do_branch) {
        unsigned short addr = cpu->reg.pc;
        cpu->reg.pc += (signed char)cpu->op.data;
        cpu->op.cycles = ((addr & 0xF0) == (cpu->reg.pc & 0xF0)) ? 4 : 3;
    } else {
        cpu->reg.pc += 2;
        cpu->op.cycles = 2;
    }
}
void op_bvs(cpu* cpu) {
    bool flag = GET_BIT(cpu->reg.status, FLAG_OVERFLOW);
    _op_branch(cpu, "bvs", flag);
}
void op_bvc(cpu* cpu) {
    bool flag = !(GET_BIT(cpu->reg.status, FLAG_OVERFLOW));
    _op_branch(cpu, "bvc", flag);
}
void op_bcc(cpu* cpu) {
    bool flag = !(GET_BIT(cpu->reg.status, FLAG_CARRY));
    _op_branch(cpu, "bcc", flag);
}
void op_bcs(cpu* cpu) {
    bool flag = GET_BIT(cpu->reg.status, FLAG_CARRY);
    _op_branch(cpu, "bcs", flag);
}
void op_bne(cpu* cpu) {
    bool flag = !(GET_BIT(cpu->reg.status, FLAG_ZERO));
    _op_branch(cpu, "bne", flag);
}
void op_beq(cpu* cpu) {
    bool flag = GET_BIT(cpu->reg.status, FLAG_ZERO);
    _op_branch(cpu, "beq", flag);
}
void op_bmi(cpu* cpu) {
    bool flag = GET_BIT(cpu->reg.status, FLAG_NEGATIVE);
    _op_branch(cpu, "bmi", flag);
}
void op_bpl(cpu* cpu) {
    bool flag = !(GET_BIT(cpu->reg.status, FLAG_ZERO));
    _op_branch(cpu, "bpl", flag);
}
// clear flag operations
void _op_clear(cpu* cpu, const char* lhs, unsigned char flag) {
    strcpy(cpu->op.lhs, lhs);
    CLEAR_BIT(cpu->reg.status, flag);
    cpu->reg.pc += 1;
    cpu->op.cycles = 2;
}
void op_clc(cpu* cpu) {
    _op_clear(cpu, "clc", FLAG_CARRY);
}
void op_cld(cpu* cpu) {
    _op_clear(cpu, "cld", FLAG_DECIMAL);
}
void op_cli(cpu* cpu) {
    _op_clear(cpu, "cli", FLAG_INTERRUPT);
}
void op_clv(cpu* cpu) {
    _op_clear(cpu, "clv", FLAG_OVERFLOW);
}
// set flag operations
void _op_set(cpu* cpu, const char* lhs, unsigned char flag) {
    strcpy(cpu->op.lhs, lhs);
    SET_BIT(cpu->reg.status, flag);
    cpu->reg.pc += 1;
    cpu->op.cycles = 2;
}
void op_sec(cpu* cpu) {
    _op_set(cpu, "sec", FLAG_CARRY);
}
void op_sei(cpu* cpu) {
    _op_set(cpu, "sei", FLAG_INTERRUPT);
}
void op_sed(cpu* cpu) {
    _op_set(cpu, "sed", FLAG_DECIMAL);
}
// transfer operations
void _op_transfer(cpu* cpu, const char* lhs, unsigned char source, unsigned char* target) {
    strcpy(cpu->op.lhs, lhs);
    *target = source;

    // set flags
    ((signed char)(*target) < 0) ? (SET_BIT(cpu->status, FLAG_NEGATIVE)) : (CLEAR_BIT(cpu->status, FLAG_NEGATIVE));
    ((*target) == 0) ? (SET_BIT(cpu->status, FLAG_ZERO)) : ((CLEAR_BIT(cpu->status, FLAG_ZERO)));

    cpu->reg.pc += 1;
    cpu->op.cycles = 2;
}
void op_tya(cpu* cpu) {
    _op_transfer(cpu, "tya", cpu->reg.y, &cpu->reg.a);
}
void op_tay(cpu* cpu) {
    _op_transfer(cpu, "tay", cpu->reg.a, &cpu->reg.y);
}
void op_txa(cpu* cpu) {
    _op_transfer(cpu, "txa", cpu->reg.x, &cpu->reg.a);
}
void op_tax(cpu* cpu) {
    _op_transfer(cpu, "tax", cpu->reg.a, &cpu->reg.x);
}
void op_txs(cpu* cpu) {
    _op_transfer(cpu, "txs", cpu->reg.x, &cpu->reg.sp);
}
void op_tsx(cpu* cpu) {
    _op_transfer(cpu, "tsx", cpu->reg.sp, &cpu->reg.x);
}
// no operation
void op_nop(cpu* cpu) {
    cpu->reg.pc += 1;
    cpu->op.cycles = 2;
}
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
        op_bcs, op_lda, op_ill, op_ill, op_ldy, op_lda, op_ldx, op_ill, op_clv, op_lda, op_tsx, op_ill, op_ldy, op_lda, op_ldx, op_ill,
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
#endif
