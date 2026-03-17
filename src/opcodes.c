#include "cpu.h"
#include "bus.h"
#include "types.h"
#include "opcodes.h"
#include <stdbool.h>

// 0x00 - 0x0F
void op_nop(CPU *cpu, Bus *bus){
    cpu->cycles += 4;
}
void op_ld_bc_n16(CPU *cpu, Bus *bus){
    cpu->BC.full = cpu_read_word(cpu, bus);
    cpu->cycles += 12;
}
void op_ld_bc_ind_a(CPU *cpu, Bus *bus){

}
void op_inc_bc(CPU *cpu, Bus *bus){
    cpu->BC.full++;
    cpu->cycles += 8;
}
void op_inc_b(CPU *cpu, Bus *bus){
    
}
void op_dec_b(CPU *cpu, Bus *bus) {
}
void op_ld_b_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC);
    cpu->BC.bytes.hi = val;
    cpu->cycles += 8;
}
void op_rlca(CPU *cpu, Bus *bus) {
}
void op_ld_n16_ind_sp(CPU *cpu, Bus *bus) {
}
void op_add_hl_bc(CPU *cpu, Bus *bus) {
}
void op_ld_a_bc_ind(CPU *cpu, Bus *bus) {
}
void op_dec_bc(CPU *cpu, Bus *bus) {
    cpu->BC.full--;
    cpu->cycles += 8;
}
void op_inc_c(CPU *cpu, Bus *bus) {
}
void op_dec_c(CPU *cpu, Bus *bus) {
}
void op_ld_c_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC);
    cpu->BC.bytes.lo = val;
    cpu->cycles += 8;
}
void op_rrca(CPU *cpu, Bus *bus) {
}

// 0x10 - 0x1F
void op_stop(CPU *cpu, Bus *bus) {
}
void op_ld_de_n16(CPU *cpu, Bus *bus) {
}
void op_ld_de_ind_a(CPU *cpu, Bus *bus) {
}
void op_inc_de(CPU *cpu, Bus *bus) {
    cpu->DE.full++;
    cpu->cycles += 8;
}
void op_inc_d(CPU *cpu, Bus *bus) {
}
void op_dec_d(CPU *cpu, Bus *bus) {
}
void op_ld_d_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC);
    cpu->DE.bytes.hi = val;
    cpu->cycles += 8;
}
void op_rla(CPU *cpu, Bus *bus) {
}
void op_jr_n8(CPU *cpu, Bus *bus) {
}
void op_add_hl_de(CPU *cpu, Bus *bus) {
}
void op_ld_a_de_ind(CPU *cpu, Bus *bus) {
}
void op_dec_de(CPU *cpu, Bus *bus) {
    cpu->DE.full--;
    cpu->cycles += 8;
}
void op_inc_e(CPU *cpu, Bus *bus) {
}
void op_dec_e(CPU *cpu, Bus *bus) {
}
void op_ld_e_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC);
    cpu->DE.bytes.lo = val;
    cpu->cycles += 8;
}
void op_rra(CPU *cpu, Bus *bus) {
}

// 0x20 - 0x2F
void op_jr_nz_n8(CPU *cpu, Bus *bus) {
}
void op_ld_hl_n16(CPU *cpu, Bus *bus) {
}
void op_ld_hli_ind_a(CPU *cpu, Bus *bus) {
}
void op_inc_hl(CPU *cpu, Bus *bus) {
    cpu->HL.full++;
    cpu->cycles += 8;
}
void op_inc_h(CPU *cpu, Bus *bus) {
}
void op_dec_h(CPU *cpu, Bus *bus) {
}
void op_ld_h_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC);
    cpu->HL.bytes.hi = val;
    cpu->cycles += 8;
}
void op_daa(CPU *cpu, Bus *bus) {
}
void op_jr_z_n8(CPU *cpu, Bus *bus) {
}
void op_add_hl_hl(CPU *cpu, Bus *bus) {
}
void op_ld_a_hli_ind(CPU *cpu, Bus *bus) {
}
void op_dec_hl(CPU *cpu, Bus *bus) {
    cpu->HL.full--;
    cpu->cycles += 8;
}
void op_inc_l(CPU *cpu, Bus *bus) {
}
void op_dec_l(CPU *cpu, Bus *bus) {
}
void op_ld_l_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC);
    cpu->HL.bytes.lo = val;
    cpu->cycles += 8;
}
void op_cpl(CPU *cpu, Bus *bus) {
}

// 0x30 - 0x3F
void op_jr_nc_n8(CPU *cpu, Bus *bus) {
}
void op_ld_sp_n16(CPU *cpu, Bus *bus) {
}
void op_ld_hld_ind_a(CPU *cpu, Bus *bus) {
}
void op_inc_sp(CPU *cpu, Bus *bus) {
    cpu->SP++;
    cpu->cycles += 8;
}
void op_inc_hl_ind(CPU *cpu, Bus *bus) {
}
void op_dec_hl_ind(CPU *cpu, Bus *bus) {
}
void op_ld_hl_ind_n8(CPU *cpu, Bus *bus) {
}
void op_scf(CPU *cpu, Bus *bus) {
}
void op_jr_c_n8(CPU *cpu, Bus *bus) {
}
void op_add_hl_sp(CPU *cpu, Bus *bus) {
}
void op_ld_a_hld_ind(CPU *cpu, Bus *bus) {
}
void op_dec_sp(CPU *cpu, Bus *bus) {
    cpu->SP--;
    cpu->cycles += 8;
}
void op_inc_a(CPU *cpu, Bus *bus) {
}
void op_dec_a(CPU *cpu, Bus *bus) {
}
void op_ld_a_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC);
    cpu->AF.bytes.lo = val;
    cpu->cycles += 8;
}
void op_ccf(CPU *cpu, Bus *bus) {
}

// 0x40 - 0x7F (LD r1, r2)
void op_ld_b_b(CPU *cpu, Bus *bus) {
    cpu->cycles += 4;
}
void op_ld_b_c(CPU *cpu, Bus *bus) {
    cpu->BC.bytes.hi = cpu->BC.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_b_d(CPU *cpu, Bus *bus) {
    cpu->BC.bytes.hi = cpu->DE.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_b_e(CPU *cpu, Bus *bus) {
    cpu->BC.bytes.hi = cpu->DE.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_b_h(CPU *cpu, Bus *bus) {
    cpu->BC.bytes.hi = cpu->HL.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_b_l(CPU *cpu, Bus *bus) {
    cpu->BC.bytes.hi = cpu->HL.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_b_hl_ind(CPU *cpu, Bus *bus) {
}
void op_ld_b_a(CPU *cpu, Bus *bus) {
    cpu->BC.bytes.hi = cpu->AF.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_c_b(CPU *cpu, Bus *bus) {
    cpu->BC.bytes.lo = cpu->BC.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_c_c(CPU *cpu, Bus *bus) {
    cpu->cycles += 4;
}
void op_ld_c_d(CPU *cpu, Bus *bus) {
    cpu->BC.bytes.lo = cpu->DE.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_c_e(CPU *cpu, Bus *bus) {
    cpu->BC.bytes.lo = cpu->DE.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_c_h(CPU *cpu, Bus *bus) {
    cpu->BC.bytes.lo = cpu->HL.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_c_l(CPU *cpu, Bus *bus) {
    cpu->BC.bytes.lo = cpu->HL.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_c_hl_ind(CPU *cpu, Bus *bus) {
}
void op_ld_c_a(CPU *cpu, Bus *bus) {
    cpu->BC.bytes.lo = cpu->AF.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_d_b(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.hi = cpu->BC.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_d_c(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.hi = cpu->BC.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_d_d(CPU *cpu, Bus *bus) {
    cpu->cycles += 4;
}
void op_ld_d_e(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.hi = cpu->DE.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_d_h(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.hi = cpu->HL.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_d_l(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.hi = cpu->HL.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_d_hl_ind(CPU *cpu, Bus *bus) {
}
void op_ld_d_a(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.lo = cpu->AF.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_e_b(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.lo = cpu->BC.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_e_c(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.lo = cpu->BC.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_e_d(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.lo = cpu->DE.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_e_e(CPU *cpu, Bus *bus) {
    cpu->cycles += 4;
}
void op_ld_e_h(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.lo = cpu->HL.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_e_l(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.lo = cpu->HL.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_e_hl_ind(CPU *cpu, Bus *bus) {
}
void op_ld_e_a(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.lo = cpu->AF.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_h_b(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.hi = cpu->BC.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_h_c(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.hi = cpu->BC.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_h_d(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.hi = cpu->DE.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_h_e(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.hi = cpu->DE.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_h_h(CPU *cpu, Bus *bus) {
    cpu->cycles += 4;
}
void op_ld_h_l(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.hi = cpu->HL.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_h_hl_ind(CPU *cpu, Bus *bus) {
}
void op_ld_h_a(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.hi = cpu->AF.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_l_b(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.lo = cpu->BC.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_l_c(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.lo = cpu->BC.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_l_d(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.lo = cpu->DE.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_l_e(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.lo = cpu->DE.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_l_h(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.lo = cpu->HL.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_l_l(CPU *cpu, Bus *bus) {
    cpu->cycles += 4;
}
void op_ld_l_hl_ind(CPU *cpu, Bus *bus) {
}
void op_ld_l_a(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.lo = cpu->AF.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_hl_ind_b(CPU *cpu, Bus *bus) {
}
void op_ld_hl_ind_c(CPU *cpu, Bus *bus) {
}
void op_ld_hl_ind_d(CPU *cpu, Bus *bus) {
}
void op_ld_hl_ind_e(CPU *cpu, Bus *bus) {
}
void op_ld_hl_ind_h(CPU *cpu, Bus *bus) {
}
void op_ld_hl_ind_l(CPU *cpu, Bus *bus) {
}
void op_halt(CPU *cpu, Bus *bus) {
    cpu->halted = true;
    cpu->cycles += 4;
}
void op_ld_hl_ind_a(CPU *cpu, Bus *bus) {
}
void op_ld_a_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu->BC.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_a_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu->BC.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_a_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu->DE.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_a_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu->DE.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_a_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu->HL.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_a_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu->HL.bytes.lo;
    cpu->cycles += 4;
}
void op_ld_a_hl_ind(CPU *cpu, Bus *bus) {
}
void op_ld_a_a(CPU *cpu, Bus *bus) {
    cpu->cycles += 4;
}

// 0x80 - 0xBF (ALU)
void op_add_a_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, 
            cpu->AF.bytes.hi, 
            cpu->BC.bytes.hi,
            ALU_ADD);
    cpu->cycles += 4;
}
void op_add_a_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, 
            cpu->AF.bytes.hi, 
            cpu->BC.bytes.lo,
            ALU_ADD);
    cpu->cycles += 4;
}
void op_add_a_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, 
            cpu->AF.bytes.hi, 
            cpu->DE.bytes.hi,
            ALU_ADD);
    cpu->cycles += 4;
}
void op_add_a_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, 
            cpu->AF.bytes.hi, 
            cpu->DE.bytesi.lo,
            ALU_ADD);
    cpu->cycles += 4;
}
void op_add_a_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, 
            cpu->AF.bytes.hi, 
            cpu->HL.bytes.hi,
            ALU_ADD);
    cpu->cycles += 4;
}
void op_add_a_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, 
            cpu->AF.bytes.hi, 
            cpu->HL.bytes.lo,
            ALU_ADD);
    cpu->cycles += 4;
}
void op_add_a_hl_ind(CPU *cpu, Bus *bus) {

}
void op_add_a_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, 
            cpu->AF.bytes.hi, 
            cpu->AF.bytes.hi,
            ALU_ADD);
    cpu->cycles += 4;
}
void op_adc_a_b(CPU *cpu, Bus *bus) {
}
void op_adc_a_c(CPU *cpu, Bus *bus) {
}
void op_adc_a_d(CPU *cpu, Bus *bus) {
}
void op_adc_a_e(CPU *cpu, Bus *bus) {
}
void op_adc_a_h(CPU *cpu, Bus *bus) {
}
void op_adc_a_l(CPU *cpu, Bus *bus) {
}
void op_adc_a_hl_ind(CPU *cpu, Bus *bus) {
}
void op_adc_a_a(CPU *cpu, Bus *bus) {
}
void op_sub_a_b(CPU *cpu, Bus *bus) {
}
void op_sub_a_c(CPU *cpu, Bus *bus) {
}
void op_sub_a_d(CPU *cpu, Bus *bus) {
}
void op_sub_a_e(CPU *cpu, Bus *bus) {
}
void op_sub_a_h(CPU *cpu, Bus *bus) {
}
void op_sub_a_l(CPU *cpu, Bus *bus) {
}
void op_sub_a_hl_ind(CPU *cpu, Bus *bus) {
}
void op_sub_a_a(CPU *cpu, Bus *bus) {
}
void op_sbc_a_b(CPU *cpu, Bus *bus) {
}
void op_sbc_a_c(CPU *cpu, Bus *bus) {
}
void op_sbc_a_d(CPU *cpu, Bus *bus) {
}
void op_sbc_a_e(CPU *cpu, Bus *bus) {
}
void op_sbc_a_h(CPU *cpu, Bus *bus) {
}
void op_sbc_a_l(CPU *cpu, Bus *bus) {
}
void op_sbc_a_hl_ind(CPU *cpu, Bus *bus) {
}
void op_sbc_a_a(CPU *cpu, Bus *bus) {
}
void op_and_a_b(CPU *cpu, Bus *bus) {
}
void op_and_a_c(CPU *cpu, Bus *bus) {
}
void op_and_a_d(CPU *cpu, Bus *bus) {
}
void op_and_a_e(CPU *cpu, Bus *bus) {
}
void op_and_a_h(CPU *cpu, Bus *bus) {
}
void op_and_a_l(CPU *cpu, Bus *bus) {
}
void op_and_a_hl_ind(CPU *cpu, Bus *bus) {
}
void op_and_a_a(CPU *cpu, Bus *bus) {
}
void op_xor_a_b(CPU *cpu, Bus *bus) {
}
void op_xor_a_c(CPU *cpu, Bus *bus) {
}
void op_xor_a_d(CPU *cpu, Bus *bus) {
}
void op_xor_a_e(CPU *cpu, Bus *bus) {
}
void op_xor_a_h(CPU *cpu, Bus *bus) {
}
void op_xor_a_l(CPU *cpu, Bus *bus) {
}
void op_xor_a_hl_ind(CPU *cpu, Bus *bus) {
}
void op_xor_a_a(CPU *cpu, Bus *bus) {
}
void op_or_a_b(CPU *cpu, Bus *bus) {
}
void op_or_a_c(CPU *cpu, Bus *bus) {
}
void op_or_a_d(CPU *cpu, Bus *bus) {
}
void op_or_a_e(CPU *cpu, Bus *bus) {
}
void op_or_a_h(CPU *cpu, Bus *bus) {
}
void op_or_a_l(CPU *cpu, Bus *bus) {
}
void op_or_a_hl_ind(CPU *cpu, Bus *bus) {
}
void op_or_a_a(CPU *cpu, Bus *bus) {
}
void op_cp_a_b(CPU *cpu, Bus *bus) {
}
void op_cp_a_c(CPU *cpu, Bus *bus) {
}
void op_cp_a_d(CPU *cpu, Bus *bus) {
}
void op_cp_a_e(CPU *cpu, Bus *bus) {
}
void op_cp_a_h(CPU *cpu, Bus *bus) {
}
void op_cp_a_l(CPU *cpu, Bus *bus) {
}
void op_cp_a_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cp_a_a(CPU *cpu, Bus *bus) {
}

// 0xC0 - 0xFF
void op_ret_nz(CPU *cpu, Bus *bus) {
}
void op_pop_bc(CPU *cpu, Bus *bus) {
}
void op_jp_nz_n16(CPU *cpu, Bus *bus) {
}
void op_jp_n16(CPU *cpu, Bus *bus) {
}
void op_call_nz_n16(CPU *cpu, Bus *bus) {
}
void op_push_bc(CPU *cpu, Bus *bus) {
}
void op_add_a_n8(CPU *cpu, Bus *bus) {
}
void op_rst_00(CPU *cpu, Bus *bus) {
}
void op_ret_z(CPU *cpu, Bus *bus) {
}
void op_ret(CPU *cpu, Bus *bus) {
}
void op_jp_z_n16(CPU *cpu, Bus *bus) {
}
void op_call_z_n16(CPU *cpu, Bus *bus) {
}
void op_call_n16(CPU *cpu, Bus *bus) {
}
void op_adc_a_n8(CPU *cpu, Bus *bus) {
}
void op_rst_08(CPU *cpu, Bus *bus) {
}
void op_ret_nc(CPU *cpu, Bus *bus) {
}
void op_pop_de(CPU *cpu, Bus *bus) {
}
void op_jp_nc_n16(CPU *cpu, Bus *bus) {
}
void op_call_nc_n16(CPU *cpu, Bus *bus) {
}
void op_push_de(CPU *cpu, Bus *bus) {
}
void op_sub_a_n8(CPU *cpu, Bus *bus) {
}
void op_rst_10(CPU *cpu, Bus *bus) {
}
void op_ret_c(CPU *cpu, Bus *bus) {
}
void op_reti(CPU *cpu, Bus *bus) {
}
void op_jp_c_n16(CPU *cpu, Bus *bus) {
}
void op_call_c_n16(CPU *cpu, Bus *bus) {
}
void op_sbc_a_n8(CPU *cpu, Bus *bus) {
}
void op_rst_18(CPU *cpu, Bus *bus) {
}
void op_ldh_n8_ind_a(CPU *cpu, Bus *bus) {
}
void op_pop_hl(CPU *cpu, Bus *bus) {
}
void op_ldh_c_ind_a(CPU *cpu, Bus *bus) {
}
void op_push_hl(CPU *cpu, Bus *bus) {
}
void op_and_a_n8(CPU *cpu, Bus *bus) {
}
void op_rst_20(CPU *cpu, Bus *bus) {
}
void op_add_sp_n8(CPU *cpu, Bus *bus) {
}
void op_jp_hl(CPU *cpu, Bus *bus) {
}
void op_ld_n16_ind_a(CPU *cpu, Bus *bus) {
}
void op_xor_a_n8(CPU *cpu, Bus *bus) {
}
void op_rst_28(CPU *cpu, Bus *bus) {
}
void op_ldh_a_n8_ind(CPU *cpu, Bus *bus) {
}
void op_pop_af(CPU *cpu, Bus *bus) {
}
void op_ldh_a_c_ind(CPU *cpu, Bus *bus) {
}
void op_di(CPU *cpu, Bus *bus) {
}
void op_push_af(CPU *cpu, Bus *bus) {
}
void op_or_a_n8(CPU *cpu, Bus *bus) {
}
void op_rst_30(CPU *cpu, Bus *bus) {
}
void op_ld_hl_sp_n8(CPU *cpu, Bus *bus) {
}
void op_ld_sp_hl(CPU *cpu, Bus *bus) {
}
void op_ld_a_n16_ind(CPU *cpu, Bus *bus) {
}
void op_ei(CPU *cpu, Bus *bus) {
}
void op_cp_a_n8(CPU *cpu, Bus *bus) {
}
void op_rst_38(CPU *cpu, Bus *bus) {
}

// Prefisso CB
void op_cb_rlc_b(CPU *cpu, Bus *bus) {
}
void op_cb_rlc_c(CPU *cpu, Bus *bus) {
}
void op_cb_rlc_d(CPU *cpu, Bus *bus) {
}
void op_cb_rlc_e(CPU *cpu, Bus *bus) {
}
void op_cb_rlc_h(CPU *cpu, Bus *bus) {
}
void op_cb_rlc_l(CPU *cpu, Bus *bus) {
}
void op_cb_rlc_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_rlc_a(CPU *cpu, Bus *bus) {
}
void op_cb_rrc_b(CPU *cpu, Bus *bus) {
}
void op_cb_rrc_c(CPU *cpu, Bus *bus) {
}
void op_cb_rrc_d(CPU *cpu, Bus *bus) {
}
void op_cb_rrc_e(CPU *cpu, Bus *bus) {
}
void op_cb_rrc_h(CPU *cpu, Bus *bus) {
}
void op_cb_rrc_l(CPU *cpu, Bus *bus) {
}
void op_cb_rrc_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_rrc_a(CPU *cpu, Bus *bus) {
}
void op_cb_rl_b(CPU *cpu, Bus *bus) {
}
void op_cb_rl_c(CPU *cpu, Bus *bus) {
}
void op_cb_rl_d(CPU *cpu, Bus *bus) {
}
void op_cb_rl_e(CPU *cpu, Bus *bus) {
}
void op_cb_rl_h(CPU *cpu, Bus *bus) {
}
void op_cb_rl_l(CPU *cpu, Bus *bus) {
}
void op_cb_rl_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_rl_a(CPU *cpu, Bus *bus) {
}
void op_cb_rr_b(CPU *cpu, Bus *bus) {
}
void op_cb_rr_c(CPU *cpu, Bus *bus) {
}
void op_cb_rr_d(CPU *cpu, Bus *bus) {
}
void op_cb_rr_e(CPU *cpu, Bus *bus) {
}
void op_cb_rr_h(CPU *cpu, Bus *bus) {
}
void op_cb_rr_l(CPU *cpu, Bus *bus) {
}
void op_cb_rr_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_rr_a(CPU *cpu, Bus *bus) {
}
void op_cb_sla_b(CPU *cpu, Bus *bus) {
}
void op_cb_sla_c(CPU *cpu, Bus *bus) {
}
void op_cb_sla_d(CPU *cpu, Bus *bus) {
}
void op_cb_sla_e(CPU *cpu, Bus *bus) {
}
void op_cb_sla_h(CPU *cpu, Bus *bus) {
}
void op_cb_sla_l(CPU *cpu, Bus *bus) {
}
void op_cb_sla_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_sla_a(CPU *cpu, Bus *bus) {
}
void op_cb_sra_b(CPU *cpu, Bus *bus) {
}
void op_cb_sra_c(CPU *cpu, Bus *bus) {
}
void op_cb_sra_d(CPU *cpu, Bus *bus) {
}
void op_cb_sra_e(CPU *cpu, Bus *bus) {
}
void op_cb_sra_h(CPU *cpu, Bus *bus) {
}
void op_cb_sra_l(CPU *cpu, Bus *bus) {
}
void op_cb_sra_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_sra_a(CPU *cpu, Bus *bus) {
}
void op_cb_swap_b(CPU *cpu, Bus *bus) {
}
void op_cb_swap_c(CPU *cpu, Bus *bus) {
}
void op_cb_swap_d(CPU *cpu, Bus *bus) {
}
void op_cb_swap_e(CPU *cpu, Bus *bus) {
}
void op_cb_swap_h(CPU *cpu, Bus *bus) {
}
void op_cb_swap_l(CPU *cpu, Bus *bus) {
}
void op_cb_swap_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_swap_a(CPU *cpu, Bus *bus) {
}
void op_cb_srl_b(CPU *cpu, Bus *bus) {
}
void op_cb_srl_c(CPU *cpu, Bus *bus) {
}
void op_cb_srl_d(CPU *cpu, Bus *bus) {
}
void op_cb_srl_e(CPU *cpu, Bus *bus) {
}
void op_cb_srl_h(CPU *cpu, Bus *bus) {
}
void op_cb_srl_l(CPU *cpu, Bus *bus) {
}
void op_cb_srl_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_srl_a(CPU *cpu, Bus *bus) {
}
void op_cb_bit_0_b(CPU *cpu, Bus *bus) {
}
void op_cb_bit_0_c(CPU *cpu, Bus *bus) {
}
void op_cb_bit_0_d(CPU *cpu, Bus *bus) {
}
void op_cb_bit_0_e(CPU *cpu, Bus *bus) {
}
void op_cb_bit_0_h(CPU *cpu, Bus *bus) {
}
void op_cb_bit_0_l(CPU *cpu, Bus *bus) {
}
void op_cb_bit_0_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_bit_0_a(CPU *cpu, Bus *bus) {
}
void op_cb_bit_1_b(CPU *cpu, Bus *bus) {
}
void op_cb_bit_1_c(CPU *cpu, Bus *bus) {
}
void op_cb_bit_1_d(CPU *cpu, Bus *bus) {
}
void op_cb_bit_1_e(CPU *cpu, Bus *bus) {
}
void op_cb_bit_1_h(CPU *cpu, Bus *bus) {
}
void op_cb_bit_1_l(CPU *cpu, Bus *bus) {
}
void op_cb_bit_1_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_bit_1_a(CPU *cpu, Bus *bus) {
}
void op_cb_bit_2_b(CPU *cpu, Bus *bus) {
}
void op_cb_bit_2_c(CPU *cpu, Bus *bus) {
}
void op_cb_bit_2_d(CPU *cpu, Bus *bus) {
}
void op_cb_bit_2_e(CPU *cpu, Bus *bus) {
}
void op_cb_bit_2_h(CPU *cpu, Bus *bus) {
}
void op_cb_bit_2_l(CPU *cpu, Bus *bus) {
}
void op_cb_bit_2_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_bit_2_a(CPU *cpu, Bus *bus) {
}
void op_cb_bit_3_b(CPU *cpu, Bus *bus) {
}
void op_cb_bit_3_c(CPU *cpu, Bus *bus) {
}
void op_cb_bit_3_d(CPU *cpu, Bus *bus) {
}
void op_cb_bit_3_e(CPU *cpu, Bus *bus) {
}
void op_cb_bit_3_h(CPU *cpu, Bus *bus) {
}
void op_cb_bit_3_l(CPU *cpu, Bus *bus) {
}
void op_cb_bit_3_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_bit_3_a(CPU *cpu, Bus *bus) {
}
void op_cb_bit_4_b(CPU *cpu, Bus *bus) {
}
void op_cb_bit_4_c(CPU *cpu, Bus *bus) {
}
void op_cb_bit_4_d(CPU *cpu, Bus *bus) {
}
void op_cb_bit_4_e(CPU *cpu, Bus *bus) {
}
void op_cb_bit_4_h(CPU *cpu, Bus *bus) {
}
void op_cb_bit_4_l(CPU *cpu, Bus *bus) {
}
void op_cb_bit_4_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_bit_4_a(CPU *cpu, Bus *bus) {
}
void op_cb_bit_5_b(CPU *cpu, Bus *bus) {
}
void op_cb_bit_5_c(CPU *cpu, Bus *bus) {
}
void op_cb_bit_5_d(CPU *cpu, Bus *bus) {
}
void op_cb_bit_5_e(CPU *cpu, Bus *bus) {
}
void op_cb_bit_5_h(CPU *cpu, Bus *bus) {
}
void op_cb_bit_5_l(CPU *cpu, Bus *bus) {
}
void op_cb_bit_5_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_bit_5_a(CPU *cpu, Bus *bus) {
}
void op_cb_bit_6_b(CPU *cpu, Bus *bus) {
}
void op_cb_bit_6_c(CPU *cpu, Bus *bus) {
}
void op_cb_bit_6_d(CPU *cpu, Bus *bus) {
}
void op_cb_bit_6_e(CPU *cpu, Bus *bus) {
}
void op_cb_bit_6_h(CPU *cpu, Bus *bus) {
}
void op_cb_bit_6_l(CPU *cpu, Bus *bus) {
}
void op_cb_bit_6_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_bit_6_a(CPU *cpu, Bus *bus) {
}
void op_cb_bit_7_b(CPU *cpu, Bus *bus) {
}
void op_cb_bit_7_c(CPU *cpu, Bus *bus) {
}
void op_cb_bit_7_d(CPU *cpu, Bus *bus) {
}
void op_cb_bit_7_e(CPU *cpu, Bus *bus) {
}
void op_cb_bit_7_h(CPU *cpu, Bus *bus) {
}
void op_cb_bit_7_l(CPU *cpu, Bus *bus) {
}
void op_cb_bit_7_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_bit_7_a(CPU *cpu, Bus *bus) {
}
void op_cb_res_0_b(CPU *cpu, Bus *bus) {
}
void op_cb_res_0_c(CPU *cpu, Bus *bus) {
}
void op_cb_res_0_d(CPU *cpu, Bus *bus) {
}
void op_cb_res_0_e(CPU *cpu, Bus *bus) {
}
void op_cb_res_0_h(CPU *cpu, Bus *bus) {
}
void op_cb_res_0_l(CPU *cpu, Bus *bus) {
}
void op_cb_res_0_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_res_0_a(CPU *cpu, Bus *bus) {
}
void op_cb_res_1_b(CPU *cpu, Bus *bus) {
}
void op_cb_res_1_c(CPU *cpu, Bus *bus) {
}
void op_cb_res_1_d(CPU *cpu, Bus *bus) {
}
void op_cb_res_1_e(CPU *cpu, Bus *bus) {
}
void op_cb_res_1_h(CPU *cpu, Bus *bus) {
}
void op_cb_res_1_l(CPU *cpu, Bus *bus) {
}
void op_cb_res_1_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_res_1_a(CPU *cpu, Bus *bus) {
}
void op_cb_res_2_b(CPU *cpu, Bus *bus) {
}
void op_cb_res_2_c(CPU *cpu, Bus *bus) {
}
void op_cb_res_2_d(CPU *cpu, Bus *bus) {
}
void op_cb_res_2_e(CPU *cpu, Bus *bus) {
}
void op_cb_res_2_h(CPU *cpu, Bus *bus) {
}
void op_cb_res_2_l(CPU *cpu, Bus *bus) {
}
void op_cb_res_2_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_res_2_a(CPU *cpu, Bus *bus) {
}
void op_cb_res_3_b(CPU *cpu, Bus *bus) {
}
void op_cb_res_3_c(CPU *cpu, Bus *bus) {
}
void op_cb_res_3_d(CPU *cpu, Bus *bus) {
}
void op_cb_res_3_e(CPU *cpu, Bus *bus) {
}
void op_cb_res_3_h(CPU *cpu, Bus *bus) {
}
void op_cb_res_3_l(CPU *cpu, Bus *bus) {
}
void op_cb_res_3_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_res_3_a(CPU *cpu, Bus *bus) {
}
void op_cb_res_4_b(CPU *cpu, Bus *bus) {
}
void op_cb_res_4_c(CPU *cpu, Bus *bus) {
}
void op_cb_res_4_d(CPU *cpu, Bus *bus) {
}
void op_cb_res_4_e(CPU *cpu, Bus *bus) {
}
void op_cb_res_4_h(CPU *cpu, Bus *bus) {
}
void op_cb_res_4_l(CPU *cpu, Bus *bus) {
}
void op_cb_res_4_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_res_4_a(CPU *cpu, Bus *bus) {
}
void op_cb_res_5_b(CPU *cpu, Bus *bus) {
}
void op_cb_res_5_c(CPU *cpu, Bus *bus) {
}
void op_cb_res_5_d(CPU *cpu, Bus *bus) {
}
void op_cb_res_5_e(CPU *cpu, Bus *bus) {
}
void op_cb_res_5_h(CPU *cpu, Bus *bus) {
}
void op_cb_res_5_l(CPU *cpu, Bus *bus) {
}
void op_cb_res_5_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_res_5_a(CPU *cpu, Bus *bus) {
}
void op_cb_res_6_b(CPU *cpu, Bus *bus) {
}
void op_cb_res_6_c(CPU *cpu, Bus *bus) {
}
void op_cb_res_6_d(CPU *cpu, Bus *bus) {
}
void op_cb_res_6_e(CPU *cpu, Bus *bus) {
}
void op_cb_res_6_h(CPU *cpu, Bus *bus) {
}
void op_cb_res_6_l(CPU *cpu, Bus *bus) {
}
void op_cb_res_6_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_res_6_a(CPU *cpu, Bus *bus) {
}
void op_cb_res_7_b(CPU *cpu, Bus *bus) {
}
void op_cb_res_7_c(CPU *cpu, Bus *bus) {
}
void op_cb_res_7_d(CPU *cpu, Bus *bus) {
}
void op_cb_res_7_e(CPU *cpu, Bus *bus) {
}
void op_cb_res_7_h(CPU *cpu, Bus *bus) {
}
void op_cb_res_7_l(CPU *cpu, Bus *bus) {
}
void op_cb_res_7_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_res_7_a(CPU *cpu, Bus *bus) {
}
void op_cb_set_0_b(CPU *cpu, Bus *bus) {
}
void op_cb_set_0_c(CPU *cpu, Bus *bus) {
}
void op_cb_set_0_d(CPU *cpu, Bus *bus) {
}
void op_cb_set_0_e(CPU *cpu, Bus *bus) {
}
void op_cb_set_0_h(CPU *cpu, Bus *bus) {
}
void op_cb_set_0_l(CPU *cpu, Bus *bus) {
}
void op_cb_set_0_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_set_0_a(CPU *cpu, Bus *bus) {
}
void op_cb_set_1_b(CPU *cpu, Bus *bus) {
}
void op_cb_set_1_c(CPU *cpu, Bus *bus) {
}
void op_cb_set_1_d(CPU *cpu, Bus *bus) {
}
void op_cb_set_1_e(CPU *cpu, Bus *bus) {
}
void op_cb_set_1_h(CPU *cpu, Bus *bus) {
}
void op_cb_set_1_l(CPU *cpu, Bus *bus) {
}
void op_cb_set_1_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_set_1_a(CPU *cpu, Bus *bus) {
}
void op_cb_set_2_b(CPU *cpu, Bus *bus) {
}
void op_cb_set_2_c(CPU *cpu, Bus *bus) {
}
void op_cb_set_2_d(CPU *cpu, Bus *bus) {
}
void op_cb_set_2_e(CPU *cpu, Bus *bus) {
}
void op_cb_set_2_h(CPU *cpu, Bus *bus) {
}
void op_cb_set_2_l(CPU *cpu, Bus *bus) {
}
void op_cb_set_2_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_set_2_a(CPU *cpu, Bus *bus) {
}
void op_cb_set_3_b(CPU *cpu, Bus *bus) {
}
void op_cb_set_3_c(CPU *cpu, Bus *bus) {
}
void op_cb_set_3_d(CPU *cpu, Bus *bus) {
}
void op_cb_set_3_e(CPU *cpu, Bus *bus) {
}
void op_cb_set_3_h(CPU *cpu, Bus *bus) {
}
void op_cb_set_3_l(CPU *cpu, Bus *bus) {
}
void op_cb_set_3_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_set_3_a(CPU *cpu, Bus *bus) {
}
void op_cb_set_4_b(CPU *cpu, Bus *bus) {
}
void op_cb_set_4_c(CPU *cpu, Bus *bus) {
}
void op_cb_set_4_d(CPU *cpu, Bus *bus) {
}
void op_cb_set_4_e(CPU *cpu, Bus *bus) {
}
void op_cb_set_4_h(CPU *cpu, Bus *bus) {
}
void op_cb_set_4_l(CPU *cpu, Bus *bus) {
}
void op_cb_set_4_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_set_4_a(CPU *cpu, Bus *bus) {
}
void op_cb_set_5_b(CPU *cpu, Bus *bus) {
}
void op_cb_set_5_c(CPU *cpu, Bus *bus) {
}
void op_cb_set_5_d(CPU *cpu, Bus *bus) {
}
void op_cb_set_5_e(CPU *cpu, Bus *bus) {
}
void op_cb_set_5_h(CPU *cpu, Bus *bus) {
}
void op_cb_set_5_l(CPU *cpu, Bus *bus) {
}
void op_cb_set_5_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_set_5_a(CPU *cpu, Bus *bus) {
}
void op_cb_set_6_b(CPU *cpu, Bus *bus) {
}
void op_cb_set_6_c(CPU *cpu, Bus *bus) {
}
void op_cb_set_6_d(CPU *cpu, Bus *bus) {
}
void op_cb_set_6_e(CPU *cpu, Bus *bus) {
}
void op_cb_set_6_h(CPU *cpu, Bus *bus) {
}
void op_cb_set_6_l(CPU *cpu, Bus *bus) {
}
void op_cb_set_6_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_set_6_a(CPU *cpu, Bus *bus) {
}
void op_cb_set_7_b(CPU *cpu, Bus *bus) {
}
void op_cb_set_7_c(CPU *cpu, Bus *bus) {
}
void op_cb_set_7_d(CPU *cpu, Bus *bus) {
}
void op_cb_set_7_e(CPU *cpu, Bus *bus) {
}
void op_cb_set_7_h(CPU *cpu, Bus *bus) {
}
void op_cb_set_7_l(CPU *cpu, Bus *bus) {
}
void op_cb_set_7_hl_ind(CPU *cpu, Bus *bus) {
}
void op_cb_set_7_a(CPU *cpu, Bus *bus) {
}

const Instruction instructions[256] = {
    [0x00] = op_nop,
    [0x01] = op_ld_bc_n16,
    [0x02] = op_ld_bc_ind_a,
};

const Instruction instructions_cb[256] = {
    // da riempire
};
