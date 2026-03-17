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
    cpu_write(cpu, bus, cpu->BC.full, cpu->AF.bytes.hi);
    cpu->cycles += 8;
}
void op_inc_bc(CPU *cpu, Bus *bus){
    cpu->BC.full++;
    cpu->cycles += 8;
}
void op_inc_b(CPU *cpu, Bus *bus){
    cpu->BC.bytes.hi = cpu_alu(cpu, cpu->BC.bytes.hi, 0, ALU_INC);
    cpu->cycles += 4;
}
void op_dec_b(CPU *cpu, Bus *bus) {
    cpu->BC.bytes.hi = cpu_alu(cpu, cpu->BC.bytes.hi, 0, ALU_DEC);
    cpu->cycles += 4;
}
void op_ld_b_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu->BC.bytes.hi = val;
    cpu->cycles += 8;
}
void op_rlca(CPU *cpu, Bus *bus) {
}
void op_ld_n16_ind_sp(CPU *cpu, Bus *bus) {
    u16 address = cpu_read_word(cpu, bus);
    cpu_write_word(cpu, bus, address, cpu->SP);
    cpu->cycles += 20;
}
void op_add_hl_bc(CPU *cpu, Bus *bus) {
}
void op_ld_a_bc_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = bus_read(bus, cpu->BC.full);
    cpu->cycles += 8;
}
void op_dec_bc(CPU *cpu, Bus *bus) {
    cpu->BC.full--;
    cpu->cycles += 8;
}
void op_inc_c(CPU *cpu, Bus *bus) {
    cpu->BC.bytes.lo = cpu_alu(cpu, cpu->BC.bytes.lo, 0, ALU_INC);
    cpu->cycles += 4;
}
void op_dec_c(CPU *cpu, Bus *bus) {
    cpu->BC.bytes.lo = cpu_alu(cpu, cpu->BC.bytes.lo, 0, ALU_DEC);
    cpu->cycles += 4;
}
void op_ld_c_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu->BC.bytes.lo = val;
    cpu->cycles += 8;
}
void op_rrca(CPU *cpu, Bus *bus) {
}

// 0x10 - 0x1F
void op_stop(CPU *cpu, Bus *bus) {
    // Leggo a vuoto (deve contenere 0)
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    if(val != 0) return;
    cpu->stopped = true;
    cpu->cycles += 4;
}
void op_ld_de_n16(CPU *cpu, Bus *bus) {
    cpu->DE.full = cpu_read_word(cpu, bus);
    cpu->cycles += 12;
}
void op_ld_de_ind_a(CPU *cpu, Bus *bus) {
    cpu_write(cpu, bus, cpu->DE.full, cpu->AF.bytes.hi);
    cpu->cycles += 8;
}
void op_inc_de(CPU *cpu, Bus *bus) {
    cpu->DE.full++;
    cpu->cycles += 8;
}
void op_inc_d(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.hi = cpu_alu(cpu, cpu->DE.bytes.hi, 0, ALU_INC);
    cpu->cycles += 4;
}
void op_dec_d(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.hi = cpu_alu(cpu, cpu->DE.bytes.hi, 0, ALU_DEC);
    cpu->cycles += 4;
}
void op_ld_d_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu->DE.bytes.hi = val;
    cpu->cycles += 8;
}
void op_rla(CPU *cpu, Bus *bus) {
}
void op_jr_n8(CPU *cpu, Bus *bus) {
    i8 val = (i8)cpu_read(cpu, bus, cpu->PC++);
    cpu->PC += val;
    cpu->cycles += 12;
}
void op_add_hl_de(CPU *cpu, Bus *bus) {
}
void op_ld_a_de_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = bus_read(bus, cpu->DE.full);
    cpu->cycles += 8;
}
void op_dec_de(CPU *cpu, Bus *bus) {
    cpu->DE.full--;
    cpu->cycles += 8;
}
void op_inc_e(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.lo = cpu_alu(cpu, cpu->DE.bytes.lo, 0, ALU_INC);
    cpu->cycles += 4;
}
void op_dec_e(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.lo = cpu_alu(cpu, cpu->DE.bytes.lo, 0, ALU_DEC);
    cpu->cycles += 4;
}
void op_ld_e_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu->DE.bytes.lo = val;
    cpu->cycles += 8;
}
void op_rra(CPU *cpu, Bus *bus) {
}

// 0x20 - 0x2F
void op_jr_nz_n8(CPU *cpu, Bus *bus) {
    i8 val = (i8)cpu_read(cpu, bus, cpu->PC++);
    if (!(cpu->AF.bytes.lo & FLAG_Z)){
        cpu->cycles += 12;
        cpu->PC += val; 
    } else {
        cpu->cycles += 8;
    }
}
void op_ld_hl_n16(CPU *cpu, Bus *bus) {
    cpu->HL.full = cpu_read_word(cpu, bus);
    cpu->cycles += 12;
}
void op_ld_hli_ind_a(CPU *cpu, Bus *bus) {
    cpu_write(cpu, bus, cpu->HL.full, cpu->AF.bytes.hi);
    cpu->HL.full++;
    cpu->cycles += 8;
}
void op_inc_hl(CPU *cpu, Bus *bus) {
    cpu->HL.full++;
    cpu->cycles += 8;
}
void op_inc_h(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.hi = cpu_alu(cpu, cpu->HL.bytes.hi, 0, ALU_INC);
    cpu->cycles += 4;
}
void op_dec_h(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.hi = cpu_alu(cpu, cpu->HL.bytes.hi, 0, ALU_DEC);
    cpu->cycles += 4;
}
void op_ld_h_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu->HL.bytes.hi = val;
    cpu->cycles += 8;
}
void op_daa(CPU *cpu, Bus *bus) {
}
void op_jr_z_n8(CPU *cpu, Bus *bus) {
    i8 val = (i8)cpu_read(cpu, bus, cpu->PC++);
    if ((cpu->AF.bytes.lo & FLAG_Z)){
        cpu->cycles += 12;
        cpu->PC += val; 
    } else {
        cpu->cycles += 8;
    }
}
void op_add_hl_hl(CPU *cpu, Bus *bus) {
}
void op_ld_a_hli_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = bus_read(bus, cpu->HL.full);
    cpu->HL.full++;
    cpu->cycles += 8;
}
void op_dec_hl(CPU *cpu, Bus *bus) {
    cpu->HL.full--;
    cpu->cycles += 8;
}
void op_inc_l(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.lo = cpu_alu(cpu, cpu->HL.bytes.lo, 0, ALU_INC);
    cpu->cycles += 4;
}
void op_dec_l(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.lo = cpu_alu(cpu, cpu->HL.bytes.lo, 0, ALU_DEC);
    cpu->cycles += 4;
}
void op_ld_l_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu->HL.bytes.lo = val;
    cpu->cycles += 8;
}
void op_cpl(CPU *cpu, Bus *bus) {
}

// 0x30 - 0x3F
void op_jr_nc_n8(CPU *cpu, Bus *bus) {
    i8 val = (i8)cpu_read(cpu, bus, cpu->PC++);
    if (!(cpu->AF.bytes.lo & FLAG_C)){
        cpu->cycles += 12;
        cpu->PC += val; 
    } else {
        cpu->cycles += 8;
    }
}
void op_ld_sp_n16(CPU *cpu, Bus *bus) {
    cpu->SP = cpu_read_word(cpu, bus);
    cpu->cycles += 12;
}
void op_ld_hld_ind_a(CPU *cpu, Bus *bus) {
    cpu_write(cpu, bus, cpu->HL.full, cpu->AF.bytes.hi);
    cpu->HL.full--;
    cpu->cycles += 8;
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
    i8 val = (i8)cpu_read(cpu, bus, cpu->PC++);
    if ((cpu->AF.bytes.lo & FLAG_C)){
        cpu->cycles += 12;
        cpu->PC += val; 
    } else {
        cpu->cycles += 8;
    }
}
void op_add_hl_sp(CPU *cpu, Bus *bus) {
}
void op_ld_a_hld_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = bus_read(bus, cpu->HL.full);
    cpu->HL.full--;
    cpu->cycles += 8;
}
void op_dec_sp(CPU *cpu, Bus *bus) {
    cpu->SP--;
    cpu->cycles += 8;
}
void op_inc_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, 0, ALU_INC);
    cpu->cycles += 4;
}
void op_dec_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, 0, ALU_DEC);
    cpu->cycles += 4;
}
void op_ld_a_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu->AF.bytes.hi = val;
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
    cpu->BC.bytes.hi = bus_read(bus, cpu->HL.full);
    cpu->cycles += 8;
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
    cpu->BC.bytes.lo = bus_read(bus, cpu->HL.full);
    cpu->cycles += 8;
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
    cpu->DE.bytes.hi = bus_read(bus, cpu->HL.full);
    cpu->cycles += 8;
}
void op_ld_d_a(CPU *cpu, Bus *bus) {
    cpu->DE.bytes.hi = cpu->AF.bytes.hi;
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
    cpu->DE.bytes.lo = bus_read(bus, cpu->HL.full);
    cpu->cycles += 8;
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
    cpu->HL.bytes.hi = bus_read(bus, cpu->HL.full);
    cpu->cycles += 8;
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
    cpu->HL.bytes.lo = bus_read(bus, cpu->HL.full);
    cpu->cycles += 8;
}
void op_ld_l_a(CPU *cpu, Bus *bus) {
    cpu->HL.bytes.lo = cpu->AF.bytes.hi;
    cpu->cycles += 4;
}
void op_ld_hl_ind_b(CPU *cpu, Bus *bus) {
    cpu_write(cpu, bus, cpu->HL.full, cpu->BC.bytes.hi);   
    cpu->cycles += 8;
}
void op_ld_hl_ind_c(CPU *cpu, Bus *bus) {
    cpu_write(cpu, bus, cpu->HL.full, cpu->BC.bytes.lo);   
    cpu->cycles += 8;
}
void op_ld_hl_ind_d(CPU *cpu, Bus *bus) {
    cpu_write(cpu, bus, cpu->HL.full, cpu->DE.bytes.hi);   
    cpu->cycles += 8;
}
void op_ld_hl_ind_e(CPU *cpu, Bus *bus) {
    cpu_write(cpu, bus, cpu->HL.full, cpu->DE.bytes.lo);   
    cpu->cycles += 8;
}
void op_ld_hl_ind_h(CPU *cpu, Bus *bus) {
    cpu_write(cpu, bus, cpu->HL.full, cpu->HL.bytes.hi);   
    cpu->cycles += 8;
}
void op_ld_hl_ind_l(CPU *cpu, Bus *bus) {
    cpu_write(cpu, bus, cpu->HL.full, cpu->HL.bytes.lo);   
    cpu->cycles += 8;
}
void op_halt(CPU *cpu, Bus *bus) {
    cpu->halted = true;
    cpu->cycles += 4;
}
void op_ld_hl_ind_a(CPU *cpu, Bus *bus) {
    cpu_write(cpu, bus, cpu->HL.full, cpu->AF.bytes.hi);   
    cpu->cycles += 8;
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
    cpu->AF.bytes.hi = bus_read(bus, cpu->HL.full);
    cpu->cycles += 8;
}
void op_ld_a_a(CPU *cpu, Bus *bus) {
    cpu->cycles += 4;
}

// 0x80 - 0xBF (ALU)
// --- ADD A,r ---
void op_add_a_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->BC.bytes.hi, ALU_ADD);
    cpu->cycles += 4;
}
void op_add_a_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->BC.bytes.lo, ALU_ADD);
    cpu->cycles += 4;
}
void op_add_a_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->DE.bytes.hi, ALU_ADD);
    cpu->cycles += 4;
}
void op_add_a_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->DE.bytes.lo, ALU_ADD);
    cpu->cycles += 4;
}
void op_add_a_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->HL.bytes.hi, ALU_ADD);
    cpu->cycles += 4;
}
void op_add_a_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->HL.bytes.lo, ALU_ADD);
    cpu->cycles += 4;
}
void op_add_a_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = bus_read(bus, cpu->HL.full);
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, val, ALU_ADD);
    cpu->cycles += 8;

}
void op_add_a_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->AF.bytes.hi, ALU_ADD);
    cpu->cycles += 4;
}

// --- ADC A,r ---
void op_adc_a_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->BC.bytes.hi, ALU_ADC);
    cpu->cycles += 4;
}
void op_adc_a_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->BC.bytes.lo, ALU_ADC);
    cpu->cycles += 4;
}
void op_adc_a_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->DE.bytes.hi, ALU_ADC);
    cpu->cycles += 4;
}
void op_adc_a_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->DE.bytes.lo, ALU_ADC);
    cpu->cycles += 4;
}
void op_adc_a_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->HL.bytes.hi, ALU_ADC);
    cpu->cycles += 4;
}
void op_adc_a_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->HL.bytes.lo, ALU_ADC);
    cpu->cycles += 4;
}
void op_adc_a_hl_ind(CPU *cpu, Bus *bus) {

}
void op_adc_a_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->AF.bytes.hi, ALU_ADC);
    cpu->cycles += 4;
}

// --- SUB A,r ---
void op_sub_a_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->BC.bytes.hi, ALU_SUB);
    cpu->cycles += 4;
}
void op_sub_a_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->BC.bytes.lo, ALU_SUB);
    cpu->cycles += 4;
}
void op_sub_a_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->DE.bytes.hi, ALU_SUB);
    cpu->cycles += 4;
}
void op_sub_a_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->DE.bytes.lo, ALU_SUB);
    cpu->cycles += 4;
}
void op_sub_a_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->HL.bytes.hi, ALU_SUB);
    cpu->cycles += 4;
}
void op_sub_a_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->HL.bytes.lo, ALU_SUB);
    cpu->cycles += 4;
}
void op_sub_a_hl_ind(CPU *cpu, Bus *bus) {

}
void op_sub_a_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->AF.bytes.hi, ALU_SUB);
    cpu->cycles += 4;
}

// --- SBC A,r ---
void op_sbc_a_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->BC.bytes.hi, ALU_SBC);
    cpu->cycles += 4;
}
void op_sbc_a_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->BC.bytes.lo, ALU_SBC);
    cpu->cycles += 4;
}
void op_sbc_a_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->DE.bytes.hi, ALU_SBC);
    cpu->cycles += 4;
}
void op_sbc_a_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->DE.bytes.lo, ALU_SBC);
    cpu->cycles += 4;
}
void op_sbc_a_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->HL.bytes.hi, ALU_SBC);
    cpu->cycles += 4;
}
void op_sbc_a_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->HL.bytes.lo, ALU_SBC);
    cpu->cycles += 4;
}
void op_sbc_a_hl_ind(CPU *cpu, Bus *bus) {

}
void op_sbc_a_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->AF.bytes.hi, ALU_SBC);
    cpu->cycles += 4;
}

// --- AND A,r ---
void op_and_a_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->BC.bytes.hi, ALU_AND);
    cpu->cycles += 4;
}
void op_and_a_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->BC.bytes.lo, ALU_AND);
    cpu->cycles += 4;
}
void op_and_a_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->DE.bytes.hi, ALU_AND);
    cpu->cycles += 4;
}
void op_and_a_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->DE.bytes.lo, ALU_AND);
    cpu->cycles += 4;
}
void op_and_a_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->HL.bytes.hi, ALU_AND);
    cpu->cycles += 4;
}
void op_and_a_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->HL.bytes.lo, ALU_AND);
    cpu->cycles += 4;
}
void op_and_a_hl_ind(CPU *cpu, Bus *bus) {

}
void op_and_a_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->AF.bytes.hi, ALU_AND);
    cpu->cycles += 4;
}

// --- OR A,r ---
void op_or_a_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->BC.bytes.hi, ALU_OR);
    cpu->cycles += 4;
}
void op_or_a_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->BC.bytes.lo, ALU_OR);
    cpu->cycles += 4;
}
void op_or_a_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->DE.bytes.hi, ALU_OR);
    cpu->cycles += 4;
}
void op_or_a_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->DE.bytes.lo, ALU_OR);
    cpu->cycles += 4;
}
void op_or_a_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->HL.bytes.hi, ALU_OR);
    cpu->cycles += 4;
}
void op_or_a_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->HL.bytes.lo, ALU_OR);
    cpu->cycles += 4;
}
void op_or_a_hl_ind(CPU *cpu, Bus *bus) {

}
void op_or_a_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->AF.bytes.hi, ALU_OR);
    cpu->cycles += 4;
}

// --- XOR A,r ---
void op_xor_a_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->BC.bytes.hi, ALU_XOR);
    cpu->cycles += 4;
}
void op_xor_a_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->BC.bytes.lo, ALU_XOR);
    cpu->cycles += 4;
}
void op_xor_a_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->DE.bytes.hi, ALU_XOR);
    cpu->cycles += 4;
}
void op_xor_a_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->DE.bytes.lo, ALU_XOR);
    cpu->cycles += 4;
}
void op_xor_a_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->HL.bytes.hi, ALU_XOR);
    cpu->cycles += 4;
}
void op_xor_a_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->HL.bytes.lo, ALU_XOR);
    cpu->cycles += 4;
}
void op_xor_a_hl_ind(CPU *cpu, Bus *bus) {

}
void op_xor_a_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, cpu->AF.bytes.hi, ALU_XOR);
    cpu->cycles += 4;
}

// --- CP A,r ---
void op_cp_a_b(CPU *cpu, Bus *bus) {
    cpu_alu(cpu, cpu->AF.bytes.hi, cpu->BC.bytes.hi, ALU_CP);
    cpu->cycles += 4;
}
void op_cp_a_c(CPU *cpu, Bus *bus) {
    cpu_alu(cpu, cpu->AF.bytes.hi, cpu->BC.bytes.lo, ALU_CP);
    cpu->cycles += 4;
}
void op_cp_a_d(CPU *cpu, Bus *bus) {
    cpu_alu(cpu, cpu->AF.bytes.hi, cpu->DE.bytes.hi, ALU_CP);
    cpu->cycles += 4;
}
void op_cp_a_e(CPU *cpu, Bus *bus) {
    cpu_alu(cpu, cpu->AF.bytes.hi, cpu->DE.bytes.lo, ALU_CP);
    cpu->cycles += 4;
}
void op_cp_a_h(CPU *cpu, Bus *bus) {
    cpu_alu(cpu, cpu->AF.bytes.hi, cpu->HL.bytes.hi, ALU_CP);
    cpu->cycles += 4;
}
void op_cp_a_l(CPU *cpu, Bus *bus) {
    cpu_alu(cpu, cpu->AF.bytes.hi, cpu->HL.bytes.lo, ALU_CP);
    cpu->cycles += 4;
}
void op_cp_a_hl_ind(CPU *cpu, Bus *bus) {

}
void op_cp_a_a(CPU *cpu, Bus *bus) {
    cpu_alu(cpu, cpu->AF.bytes.hi, cpu->AF.bytes.hi, ALU_CP);
    cpu->cycles += 4;
}

// 0xC0 - 0xFF
void op_ret_nz(CPU *cpu, Bus *bus) {
    if(!(cpu->AF.bytes.lo & FLAG_Z)){
        u8 lo = bus_read(bus, cpu->SP++);
        u8 hi = bus_read(bus, cpu->SP++);
        cpu->PC = (hi << 8) | lo; 
        cpu->cycles += 20;
    } else {
        cpu->cycles += 8;    
    }
}
void op_pop_bc(CPU *cpu, Bus *bus) {
    u8 lo = bus_read(bus, cpu->SP++);
    u8 hi = bus_read(bus, cpu->SP++);
    cpu->BC.full = (hi << 8) | lo;
    cpu->cycles += 12;
}
void op_jp_nz_n16(CPU *cpu, Bus *bus) {
    u16 address = cpu_read_word(cpu, bus);
    if(!(cpu->AF.bytes.lo & FLAG_Z)){
        cpu->PC = address;
        cpu->cycles += 16;
    } else {
        cpu->cycles += 12;
    }
}
void op_jp_n16(CPU *cpu, Bus *bus) {
    u16 address = cpu_read_word(cpu, bus);
    cpu->PC = address;
    cpu->cycles += 16;
}
void op_call_nz_n16(CPU *cpu, Bus *bus) {
    u16 address = cpu_read_word(cpu, bus);
    if(!(cpu->AF.bytes.lo & FLAG_Z)){
        cpu->SP -= 2;
        cpu_write_word(cpu, bus, cpu->SP, cpu->PC);
        cpu->PC = address;
    
        cpu->cycles += 24;
    } else cpu->cycles += 12;
}
void op_push_bc(CPU *cpu, Bus *bus) {
    cpu->SP -= 2;
    cpu_write_word(cpu, bus, cpu->SP, cpu->BC.full);
    cpu->cycles += 16;
}
void op_add_a_n8(CPU *cpu, Bus *bus) {
}
void op_rst_00(CPU *cpu, Bus *bus) {
}
void op_ret_z(CPU *cpu, Bus *bus) {
    if((cpu->AF.bytes.lo & FLAG_Z)){
        u8 lo = bus_read(bus, cpu->SP++);
        u8 hi = bus_read(bus, cpu->SP++);
        cpu->PC = (hi << 8) | lo; 
        cpu->cycles += 20;
    } else {
        cpu->cycles += 8;    
    }
}
void op_ret(CPU *cpu, Bus *bus) {
    u8 lo = bus_read(bus, cpu->SP++);
    u8 hi = bus_read(bus, cpu->SP++);
    cpu->PC = (hi << 8) | lo; 
    cpu->cycles += 16;
}
void op_jp_z_n16(CPU *cpu, Bus *bus) {
    u16 address = cpu_read_word(cpu, bus);
    if((cpu->AF.bytes.lo & FLAG_Z)){
        cpu->PC = address;
        cpu->cycles += 16;
    } else {
        cpu->cycles += 12;
    }
}
void op_call_z_n16(CPU *cpu, Bus *bus) {
    u16 address = cpu_read_word(cpu, bus);
    if((cpu->AF.bytes.lo & FLAG_Z)){
        cpu->SP -= 2;
        cpu_write_word(cpu, bus, cpu->SP, cpu->PC);
        cpu->PC = address;
    
        cpu->cycles += 24;
    } else cpu->cycles += 12; 
}
void op_call_n16(CPU *cpu, Bus *bus) {
    u16 address = cpu_read_word(cpu, bus);
     
    cpu->SP -= 2;
    cpu_write_word(cpu, bus, cpu->SP, cpu->PC);

    cpu->PC = address;
    cpu->cycles += 24;
}
void op_adc_a_n8(CPU *cpu, Bus *bus) {
}
void op_rst_08(CPU *cpu, Bus *bus) {
}
void op_ret_nc(CPU *cpu, Bus *bus) {
    if(!(cpu->AF.bytes.lo & FLAG_C)){
        u8 lo = bus_read(bus, cpu->SP++);
        u8 hi = bus_read(bus, cpu->SP++);
        cpu->PC = (hi << 8) | lo; 
        cpu->cycles += 20;
    } else {
        cpu->cycles += 8;    
    }
}
void op_pop_de(CPU *cpu, Bus *bus) {
    u8 lo = bus_read(bus, cpu->SP++);
    u8 hi = bus_read(bus, cpu->SP++);
    cpu->DE.full = (hi << 8) | lo;
    cpu->cycles += 12;
}
void op_jp_nc_n16(CPU *cpu, Bus *bus) {
    u16 address = cpu_read_word(cpu, bus);
    if(!(cpu->AF.bytes.lo & FLAG_C)){
        cpu->PC = address;
        cpu->cycles += 16;
    } else {
        cpu->cycles += 12;
    }
}
void op_call_nc_n16(CPU *cpu, Bus *bus) {
    u16 address = cpu_read_word(cpu, bus);
    if(!(cpu->AF.bytes.lo & FLAG_C)){
        cpu->SP -= 2;
        cpu_write_word(cpu, bus, cpu->SP, cpu->PC);
        cpu->PC = address;
    
        cpu->cycles += 24;
    } else cpu->cycles += 12;
}
void op_push_de(CPU *cpu, Bus *bus) {
    cpu->SP -= 2;
    cpu_write_word(cpu, bus, cpu->SP, cpu->DE.full);
    cpu->cycles += 16;
}
void op_sub_a_n8(CPU *cpu, Bus *bus) {
}
void op_rst_10(CPU *cpu, Bus *bus) {
}
void op_ret_c(CPU *cpu, Bus *bus) {
    if((cpu->AF.bytes.lo & FLAG_C)){
        u8 lo = bus_read(bus, cpu->SP++);
        u8 hi = bus_read(bus, cpu->SP++);
        cpu->PC = (hi << 8) | lo; 
        cpu->cycles += 20;
    } else {
        cpu->cycles += 8;    
    }
}
void op_reti(CPU *cpu, Bus *bus) {
    u8 lo = bus_read(bus, cpu->SP++);
    u8 hi = bus_read(bus, cpu->SP++);
    cpu->PC = (hi << 8) | lo;
    cpu->IME = true; 
    cpu->cycles += 16;
}
void op_jp_c_n16(CPU *cpu, Bus *bus) {
    u16 address = cpu_read_word(cpu, bus);
    if((cpu->AF.bytes.lo & FLAG_C)){
        cpu->PC = address;
        cpu->cycles += 16;
    } else {
        cpu->cycles += 12;
    }
}
void op_call_c_n16(CPU *cpu, Bus *bus) {
    u16 address = cpu_read_word(cpu, bus);
    if((cpu->AF.bytes.lo & FLAG_C)){
        cpu->SP -= 2;
        cpu_write_word(cpu, bus, cpu->SP, cpu->PC);
        cpu->PC = address;
    
        cpu->cycles += 24;
    } else cpu->cycles += 12;
}
void op_sbc_a_n8(CPU *cpu, Bus *bus) {
}
void op_rst_18(CPU *cpu, Bus *bus) {
}
void op_ldh_n8_ind_a(CPU *cpu, Bus *bus) {
}
void op_pop_hl(CPU *cpu, Bus *bus) {
    u8 lo = bus_read(bus, cpu->SP++);
    u8 hi = bus_read(bus, cpu->SP++);
    cpu->HL.full = (hi << 8) | lo;
    cpu->cycles += 12;
}
void op_ldh_c_ind_a(CPU *cpu, Bus *bus) {
}
void op_push_hl(CPU *cpu, Bus *bus) {
    cpu->SP -= 2;
    cpu_write_word(cpu, bus, cpu->SP, cpu->HL.full);
    cpu->cycles += 16;
}
void op_and_a_n8(CPU *cpu, Bus *bus) {
}
void op_rst_20(CPU *cpu, Bus *bus) {
}
void op_add_sp_n8(CPU *cpu, Bus *bus) {
}
void op_jp_hl(CPU *cpu, Bus *bus) {
    cpu->PC = cpu->HL.full;
    cpu->cycles += 4;
}
void op_ld_n16_ind_a(CPU *cpu, Bus *bus) {
    u16 address = cpu_read_word(cpu, bus);
    cpu_write(cpu, bus, address, cpu->AF.bytes.hi); 
    cpu->cycles += 16;
}
void op_xor_a_n8(CPU *cpu, Bus *bus) {
}
void op_rst_28(CPU *cpu, Bus *bus) {
}
void op_ldh_a_n8_ind(CPU *cpu, Bus *bus) {
}
void op_pop_af(CPU *cpu, Bus *bus) {
    u8 lo = bus_read(bus, cpu->SP++);
    u8 hi = bus_read(bus, cpu->SP++);
    cpu->AF.full = ((hi << 8) | lo) & 0xFFF0;
    cpu->cycles += 12;
}
void op_ldh_a_c_ind(CPU *cpu, Bus *bus) {
}
void op_di(CPU *cpu, Bus *bus) {
}
void op_push_af(CPU *cpu, Bus *bus) {
    cpu->SP -= 2;
    cpu_write_word(cpu, bus, cpu->SP, cpu->AF.full);
    cpu->cycles += 16;
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
    u16 address = cpu_read_word(cpu, bus);
    u8 val = bus_read(bus, address);
    cpu->AF.bytes.hi = val;
    cpu->cycles += 16;
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
    [0x03] = op_inc_bc,
    [0x04] = op_inc_b,
    [0x05] = op_dec_b,
    [0x06] = op_ld_b_n8,
    [0x08] = op_ld_n16_ind_sp,
    [0x0A] = op_ld_a_bc_ind,
    [0x0B] = op_dec_bc,
    [0x0C] = op_inc_c,
    [0x0D] = op_dec_c,
    [0x0E] = op_ld_c_n8,
    [0x11] = op_ld_de_n16,
    [0x12] = op_ld_de_ind_a,
    [0x13] = op_inc_de,
    [0x14] = op_inc_d,
    [0x15] = op_dec_d,
    [0x16] = op_ld_d_n8,
    [0x18] = op_jr_n8,
    [0x1A] = op_ld_a_de_ind,
    [0x1B] = op_dec_de,
    [0x1C] = op_inc_e,
    [0x1D] = op_dec_e,
    [0x1E] = op_ld_e_n8,
    [0x20] = op_jr_nz_n8,
    [0x21] = op_ld_hl_n16,
    [0x22] = op_ld_hli_ind_a,
    [0x23] = op_inc_hl,
    [0x24] = op_inc_h,
    [0x25] = op_dec_h,
    [0x26] = op_ld_h_n8,
    [0x28] = op_jr_z_n8,
    [0x2A] = op_ld_a_hli_ind,
    [0x2B] = op_dec_hl,
    [0x2C] = op_inc_l,
    [0x2D] = op_dec_l,
    [0x2E] = op_ld_l_n8,
    [0x30] = op_jr_nc_n8,
    [0x31] = op_ld_sp_n16,
    [0x32] = op_ld_hld_ind_a,
    [0x33] = op_inc_sp,
    [0x38] = op_jr_c_n8,
    [0x3A] = op_ld_a_hld_ind,
    [0x3B] = op_dec_sp,
    [0x3C] = op_inc_a,
    [0x3D] = op_dec_a,
    [0x3E] = op_ld_a_n8,
    [0x40] = op_ld_b_b,
    [0x41] = op_ld_b_c,
    [0x42] = op_ld_b_d,
    [0x43] = op_ld_b_e,
    [0x44] = op_ld_b_h,
    [0x45] = op_ld_b_l,
    [0x46] = op_ld_b_hl_ind,
    [0x47] = op_ld_b_a,
    [0x48] = op_ld_c_b,
    [0x49] = op_ld_c_c,
    [0x4A] = op_ld_c_d,
    [0x4B] = op_ld_c_e,
    [0x4C] = op_ld_c_h,
    [0x4D] = op_ld_c_l,
    [0x4E] = op_ld_c_hl_ind,
    [0x4F] = op_ld_c_a,
    [0x50] = op_ld_d_b,
    [0x51] = op_ld_d_c,
    [0x52] = op_ld_d_d,
    [0x53] = op_ld_d_e,
    [0x54] = op_ld_d_h,
    [0x55] = op_ld_d_l,
    [0x56] = op_ld_d_hl_ind,
    [0x57] = op_ld_d_a,
    [0x58] = op_ld_e_b,
    [0x59] = op_ld_e_c,
    [0x5A] = op_ld_e_d,
    [0x5B] = op_ld_e_e,
    [0x5C] = op_ld_e_h,
    [0x5D] = op_ld_e_l,
    [0x5E] = op_ld_e_hl_ind,
    [0x5F] = op_ld_e_a,
    [0x60] = op_ld_h_b,
    [0x61] = op_ld_h_c,
    [0x62] = op_ld_h_d,
    [0x63] = op_ld_h_e,
    [0x64] = op_ld_h_h,
    [0x65] = op_ld_h_l,
    [0x66] = op_ld_h_hl_ind,
    [0x67] = op_ld_h_a,
    [0x68] = op_ld_l_b,
    [0x69] = op_ld_l_c,
    [0x6A] = op_ld_l_d,
    [0x6B] = op_ld_l_e,
    [0x6C] = op_ld_l_h,
    [0x6D] = op_ld_l_l,
    [0x6E] = op_ld_l_hl_ind,
    [0x6F] = op_ld_l_a,
    [0x70] = op_ld_hl_ind_b,
    [0x71] = op_ld_hl_ind_c,
    [0x72] = op_ld_hl_ind_d,
    [0x73] = op_ld_hl_ind_e,
    [0x74] = op_ld_hl_ind_h,
    [0x75] = op_ld_hl_ind_l,
    [0x76] = op_halt,
    [0x77] = op_ld_hl_ind_a,
    [0x78] = op_ld_a_b,
    [0x79] = op_ld_a_c,
    [0x7A] = op_ld_a_d,
    [0x7B] = op_ld_a_e,
    [0x7C] = op_ld_a_h,
    [0x7D] = op_ld_a_l,
    [0x7E] = op_ld_a_hl_ind,
    [0x7F] = op_ld_a_a,
    [0x80] = op_add_a_b,
    [0x81] = op_add_a_c,
    [0x82] = op_add_a_d,
    [0x83] = op_add_a_e,
    [0x84] = op_add_a_h,
    [0x85] = op_add_a_l,
    [0x86] = op_add_a_hl_ind,
    [0x87] = op_add_a_a,
    [0x88] = op_adc_a_b,
    [0x89] = op_adc_a_c,
    [0x8A] = op_adc_a_d,
    [0x8B] = op_adc_a_e,
    [0x8C] = op_adc_a_h,
    [0x8D] = op_adc_a_l,
    [0x8F] = op_adc_a_a,
    [0x90] = op_sub_a_b,
    [0x91] = op_sub_a_c,
    [0x92] = op_sub_a_d,
    [0x93] = op_sub_a_e,
    [0x94] = op_sub_a_h,
    [0x95] = op_sub_a_l,
    [0x97] = op_sub_a_a,
    [0x98] = op_sbc_a_b,
    [0x99] = op_sbc_a_c,
    [0x9A] = op_sbc_a_d,
    [0x9B] = op_sbc_a_e,
    [0x9C] = op_sbc_a_h,
    [0x9D] = op_sbc_a_l,
    [0x9F] = op_sbc_a_a,
    [0xA0] = op_and_a_b,
    [0xA1] = op_and_a_c,
    [0xA2] = op_and_a_d,
    [0xA3] = op_and_a_e,
    [0xA4] = op_and_a_h,
    [0xA5] = op_and_a_l,
    [0xA7] = op_and_a_a,
    [0xA8] = op_xor_a_b,
    [0xA9] = op_xor_a_c,
    [0xAA] = op_xor_a_d,
    [0xAB] = op_xor_a_e,
    [0xAC] = op_xor_a_h,
    [0xAD] = op_xor_a_l,
    [0xAF] = op_xor_a_a,
    [0xB0] = op_or_a_b,
    [0xB1] = op_or_a_c,
    [0xB2] = op_or_a_d,
    [0xB3] = op_or_a_e,
    [0xB4] = op_or_a_h,
    [0xB5] = op_or_a_l,
    [0xB7] = op_or_a_a,
    [0xB8] = op_cp_a_b,
    [0xB9] = op_cp_a_c,
    [0xBA] = op_cp_a_d,
    [0xBB] = op_cp_a_e,
    [0xBC] = op_cp_a_h,
    [0xBD] = op_cp_a_l,
    [0xBF] = op_cp_a_a,
    [0xC0] = op_ret_nz,
    [0xC1] = op_pop_bc,
    [0xC2] = op_jp_nz_n16,
    [0xC3] = op_jp_n16,
    [0xC4] = op_call_nz_n16,
    [0xC5] = op_push_bc,
    [0xC8] = op_ret_z,
    [0xC9] = op_ret,
    [0xCA] = op_jp_z_n16,
    [0xCC] = op_call_z_n16,
    [0xCD] = op_call_n16,
    [0xD0] = op_ret_nc,
    [0xD1] = op_pop_de,
    [0xD2] = op_jp_nc_n16,
    [0xD4] = op_call_nc_n16,
    [0xD5] = op_push_de,
    [0xD8] = op_ret_c,
    [0xD9] = op_reti,
    [0xDA] = op_jp_c_n16,
    [0xDC] = op_call_c_n16,
    [0xE1] = op_pop_hl,
    [0xE2] = op_ldh_c_ind_a,
    [0xE5] = op_push_hl,
    [0xE9] = op_jp_hl,
    [0xEA] = op_ld_n16_ind_a,
    [0xF1] = op_pop_af,
    [0xF5] = op_push_af,
    [0xFA] = op_ld_a_n16_ind,
};

const Instruction instructions_cb[256] = {
    // da riempire
};
