#ifndef __6502_OPS_H__
#define __6502_OPS_H__
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
    unsigned short new_value = cpu->reg.a + cpu->op.data;
    cpu->reg.a = (unsigned char)new_value;
    if(new_value & 0x100) {
        SET_BIT(cpu->reg.status, FLAG_CARRY);
    }
    cpu->reg.pc += 2;
}
void op_sta(cpu* cpu) {}
void op_lda(cpu* cpu) {}
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
void op_sec(cpu* cpu) {}
void op_pha(cpu* cpu) {}
void op_cli(cpu* cpu) {}
void op_pla(cpu* cpu) {}
void op_sei(cpu* cpu) {}
void op_dey(cpu* cpu) {}
void op_tya(cpu* cpu) {}
void op_tay(cpu* cpu) {}
void op_clv(cpu* cpu) {}
void op_iny(cpu* cpu) {}
void op_cld(cpu* cpu) {}
void op_inx(cpu* cpu) {}
void op_sed(cpu* cpu) {}
void op_txa(cpu* cpu) {}
void op_txs(cpu* cpu) {}
void op_tax(cpu* cpu) {}
void op_tsx(cpu* cpu) {}
void op_dex(cpu* cpu) {}
void op_nop(cpu* cpu) {
    // do nothing
    cpu->op.length = 1;
    cpu->op.cycles = 2;
}
void op_lpy(cpu* cpu) {}
// ILLEGAL OP
void op_ill(cpu* cpu) {}

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
#endif
