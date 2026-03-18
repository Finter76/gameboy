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
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    
    u8 carry = cpu->AF.bytes.hi & 0x80 ? 1 : 0;
    cpu->AF.bytes.hi <<= 1;
    
    if(carry){
        cpu->AF.bytes.lo |= FLAG_C;
        cpu->AF.bytes.hi |= 0x01;
    } else{
        cpu->AF.bytes.lo &= ~FLAG_C;
        cpu->AF.bytes.hi &= ~0x01;
    }

    cpu->cycles += 4;
}
void op_ld_n16_ind_sp(CPU *cpu, Bus *bus) {
    u16 address = cpu_read_word(cpu, bus);
    cpu_write_word(cpu, bus, address, cpu->SP);
    cpu->cycles += 20;
}
void op_add_hl_bc(CPU *cpu, Bus *bus) {
    u32 result32 = (u32)cpu->HL.full + (u32)cpu->BC.full;
   
    cpu->AF.bytes.lo &= ~(FLAG_N | FLAG_H | FLAG_C);
    if (((cpu->HL.full & 0xFFF) + (cpu->BC.full & 0xFFF)) > 0xFFF) 
        cpu->AF.bytes.lo |= FLAG_H;
    if (result32 > 0xFFFF) cpu->AF.bytes.lo |= FLAG_C;

    cpu->HL.full = result32;
    cpu->cycles += 8;
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
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    
    u8 carry = cpu->AF.bytes.hi & 0x01 ? 1 : 0;
    cpu->AF.bytes.hi >>= 1;
    
    if(carry){
        cpu->AF.bytes.lo |= FLAG_C;
        cpu->AF.bytes.hi |= 0x80;
    } else{
        cpu->AF.bytes.lo &= ~FLAG_C;
        cpu->AF.bytes.hi &= ~0x80;
    }

    cpu->cycles += 4;
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
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    
    u8 carry = cpu->AF.bytes.hi & 0x80 ? 1 : 0;
    cpu->AF.bytes.hi <<= 1;

    if(cpu->AF.bytes.lo & FLAG_C) cpu->AF.bytes.hi |= 0x01;
        else cpu->AF.bytes.hi &= ~0x01;
    
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;

    cpu->cycles += 4;
}
void op_jr_n8(CPU *cpu, Bus *bus) {
    i8 val = (i8)cpu_read(cpu, bus, cpu->PC++);
    cpu->PC += val;
    cpu->cycles += 12;
}
void op_add_hl_de(CPU *cpu, Bus *bus) {
    u32 result32 = (u32)cpu->HL.full + (u32)cpu->DE.full;
   
    cpu->AF.bytes.lo &= ~(FLAG_N | FLAG_H | FLAG_C);
    if (((cpu->HL.full & 0xFFF) + (cpu->DE.full & 0xFFF)) > 0xFFF) 
        cpu->AF.bytes.lo |= FLAG_H;
    if (result32 > 0xFFFF) cpu->AF.bytes.lo |= FLAG_C;

    cpu->HL.full = result32;
    cpu->cycles += 8;
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
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    
    u8 carry = cpu->AF.bytes.hi & 0x01 ? 1 : 0;
    cpu->AF.bytes.hi >>= 1;

    if(cpu->AF.bytes.lo & FLAG_C) cpu->AF.bytes.hi |= 0x80;
        else cpu->AF.bytes.hi &= ~0x80;
    
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;

    cpu->cycles += 4;
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
    bool correct_low  = (cpu->AF.bytes.hi & 0x0F) > 9 
                        || (cpu->AF.bytes.lo & FLAG_H);
    bool correct_high = cpu->AF.bytes.hi > 0x99    
                        || (cpu->AF.bytes.lo & FLAG_C);

    // Ultima operazione addizione
    if(!(cpu->AF.bytes.lo & FLAG_N)){
        if(correct_low){
            cpu->AF.bytes.hi += 0x06;
        } if(correct_high){
            cpu->AF.bytes.hi += 0x60;
            cpu->AF.bytes.lo |= FLAG_C;
        }
    } else { // Ultima operazione sottrazione
        if(correct_low){
            cpu->AF.bytes.hi -= 0x06;
        } if(correct_high){
            cpu->AF.bytes.hi -= 0x60;
        } 
    }

    cpu->AF.bytes.lo &= ~FLAG_H;
    if(cpu->AF.bytes.hi == 0) cpu->AF.bytes.lo |= FLAG_Z;
    else cpu->AF.bytes.lo &= ~FLAG_Z; 

    cpu->cycles += 4;
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
    u32 result32 = (u32)cpu->HL.full + (u32)cpu->HL.full;
    
    cpu->AF.bytes.lo &= ~(FLAG_N | FLAG_H | FLAG_C);
    if (((cpu->HL.full & 0xFFF) + (cpu->HL.full & 0xFFF)) > 0xFFF) 
        cpu->AF.bytes.lo |= FLAG_H;
    if (result32 > 0xFFFF) cpu->AF.bytes.lo |= FLAG_C;

    cpu->HL.full = result32;
    cpu->cycles += 8;
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
    cpu->AF.bytes.hi = ~(cpu->AF.bytes.hi);
    cpu->AF.bytes.lo |= FLAG_N;
    cpu->AF.bytes.lo |= FLAG_H;
    cpu->cycles += 4;
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
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val = cpu_alu(cpu, val, 0, ALU_INC);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 12;
}
void op_dec_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val = cpu_alu(cpu, val, 0, ALU_DEC);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 12;
}
void op_ld_hl_ind_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 12;
}
void op_scf(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo |=  FLAG_C; 
    cpu->AF.bytes.lo &= ~FLAG_N; 
    cpu->AF.bytes.lo &= ~FLAG_H; 
    cpu->cycles += 4;
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
    u32 result32 = (u32)cpu->HL.full + (u32)cpu->SP;
    
    cpu->AF.bytes.lo &= ~(FLAG_N | FLAG_H | FLAG_C);
    if (((cpu->HL.full & 0xFFF) + (cpu->SP & 0xFFF)) > 0xFFF) 
        cpu->AF.bytes.lo |= FLAG_H;
    if (result32 > 0xFFFF) cpu->AF.bytes.lo |= FLAG_C;

    cpu->HL.full = result32;
    cpu->cycles += 8;
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
    cpu->AF.bytes.lo &= ~(FLAG_N | FLAG_H);
    cpu->AF.bytes.lo ^= FLAG_C; 
    cpu->cycles += 4;
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
    u8 val = bus_read(bus, cpu->HL.full);
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, val, ALU_ADC);
    cpu->cycles += 8;

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
    u8 val = bus_read(bus, cpu->HL.full);
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, val, ALU_SUB);
    cpu->cycles += 8;

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
    u8 val = bus_read(bus, cpu->HL.full);
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, val, ALU_SBC);
    cpu->cycles += 8;

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
    u8 val = bus_read(bus, cpu->HL.full);
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, val, ALU_AND);
    cpu->cycles += 8;

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
    u8 val = bus_read(bus, cpu->HL.full);
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, val, ALU_OR);
    cpu->cycles += 8;

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
    u8 val = bus_read(bus, cpu->HL.full);
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, val, ALU_XOR);
    cpu->cycles += 8; 
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
    u8 val = bus_read(bus, cpu->HL.full);
    cpu_alu(cpu, cpu->AF.bytes.hi, val, ALU_CP);
    cpu->cycles += 8;

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
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, val, ALU_ADD);
    cpu->cycles += 8;
}
void op_rst_00(CPU *cpu, Bus *bus) {
    cpu->SP -= 2;
    cpu_write_word(cpu, bus, cpu->SP, cpu->PC);
    cpu->PC = 0x0000; 
    cpu->cycles += 16;
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
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, val, ALU_ADC);
    cpu->cycles += 8;
}
void op_rst_08(CPU *cpu, Bus *bus) {
    cpu->SP -= 2;
    cpu_write_word(cpu, bus, cpu->SP, cpu->PC);
    cpu->PC = 0x0008; 
    cpu->cycles += 16;
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
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, val, ALU_SUB);
    cpu->cycles += 8;
}
void op_rst_10(CPU *cpu, Bus *bus) {
    cpu->SP -= 2;
    cpu_write_word(cpu, bus, cpu->SP, cpu->PC);
    cpu->PC = 0x0010; 
    cpu->cycles += 16;
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
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, val, ALU_SBC);
    cpu->cycles += 8;
}
void op_rst_18(CPU *cpu, Bus *bus) {
    cpu->SP -= 2;
    cpu_write_word(cpu, bus, cpu->SP, cpu->PC);
    cpu->PC = 0x0018; 
    cpu->cycles += 16;
}
void op_ldh_n8_ind_a(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu_write(cpu, bus, 0xFF00 + val, cpu->AF.bytes.hi);
    cpu->cycles += 12;
}
void op_pop_hl(CPU *cpu, Bus *bus) {
    u8 lo = bus_read(bus, cpu->SP++);
    u8 hi = bus_read(bus, cpu->SP++);
    cpu->HL.full = (hi << 8) | lo;
    cpu->cycles += 12;
}
void op_ldh_c_ind_a(CPU *cpu, Bus *bus) {
    cpu_write(cpu, bus, 0xFF00 + cpu->BC.bytes.lo, cpu->AF.bytes.hi);
    cpu->cycles += 12;
}
void op_push_hl(CPU *cpu, Bus *bus) {
    cpu->SP -= 2;
    cpu_write_word(cpu, bus, cpu->SP, cpu->HL.full);
    cpu->cycles += 16;
}
void op_and_a_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, val, ALU_AND);
    cpu->cycles += 8;
}
void op_rst_20(CPU *cpu, Bus *bus) {
    cpu->SP -= 2;
    cpu_write_word(cpu, bus, cpu->SP, cpu->PC);
    cpu->PC = 0x0020; 
    cpu->cycles += 16;
}
void op_add_sp_n8(CPU *cpu, Bus *bus) {
    i8 val = (i8)cpu_read(cpu, bus, cpu->PC++);
    u16 result = cpu->SP + val;

    cpu->AF.bytes.lo = 0;
    if (((cpu->SP ^ val ^ result) & 0x10)) 
        cpu->AF.bytes.lo |= FLAG_H;
    if (((cpu->SP ^ val ^ result) & 0x100)) 
        cpu->AF.bytes.lo |= FLAG_C;

    cpu->SP = result;
    cpu->cycles += 16;
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
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, val, ALU_XOR);
    cpu->cycles += 8;
}
void op_rst_28(CPU *cpu, Bus *bus) {
    cpu->SP -= 2;
    cpu_write_word(cpu, bus, cpu->SP, cpu->PC);
    cpu->PC = 0x0028; 
    cpu->cycles += 16;
}
void op_ldh_a_n8_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu->AF.bytes.hi = cpu_read(cpu, bus, 0xFF00 + val); 
    cpu->cycles += 12;
}
void op_pop_af(CPU *cpu, Bus *bus) {
    u8 lo = bus_read(bus, cpu->SP++);
    u8 hi = bus_read(bus, cpu->SP++);
    cpu->AF.full = ((hi << 8) | lo) & 0xFFF0;
    cpu->cycles += 12;
}
void op_ldh_a_c_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.hi = cpu_read(cpu, bus, 0xFF00 + cpu->BC.bytes.lo);
    cpu->cycles += 8;
}
void op_di(CPU *cpu, Bus *bus) {
    cpu->IME = false;
    cpu->cycles += 4;
}
void op_push_af(CPU *cpu, Bus *bus) {
    cpu->SP -= 2;
    cpu_write_word(cpu, bus, cpu->SP, cpu->AF.full);
    cpu->cycles += 16;
}
void op_or_a_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu->AF.bytes.hi = cpu_alu(cpu, cpu->AF.bytes.hi, val, ALU_OR);
    cpu->cycles += 8;
}
void op_rst_30(CPU *cpu, Bus *bus) {
    cpu->SP -= 2;
    cpu_write_word(cpu, bus, cpu->SP, cpu->PC);
    cpu->PC = 0x0030; 
    cpu->cycles += 16;
}
void op_ld_hl_sp_n8(CPU *cpu, Bus *bus) {
    i8 val = (i8)cpu_read(cpu, bus, cpu->PC++);
    u16 result = cpu->SP + val;

    cpu->AF.bytes.lo = 0;
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    if (((cpu->SP ^ val ^ result) & 0x10)) 
        cpu->AF.bytes.lo |= FLAG_H;
    if (((cpu->SP ^ val ^ result) & 0x100)) 
        cpu->AF.bytes.lo |= FLAG_C;

    cpu->HL.full = result;
    cpu->cycles += 12;    
}
void op_ld_sp_hl(CPU *cpu, Bus *bus) {
    cpu->SP = cpu->HL.full;
    cpu->cycles += 8;
}
void op_ld_a_n16_ind(CPU *cpu, Bus *bus) {
    u16 address = cpu_read_word(cpu, bus);
    u8 val = bus_read(bus, address);
    cpu->AF.bytes.hi = val;
    cpu->cycles += 16;
}
void op_ei(CPU *cpu, Bus *bus) {
    cpu->ime_pending = true;
    cpu->cycles += 4;
}
void op_cp_a_n8(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->PC++);
    cpu_alu(cpu, cpu->AF.bytes.hi, val, ALU_CP);
    cpu->cycles += 8;
}
void op_rst_38(CPU *cpu, Bus *bus) {
    cpu->SP -= 2;
    cpu_write_word(cpu, bus, cpu->SP, cpu->PC);
    cpu->PC = 0x0038; 
    cpu->cycles += 16;
}

// Prefisso CB
// ==================== RLC ====================
void op_cb_rlc_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->BC.bytes.hi & 0x80 ? 1 : 0;
    cpu->BC.bytes.hi <<= 1;
    if(carry) cpu->BC.bytes.hi |= 0x01;
        else cpu->BC.bytes.hi &= ~0x01;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->BC.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rlc_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->BC.bytes.lo & 0x80 ? 1 : 0;
    cpu->BC.bytes.lo <<= 1;
    if(carry) cpu->BC.bytes.lo |= 0x01;
        else cpu->BC.bytes.lo &= ~0x01;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->BC.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rlc_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->DE.bytes.hi & 0x80 ? 1 : 0;
    cpu->DE.bytes.hi <<= 1;
    if(carry) cpu->DE.bytes.hi |= 0x01;
        else cpu->DE.bytes.hi &= ~0x01;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->DE.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rlc_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->DE.bytes.lo & 0x80 ? 1 : 0;
    cpu->DE.bytes.lo <<= 1;
    if(carry) cpu->DE.bytes.lo |= 0x01;
        else cpu->DE.bytes.lo &= ~0x01;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->DE.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rlc_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->HL.bytes.hi & 0x80 ? 1 : 0;
    cpu->HL.bytes.hi <<= 1;
    if(carry) cpu->HL.bytes.hi |= 0x01;
        else cpu->HL.bytes.hi &= ~0x01;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->HL.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rlc_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->HL.bytes.lo & 0x80 ? 1 : 0;
    cpu->HL.bytes.lo <<= 1;
    if(carry) cpu->HL.bytes.lo |= 0x01;
        else cpu->HL.bytes.lo &= ~0x01;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->HL.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rlc_hl_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    u8 carry = val & 0x80 ? 1 : 0;
    val <<= 1;
    if(carry) val |= 0x01;
        else val &= ~0x01;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!val) cpu->AF.bytes.lo |= FLAG_Z;
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_rlc_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->AF.bytes.hi & 0x80 ? 1 : 0;
    cpu->AF.bytes.hi <<= 1;
    if(carry) cpu->AF.bytes.hi |= 0x01;
        else cpu->AF.bytes.hi &= ~0x01;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->AF.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}

// ==================== RRC ====================
void op_cb_rrc_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->BC.bytes.hi & 0x01 ? 1 : 0;
    cpu->BC.bytes.hi >>= 1;
    if(carry) cpu->BC.bytes.hi |= 0x80;
        else cpu->BC.bytes.hi &= ~0x80;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->BC.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rrc_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->BC.bytes.lo & 0x01 ? 1 : 0;
    cpu->BC.bytes.lo >>= 1;
    if(carry) cpu->BC.bytes.lo |= 0x80;
        else cpu->BC.bytes.lo &= ~0x80;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->BC.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rrc_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->DE.bytes.hi & 0x01 ? 1 : 0;
    cpu->DE.bytes.hi >>= 1;
    if(carry) cpu->DE.bytes.hi |= 0x80;
        else cpu->DE.bytes.hi &= ~0x80;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->DE.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rrc_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->DE.bytes.lo & 0x01 ? 1 : 0;
    cpu->DE.bytes.lo >>= 1;
    if(carry) cpu->DE.bytes.lo |= 0x80;
        else cpu->DE.bytes.lo &= ~0x80;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->DE.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rrc_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->HL.bytes.hi & 0x01 ? 1 : 0;
    cpu->HL.bytes.hi >>= 1;
    if(carry) cpu->HL.bytes.hi |= 0x80;
        else cpu->HL.bytes.hi &= ~0x80;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->HL.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rrc_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->HL.bytes.lo & 0x01 ? 1 : 0;
    cpu->HL.bytes.lo >>= 1;
    if(carry) cpu->HL.bytes.lo |= 0x80;
        else cpu->HL.bytes.lo &= ~0x80;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->HL.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rrc_hl_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    u8 carry = val & 0x01 ? 1 : 0;
    val >>= 1;
    if(carry) val |= 0x80;
        else val &= ~0x80;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!val) cpu->AF.bytes.lo |= FLAG_Z;
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_rrc_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->AF.bytes.hi & 0x01 ? 1 : 0;
    cpu->AF.bytes.hi >>= 1;
    if(carry) cpu->AF.bytes.hi |= 0x80;
        else cpu->AF.bytes.hi &= ~0x80;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->AF.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}

// ==================== RL ====================
void op_cb_rl_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->BC.bytes.hi & 0x80 ? 1 : 0;
    cpu->BC.bytes.hi <<= 1;
    if(cpu->AF.bytes.lo & FLAG_C) cpu->BC.bytes.hi |= 0x01;
        else cpu->BC.bytes.hi &= ~0x01;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->BC.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rl_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->BC.bytes.lo & 0x80 ? 1 : 0;
    cpu->BC.bytes.lo <<= 1;
    if(cpu->AF.bytes.lo & FLAG_C) cpu->BC.bytes.lo |= 0x01;
        else cpu->BC.bytes.lo &= ~0x01;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->BC.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rl_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->DE.bytes.hi & 0x80 ? 1 : 0;
    cpu->DE.bytes.hi <<= 1;
    if(cpu->AF.bytes.lo & FLAG_C) cpu->DE.bytes.hi |= 0x01;
        else cpu->DE.bytes.hi &= ~0x01;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->DE.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rl_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->DE.bytes.lo & 0x80 ? 1 : 0;
    cpu->DE.bytes.lo <<= 1;
    if(cpu->AF.bytes.lo & FLAG_C) cpu->DE.bytes.lo |= 0x01;
        else cpu->DE.bytes.lo &= ~0x01;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->DE.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rl_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->HL.bytes.hi & 0x80 ? 1 : 0;
    cpu->HL.bytes.hi <<= 1;
    if(cpu->AF.bytes.lo & FLAG_C) cpu->HL.bytes.hi |= 0x01;
        else cpu->HL.bytes.hi &= ~0x01;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->HL.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rl_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->HL.bytes.lo & 0x80 ? 1 : 0;
    cpu->HL.bytes.lo <<= 1;
    if(cpu->AF.bytes.lo & FLAG_C) cpu->HL.bytes.lo |= 0x01;
        else cpu->HL.bytes.lo &= ~0x01;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->HL.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rl_hl_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    u8 carry = val & 0x80 ? 1 : 0;
    val <<= 1;
    if(cpu->AF.bytes.lo & FLAG_C) val |= 0x01;
        else val &= ~0x01;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!val) cpu->AF.bytes.lo |= FLAG_Z;
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_rl_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->AF.bytes.hi & 0x80 ? 1 : 0;
    cpu->AF.bytes.hi <<= 1;
    if(cpu->AF.bytes.lo & FLAG_C) cpu->AF.bytes.hi |= 0x01;
        else cpu->AF.bytes.hi &= ~0x01;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->AF.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}

// ==================== RR ====================
void op_cb_rr_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->BC.bytes.hi & 0x01 ? 1 : 0;
    cpu->BC.bytes.hi >>= 1;
    if(cpu->AF.bytes.lo & FLAG_C) cpu->BC.bytes.hi |= 0x80;
        else cpu->BC.bytes.hi &= ~0x80;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->BC.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rr_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->BC.bytes.lo & 0x01 ? 1 : 0;
    cpu->BC.bytes.lo >>= 1;
    if(cpu->AF.bytes.lo & FLAG_C) cpu->BC.bytes.lo |= 0x80;
        else cpu->BC.bytes.lo &= ~0x80;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->BC.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rr_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->DE.bytes.hi & 0x01 ? 1 : 0;
    cpu->DE.bytes.hi >>= 1;
    if(cpu->AF.bytes.lo & FLAG_C) cpu->DE.bytes.hi |= 0x80;
        else cpu->DE.bytes.hi &= ~0x80;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->DE.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rr_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->DE.bytes.lo & 0x01 ? 1 : 0;
    cpu->DE.bytes.lo >>= 1;
    if(cpu->AF.bytes.lo & FLAG_C) cpu->DE.bytes.lo |= 0x80;
        else cpu->DE.bytes.lo &= ~0x80;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->DE.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rr_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->HL.bytes.hi & 0x01 ? 1 : 0;
    cpu->HL.bytes.hi >>= 1;
    if(cpu->AF.bytes.lo & FLAG_C) cpu->HL.bytes.hi |= 0x80;
        else cpu->HL.bytes.hi &= ~0x80;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->HL.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rr_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->HL.bytes.lo & 0x01 ? 1 : 0;
    cpu->HL.bytes.lo >>= 1;
    if(cpu->AF.bytes.lo & FLAG_C) cpu->HL.bytes.lo |= 0x80;
        else cpu->HL.bytes.lo &= ~0x80;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->HL.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_rr_hl_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    u8 carry = val & 0x01 ? 1 : 0;
    val >>= 1;
    if(cpu->AF.bytes.lo & FLAG_C) val |= 0x80;
        else val &= ~0x80;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!val) cpu->AF.bytes.lo |= FLAG_Z;
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_rr_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->AF.bytes.hi & 0x01 ? 1 : 0;
    cpu->AF.bytes.hi >>= 1;
    if(cpu->AF.bytes.lo & FLAG_C) cpu->AF.bytes.hi |= 0x80;
        else cpu->AF.bytes.hi &= ~0x80;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->AF.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
// ==================== SLA ====================
void op_cb_sla_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->BC.bytes.hi & 0x80 ? 1 : 0;
    cpu->BC.bytes.hi <<= 1;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->BC.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_sla_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->BC.bytes.lo & 0x80 ? 1 : 0;
    cpu->BC.bytes.lo <<= 1;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->BC.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_sla_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->DE.bytes.hi & 0x80 ? 1 : 0;
    cpu->DE.bytes.hi <<= 1;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->DE.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_sla_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->DE.bytes.lo & 0x80 ? 1 : 0;
    cpu->DE.bytes.lo <<= 1;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->DE.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_sla_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->HL.bytes.hi & 0x80 ? 1 : 0;
    cpu->HL.bytes.hi <<= 1;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->HL.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_sla_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->HL.bytes.lo & 0x80 ? 1 : 0;
    cpu->HL.bytes.lo <<= 1;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->HL.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_sla_hl_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    u8 carry = val & 0x80 ? 1 : 0;
    val <<= 1;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!val) cpu->AF.bytes.lo |= FLAG_Z;
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_sla_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->AF.bytes.hi & 0x80 ? 1 : 0;
    cpu->AF.bytes.hi <<= 1;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->AF.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
 
// ==================== SRA ====================
void op_cb_sra_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->BC.bytes.hi & 0x01 ? 1 : 0;
    u8 msb = cpu->BC.bytes.hi & 0x80;
    cpu->BC.bytes.hi >>= 1;
    cpu->BC.bytes.hi |= msb;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->BC.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_sra_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->BC.bytes.lo & 0x01 ? 1 : 0;
    u8 msb = cpu->BC.bytes.lo & 0x80;
    cpu->BC.bytes.lo >>= 1;
    cpu->BC.bytes.lo |= msb;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->BC.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_sra_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->DE.bytes.hi & 0x01 ? 1 : 0;
    u8 msb = cpu->DE.bytes.hi & 0x80;
    cpu->DE.bytes.hi >>= 1;
    cpu->DE.bytes.hi |= msb;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->DE.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_sra_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->DE.bytes.lo & 0x01 ? 1 : 0;
    u8 msb = cpu->DE.bytes.lo & 0x80;
    cpu->DE.bytes.lo >>= 1;
    cpu->DE.bytes.lo |= msb;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->DE.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_sra_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->HL.bytes.hi & 0x01 ? 1 : 0;
    u8 msb = cpu->HL.bytes.hi & 0x80;
    cpu->HL.bytes.hi >>= 1;
    cpu->HL.bytes.hi |= msb;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->HL.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_sra_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->HL.bytes.lo & 0x01 ? 1 : 0;
    u8 msb = cpu->HL.bytes.lo & 0x80;
    cpu->HL.bytes.lo >>= 1;
    cpu->HL.bytes.lo |= msb;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->HL.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_sra_hl_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    u8 carry = val & 0x01 ? 1 : 0;
    u8 msb = val & 0x80;
    val >>= 1;
    val |= msb;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!val) cpu->AF.bytes.lo |= FLAG_Z;
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_sra_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->AF.bytes.hi & 0x01 ? 1 : 0;
    u8 msb = cpu->AF.bytes.hi & 0x80;
    cpu->AF.bytes.hi >>= 1;
    cpu->AF.bytes.hi |= msb;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->AF.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
// ==================== SWAP ====================
void op_cb_swap_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);
    cpu->BC.bytes.hi = (cpu->BC.bytes.hi >> 4) | (cpu->BC.bytes.hi << 4);
    if(!cpu->BC.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_swap_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);
    cpu->BC.bytes.lo = (cpu->BC.bytes.lo >> 4) | (cpu->BC.bytes.lo << 4);
    if(!cpu->BC.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_swap_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);
    cpu->DE.bytes.hi = (cpu->DE.bytes.hi >> 4) | (cpu->DE.bytes.hi << 4);
    if(!cpu->DE.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_swap_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);
    cpu->DE.bytes.lo = (cpu->DE.bytes.lo >> 4) | (cpu->DE.bytes.lo << 4);
    if(!cpu->DE.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_swap_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);
    cpu->HL.bytes.hi = (cpu->HL.bytes.hi >> 4) | (cpu->HL.bytes.hi << 4);
    if(!cpu->HL.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_swap_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);
    cpu->HL.bytes.lo = (cpu->HL.bytes.lo >> 4) | (cpu->HL.bytes.lo << 4);
    if(!cpu->HL.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_swap_hl_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val = (val >> 4) | (val << 4);
    if(!val) cpu->AF.bytes.lo |= FLAG_Z;
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_swap_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H | FLAG_C);
    cpu->AF.bytes.hi = (cpu->AF.bytes.hi >> 4) | (cpu->AF.bytes.hi << 4);
    if(!cpu->AF.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
 
// ==================== SRL ====================
void op_cb_srl_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->BC.bytes.hi & 0x01 ? 1 : 0;
    cpu->BC.bytes.hi >>= 1;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->BC.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_srl_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->BC.bytes.lo & 0x01 ? 1 : 0;
    cpu->BC.bytes.lo >>= 1;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->BC.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_srl_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->DE.bytes.hi & 0x01 ? 1 : 0;
    cpu->DE.bytes.hi >>= 1;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->DE.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_srl_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->DE.bytes.lo & 0x01 ? 1 : 0;
    cpu->DE.bytes.lo >>= 1;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->DE.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_srl_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->HL.bytes.hi & 0x01 ? 1 : 0;
    cpu->HL.bytes.hi >>= 1;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->HL.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_srl_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->HL.bytes.lo & 0x01 ? 1 : 0;
    cpu->HL.bytes.lo >>= 1;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->HL.bytes.lo) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_srl_hl_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    u8 carry = val & 0x01 ? 1 : 0;
    val >>= 1;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!val) cpu->AF.bytes.lo |= FLAG_Z;
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_srl_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N | FLAG_H);
    u8 carry = cpu->AF.bytes.hi & 0x01 ? 1 : 0;
    cpu->AF.bytes.hi >>= 1;
    if(carry) cpu->AF.bytes.lo |= FLAG_C;
        else cpu->AF.bytes.lo &= ~FLAG_C;
    if(!cpu->AF.bytes.hi) cpu->AF.bytes.lo |= FLAG_Z;
    cpu->cycles += 8;
}
// ==================== BIT 0 ====================
void op_cb_bit_0_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->BC.bytes.hi & (1 << 0))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_0_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->BC.bytes.lo & (1 << 0))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_0_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->DE.bytes.hi & (1 << 0))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_0_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->DE.bytes.lo & (1 << 0))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_0_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->HL.bytes.hi & (1 << 0))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_0_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->HL.bytes.lo & (1 << 0))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_0_hl_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    if(!(val & (1 << 0))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 12;
}
void op_cb_bit_0_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->AF.bytes.hi & (1 << 0))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}

// ==================== BIT 1 ====================
void op_cb_bit_1_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->BC.bytes.hi & (1 << 1))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_1_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->BC.bytes.lo & (1 << 1))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_1_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->DE.bytes.hi & (1 << 1))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_1_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->DE.bytes.lo & (1 << 1))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_1_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->HL.bytes.hi & (1 << 1))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_1_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->HL.bytes.lo & (1 << 1))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_1_hl_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    if(!(val & (1 << 1))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 12;
}
void op_cb_bit_1_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->AF.bytes.hi & (1 << 1))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}

// ==================== BIT 2 ====================
void op_cb_bit_2_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->BC.bytes.hi & (1 << 2))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_2_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->BC.bytes.lo & (1 << 2))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_2_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->DE.bytes.hi & (1 << 2))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_2_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->DE.bytes.lo & (1 << 2))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_2_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->HL.bytes.hi & (1 << 2))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_2_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->HL.bytes.lo & (1 << 2))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_2_hl_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    if(!(val & (1 << 2))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 12;
}
void op_cb_bit_2_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->AF.bytes.hi & (1 << 2))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}

// ==================== BIT 3 ====================
void op_cb_bit_3_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->BC.bytes.hi & (1 << 3))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_3_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->BC.bytes.lo & (1 << 3))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_3_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->DE.bytes.hi & (1 << 3))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_3_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->DE.bytes.lo & (1 << 3))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_3_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->HL.bytes.hi & (1 << 3))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_3_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->HL.bytes.lo & (1 << 3))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_3_hl_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    if(!(val & (1 << 3))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 12;
}
void op_cb_bit_3_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->AF.bytes.hi & (1 << 3))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}

// ==================== BIT 4 ====================
void op_cb_bit_4_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->BC.bytes.hi & (1 << 4))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_4_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->BC.bytes.lo & (1 << 4))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_4_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->DE.bytes.hi & (1 << 4))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_4_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->DE.bytes.lo & (1 << 4))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_4_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->HL.bytes.hi & (1 << 4))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_4_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->HL.bytes.lo & (1 << 4))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_4_hl_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    if(!(val & (1 << 4))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 12;
}
void op_cb_bit_4_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->AF.bytes.hi & (1 << 4))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}

// ==================== BIT 5 ====================
void op_cb_bit_5_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->BC.bytes.hi & (1 << 5))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_5_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->BC.bytes.lo & (1 << 5))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_5_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->DE.bytes.hi & (1 << 5))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_5_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->DE.bytes.lo & (1 << 5))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_5_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->HL.bytes.hi & (1 << 5))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_5_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->HL.bytes.lo & (1 << 5))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_5_hl_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    if(!(val & (1 << 5))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 12;
}
void op_cb_bit_5_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->AF.bytes.hi & (1 << 5))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}

// ==================== BIT 6 ====================
void op_cb_bit_6_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->BC.bytes.hi & (1 << 6))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_6_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->BC.bytes.lo & (1 << 6))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_6_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->DE.bytes.hi & (1 << 6))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_6_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->DE.bytes.lo & (1 << 6))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_6_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->HL.bytes.hi & (1 << 6))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_6_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->HL.bytes.lo & (1 << 6))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_6_hl_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    if(!(val & (1 << 6))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 12;
}
void op_cb_bit_6_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->AF.bytes.hi & (1 << 6))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}

// ==================== BIT 7 ====================
void op_cb_bit_7_b(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->BC.bytes.hi & (1 << 7))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_7_c(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->BC.bytes.lo & (1 << 7))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_7_d(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->DE.bytes.hi & (1 << 7))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_7_e(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->DE.bytes.lo & (1 << 7))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_7_h(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->HL.bytes.hi & (1 << 7))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_7_l(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->HL.bytes.lo & (1 << 7))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
void op_cb_bit_7_hl_ind(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    if(!(val & (1 << 7))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 12;
}
void op_cb_bit_7_a(CPU *cpu, Bus *bus) {
    cpu->AF.bytes.lo &= ~(FLAG_Z | FLAG_N);
    cpu->AF.bytes.lo |= FLAG_H;
    if(!(cpu->AF.bytes.hi & (1 << 7))) cpu->AF.bytes.lo |= FLAG_Z;
        else cpu->AF.bytes.lo &= ~FLAG_Z;
    cpu->cycles += 8;
}
// ==================== RES 0 ====================
void op_cb_res_0_b(CPU *cpu, Bus *bus) { cpu->BC.bytes.hi &= ~(1 << 0); cpu->cycles += 8; }
void op_cb_res_0_c(CPU *cpu, Bus *bus) { cpu->BC.bytes.lo &= ~(1 << 0); cpu->cycles += 8; }
void op_cb_res_0_d(CPU *cpu, Bus *bus) { cpu->DE.bytes.hi &= ~(1 << 0); cpu->cycles += 8; }
void op_cb_res_0_e(CPU *cpu, Bus *bus) { cpu->DE.bytes.lo &= ~(1 << 0); cpu->cycles += 8; }
void op_cb_res_0_h(CPU *cpu, Bus *bus) { cpu->HL.bytes.hi &= ~(1 << 0); cpu->cycles += 8; }
void op_cb_res_0_l(CPU *cpu, Bus *bus) { cpu->HL.bytes.lo &= ~(1 << 0); cpu->cycles += 8; }
void op_cb_res_0_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val &= ~(1 << 0);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_res_0_a(CPU *cpu, Bus *bus) { cpu->AF.bytes.hi &= ~(1 << 0); cpu->cycles += 8; }

// ==================== RES 1 ====================
void op_cb_res_1_b(CPU *cpu, Bus *bus) { cpu->BC.bytes.hi &= ~(1 << 1); cpu->cycles += 8; }
void op_cb_res_1_c(CPU *cpu, Bus *bus) { cpu->BC.bytes.lo &= ~(1 << 1); cpu->cycles += 8; }
void op_cb_res_1_d(CPU *cpu, Bus *bus) { cpu->DE.bytes.hi &= ~(1 << 1); cpu->cycles += 8; }
void op_cb_res_1_e(CPU *cpu, Bus *bus) { cpu->DE.bytes.lo &= ~(1 << 1); cpu->cycles += 8; }
void op_cb_res_1_h(CPU *cpu, Bus *bus) { cpu->HL.bytes.hi &= ~(1 << 1); cpu->cycles += 8; }
void op_cb_res_1_l(CPU *cpu, Bus *bus) { cpu->HL.bytes.lo &= ~(1 << 1); cpu->cycles += 8; }
void op_cb_res_1_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val &= ~(1 << 1);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_res_1_a(CPU *cpu, Bus *bus) { cpu->AF.bytes.hi &= ~(1 << 1); cpu->cycles += 8; }

// ==================== RES 2 ====================
void op_cb_res_2_b(CPU *cpu, Bus *bus) { cpu->BC.bytes.hi &= ~(1 << 2); cpu->cycles += 8; }
void op_cb_res_2_c(CPU *cpu, Bus *bus) { cpu->BC.bytes.lo &= ~(1 << 2); cpu->cycles += 8; }
void op_cb_res_2_d(CPU *cpu, Bus *bus) { cpu->DE.bytes.hi &= ~(1 << 2); cpu->cycles += 8; }
void op_cb_res_2_e(CPU *cpu, Bus *bus) { cpu->DE.bytes.lo &= ~(1 << 2); cpu->cycles += 8; }
void op_cb_res_2_h(CPU *cpu, Bus *bus) { cpu->HL.bytes.hi &= ~(1 << 2); cpu->cycles += 8; }
void op_cb_res_2_l(CPU *cpu, Bus *bus) { cpu->HL.bytes.lo &= ~(1 << 2); cpu->cycles += 8; }
void op_cb_res_2_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val &= ~(1 << 2);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_res_2_a(CPU *cpu, Bus *bus) { cpu->AF.bytes.hi &= ~(1 << 2); cpu->cycles += 8; }

// ==================== RES 3 ====================
void op_cb_res_3_b(CPU *cpu, Bus *bus) { cpu->BC.bytes.hi &= ~(1 << 3); cpu->cycles += 8; }
void op_cb_res_3_c(CPU *cpu, Bus *bus) { cpu->BC.bytes.lo &= ~(1 << 3); cpu->cycles += 8; }
void op_cb_res_3_d(CPU *cpu, Bus *bus) { cpu->DE.bytes.hi &= ~(1 << 3); cpu->cycles += 8; }
void op_cb_res_3_e(CPU *cpu, Bus *bus) { cpu->DE.bytes.lo &= ~(1 << 3); cpu->cycles += 8; }
void op_cb_res_3_h(CPU *cpu, Bus *bus) { cpu->HL.bytes.hi &= ~(1 << 3); cpu->cycles += 8; }
void op_cb_res_3_l(CPU *cpu, Bus *bus) { cpu->HL.bytes.lo &= ~(1 << 3); cpu->cycles += 8; }
void op_cb_res_3_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val &= ~(1 << 3);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_res_3_a(CPU *cpu, Bus *bus) { cpu->AF.bytes.hi &= ~(1 << 3); cpu->cycles += 8; }

// ==================== RES 4 ====================
void op_cb_res_4_b(CPU *cpu, Bus *bus) { cpu->BC.bytes.hi &= ~(1 << 4); cpu->cycles += 8; }
void op_cb_res_4_c(CPU *cpu, Bus *bus) { cpu->BC.bytes.lo &= ~(1 << 4); cpu->cycles += 8; }
void op_cb_res_4_d(CPU *cpu, Bus *bus) { cpu->DE.bytes.hi &= ~(1 << 4); cpu->cycles += 8; }
void op_cb_res_4_e(CPU *cpu, Bus *bus) { cpu->DE.bytes.lo &= ~(1 << 4); cpu->cycles += 8; }
void op_cb_res_4_h(CPU *cpu, Bus *bus) { cpu->HL.bytes.hi &= ~(1 << 4); cpu->cycles += 8; }
void op_cb_res_4_l(CPU *cpu, Bus *bus) { cpu->HL.bytes.lo &= ~(1 << 4); cpu->cycles += 8; }
void op_cb_res_4_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val &= ~(1 << 4);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_res_4_a(CPU *cpu, Bus *bus) { cpu->AF.bytes.hi &= ~(1 << 4); cpu->cycles += 8; }

// ==================== RES 5 ====================
void op_cb_res_5_b(CPU *cpu, Bus *bus) { cpu->BC.bytes.hi &= ~(1 << 5); cpu->cycles += 8; }
void op_cb_res_5_c(CPU *cpu, Bus *bus) { cpu->BC.bytes.lo &= ~(1 << 5); cpu->cycles += 8; }
void op_cb_res_5_d(CPU *cpu, Bus *bus) { cpu->DE.bytes.hi &= ~(1 << 5); cpu->cycles += 8; }
void op_cb_res_5_e(CPU *cpu, Bus *bus) { cpu->DE.bytes.lo &= ~(1 << 5); cpu->cycles += 8; }
void op_cb_res_5_h(CPU *cpu, Bus *bus) { cpu->HL.bytes.hi &= ~(1 << 5); cpu->cycles += 8; }
void op_cb_res_5_l(CPU *cpu, Bus *bus) { cpu->HL.bytes.lo &= ~(1 << 5); cpu->cycles += 8; }
void op_cb_res_5_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val &= ~(1 << 5);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_res_5_a(CPU *cpu, Bus *bus) { cpu->AF.bytes.hi &= ~(1 << 5); cpu->cycles += 8; }

// ==================== RES 6 ====================
void op_cb_res_6_b(CPU *cpu, Bus *bus) { cpu->BC.bytes.hi &= ~(1 << 6); cpu->cycles += 8; }
void op_cb_res_6_c(CPU *cpu, Bus *bus) { cpu->BC.bytes.lo &= ~(1 << 6); cpu->cycles += 8; }
void op_cb_res_6_d(CPU *cpu, Bus *bus) { cpu->DE.bytes.hi &= ~(1 << 6); cpu->cycles += 8; }
void op_cb_res_6_e(CPU *cpu, Bus *bus) { cpu->DE.bytes.lo &= ~(1 << 6); cpu->cycles += 8; }
void op_cb_res_6_h(CPU *cpu, Bus *bus) { cpu->HL.bytes.hi &= ~(1 << 6); cpu->cycles += 8; }
void op_cb_res_6_l(CPU *cpu, Bus *bus) { cpu->HL.bytes.lo &= ~(1 << 6); cpu->cycles += 8; }
void op_cb_res_6_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val &= ~(1 << 6);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_res_6_a(CPU *cpu, Bus *bus) { cpu->AF.bytes.hi &= ~(1 << 6); cpu->cycles += 8; }

// ==================== RES 7 ====================
void op_cb_res_7_b(CPU *cpu, Bus *bus) { cpu->BC.bytes.hi &= ~(1 << 7); cpu->cycles += 8; }
void op_cb_res_7_c(CPU *cpu, Bus *bus) { cpu->BC.bytes.lo &= ~(1 << 7); cpu->cycles += 8; }
void op_cb_res_7_d(CPU *cpu, Bus *bus) { cpu->DE.bytes.hi &= ~(1 << 7); cpu->cycles += 8; }
void op_cb_res_7_e(CPU *cpu, Bus *bus) { cpu->DE.bytes.lo &= ~(1 << 7); cpu->cycles += 8; }
void op_cb_res_7_h(CPU *cpu, Bus *bus) { cpu->HL.bytes.hi &= ~(1 << 7); cpu->cycles += 8; }
void op_cb_res_7_l(CPU *cpu, Bus *bus) { cpu->HL.bytes.lo &= ~(1 << 7); cpu->cycles += 8; }
void op_cb_res_7_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val &= ~(1 << 7);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_res_7_a(CPU *cpu, Bus *bus) { cpu->AF.bytes.hi &= ~(1 << 7); cpu->cycles += 8; }

// ==================== SET 0 ====================
void op_cb_set_0_b(CPU *cpu, Bus *bus) { cpu->BC.bytes.hi |= (1 << 0); cpu->cycles += 8; }
void op_cb_set_0_c(CPU *cpu, Bus *bus) { cpu->BC.bytes.lo |= (1 << 0); cpu->cycles += 8; }
void op_cb_set_0_d(CPU *cpu, Bus *bus) { cpu->DE.bytes.hi |= (1 << 0); cpu->cycles += 8; }
void op_cb_set_0_e(CPU *cpu, Bus *bus) { cpu->DE.bytes.lo |= (1 << 0); cpu->cycles += 8; }
void op_cb_set_0_h(CPU *cpu, Bus *bus) { cpu->HL.bytes.hi |= (1 << 0); cpu->cycles += 8; }
void op_cb_set_0_l(CPU *cpu, Bus *bus) { cpu->HL.bytes.lo |= (1 << 0); cpu->cycles += 8; }
void op_cb_set_0_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val |= (1 << 0);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_set_0_a(CPU *cpu, Bus *bus) { cpu->AF.bytes.hi |= (1 << 0); cpu->cycles += 8; }

// ==================== SET 1 ====================
void op_cb_set_1_b(CPU *cpu, Bus *bus) { cpu->BC.bytes.hi |= (1 << 1); cpu->cycles += 8; }
void op_cb_set_1_c(CPU *cpu, Bus *bus) { cpu->BC.bytes.lo |= (1 << 1); cpu->cycles += 8; }
void op_cb_set_1_d(CPU *cpu, Bus *bus) { cpu->DE.bytes.hi |= (1 << 1); cpu->cycles += 8; }
void op_cb_set_1_e(CPU *cpu, Bus *bus) { cpu->DE.bytes.lo |= (1 << 1); cpu->cycles += 8; }
void op_cb_set_1_h(CPU *cpu, Bus *bus) { cpu->HL.bytes.hi |= (1 << 1); cpu->cycles += 8; }
void op_cb_set_1_l(CPU *cpu, Bus *bus) { cpu->HL.bytes.lo |= (1 << 1); cpu->cycles += 8; }
void op_cb_set_1_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val |= (1 << 1);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_set_1_a(CPU *cpu, Bus *bus) { cpu->AF.bytes.hi |= (1 << 1); cpu->cycles += 8; }

// ==================== SET 2 ====================
void op_cb_set_2_b(CPU *cpu, Bus *bus) { cpu->BC.bytes.hi |= (1 << 2); cpu->cycles += 8; }
void op_cb_set_2_c(CPU *cpu, Bus *bus) { cpu->BC.bytes.lo |= (1 << 2); cpu->cycles += 8; }
void op_cb_set_2_d(CPU *cpu, Bus *bus) { cpu->DE.bytes.hi |= (1 << 2); cpu->cycles += 8; }
void op_cb_set_2_e(CPU *cpu, Bus *bus) { cpu->DE.bytes.lo |= (1 << 2); cpu->cycles += 8; }
void op_cb_set_2_h(CPU *cpu, Bus *bus) { cpu->HL.bytes.hi |= (1 << 2); cpu->cycles += 8; }
void op_cb_set_2_l(CPU *cpu, Bus *bus) { cpu->HL.bytes.lo |= (1 << 2); cpu->cycles += 8; }
void op_cb_set_2_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val |= (1 << 2);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_set_2_a(CPU *cpu, Bus *bus) { cpu->AF.bytes.hi |= (1 << 2); cpu->cycles += 8; }

// ==================== SET 3 ====================
void op_cb_set_3_b(CPU *cpu, Bus *bus) { cpu->BC.bytes.hi |= (1 << 3); cpu->cycles += 8; }
void op_cb_set_3_c(CPU *cpu, Bus *bus) { cpu->BC.bytes.lo |= (1 << 3); cpu->cycles += 8; }
void op_cb_set_3_d(CPU *cpu, Bus *bus) { cpu->DE.bytes.hi |= (1 << 3); cpu->cycles += 8; }
void op_cb_set_3_e(CPU *cpu, Bus *bus) { cpu->DE.bytes.lo |= (1 << 3); cpu->cycles += 8; }
void op_cb_set_3_h(CPU *cpu, Bus *bus) { cpu->HL.bytes.hi |= (1 << 3); cpu->cycles += 8; }
void op_cb_set_3_l(CPU *cpu, Bus *bus) { cpu->HL.bytes.lo |= (1 << 3); cpu->cycles += 8; }
void op_cb_set_3_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val |= (1 << 3);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_set_3_a(CPU *cpu, Bus *bus) { cpu->AF.bytes.hi |= (1 << 3); cpu->cycles += 8; }

// ==================== SET 4 ====================
void op_cb_set_4_b(CPU *cpu, Bus *bus) { cpu->BC.bytes.hi |= (1 << 4); cpu->cycles += 8; }
void op_cb_set_4_c(CPU *cpu, Bus *bus) { cpu->BC.bytes.lo |= (1 << 4); cpu->cycles += 8; }
void op_cb_set_4_d(CPU *cpu, Bus *bus) { cpu->DE.bytes.hi |= (1 << 4); cpu->cycles += 8; }
void op_cb_set_4_e(CPU *cpu, Bus *bus) { cpu->DE.bytes.lo |= (1 << 4); cpu->cycles += 8; }
void op_cb_set_4_h(CPU *cpu, Bus *bus) { cpu->HL.bytes.hi |= (1 << 4); cpu->cycles += 8; }
void op_cb_set_4_l(CPU *cpu, Bus *bus) { cpu->HL.bytes.lo |= (1 << 4); cpu->cycles += 8; }
void op_cb_set_4_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val |= (1 << 4);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_set_4_a(CPU *cpu, Bus *bus) { cpu->AF.bytes.hi |= (1 << 4); cpu->cycles += 8; }

// ==================== SET 5 ====================
void op_cb_set_5_b(CPU *cpu, Bus *bus) { cpu->BC.bytes.hi |= (1 << 5); cpu->cycles += 8; }
void op_cb_set_5_c(CPU *cpu, Bus *bus) { cpu->BC.bytes.lo |= (1 << 5); cpu->cycles += 8; }
void op_cb_set_5_d(CPU *cpu, Bus *bus) { cpu->DE.bytes.hi |= (1 << 5); cpu->cycles += 8; }
void op_cb_set_5_e(CPU *cpu, Bus *bus) { cpu->DE.bytes.lo |= (1 << 5); cpu->cycles += 8; }
void op_cb_set_5_h(CPU *cpu, Bus *bus) { cpu->HL.bytes.hi |= (1 << 5); cpu->cycles += 8; }
void op_cb_set_5_l(CPU *cpu, Bus *bus) { cpu->HL.bytes.lo |= (1 << 5); cpu->cycles += 8; }
void op_cb_set_5_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val |= (1 << 5);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_set_5_a(CPU *cpu, Bus *bus) { cpu->AF.bytes.hi |= (1 << 5); cpu->cycles += 8; }

// ==================== SET 6 ====================
void op_cb_set_6_b(CPU *cpu, Bus *bus) { cpu->BC.bytes.hi |= (1 << 6); cpu->cycles += 8; }
void op_cb_set_6_c(CPU *cpu, Bus *bus) { cpu->BC.bytes.lo |= (1 << 6); cpu->cycles += 8; }
void op_cb_set_6_d(CPU *cpu, Bus *bus) { cpu->DE.bytes.hi |= (1 << 6); cpu->cycles += 8; }
void op_cb_set_6_e(CPU *cpu, Bus *bus) { cpu->DE.bytes.lo |= (1 << 6); cpu->cycles += 8; }
void op_cb_set_6_h(CPU *cpu, Bus *bus) { cpu->HL.bytes.hi |= (1 << 6); cpu->cycles += 8; }
void op_cb_set_6_l(CPU *cpu, Bus *bus) { cpu->HL.bytes.lo |= (1 << 6); cpu->cycles += 8; }
void op_cb_set_6_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val |= (1 << 6);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_set_6_a(CPU *cpu, Bus *bus) { cpu->AF.bytes.hi |= (1 << 6); cpu->cycles += 8; }

// ==================== SET 7 ====================
void op_cb_set_7_b(CPU *cpu, Bus *bus) { cpu->BC.bytes.hi |= (1 << 7); cpu->cycles += 8; }
void op_cb_set_7_c(CPU *cpu, Bus *bus) { cpu->BC.bytes.lo |= (1 << 7); cpu->cycles += 8; }
void op_cb_set_7_d(CPU *cpu, Bus *bus) { cpu->DE.bytes.hi |= (1 << 7); cpu->cycles += 8; }
void op_cb_set_7_e(CPU *cpu, Bus *bus) { cpu->DE.bytes.lo |= (1 << 7); cpu->cycles += 8; }
void op_cb_set_7_h(CPU *cpu, Bus *bus) { cpu->HL.bytes.hi |= (1 << 7); cpu->cycles += 8; }
void op_cb_set_7_l(CPU *cpu, Bus *bus) { cpu->HL.bytes.lo |= (1 << 7); cpu->cycles += 8; }
void op_cb_set_7_hl_ind(CPU *cpu, Bus *bus) {
    u8 val = cpu_read(cpu, bus, cpu->HL.full);
    val |= (1 << 7);
    cpu_write(cpu, bus, cpu->HL.full, val);
    cpu->cycles += 16;
}
void op_cb_set_7_a(CPU *cpu, Bus *bus) { cpu->AF.bytes.hi |= (1 << 7); cpu->cycles += 8; }

const Instruction instructions[256] = {
    [0x00] = op_nop,
    [0x01] = op_ld_bc_n16,
    [0x02] = op_ld_bc_ind_a,
    [0x03] = op_inc_bc,
    [0x04] = op_inc_b,
    [0x05] = op_dec_b,
    [0x06] = op_ld_b_n8,
    [0x07] = op_rlca,
    [0x08] = op_ld_n16_ind_sp,
    [0x09] = op_add_hl_bc,
    [0x0A] = op_ld_a_bc_ind,
    [0x0B] = op_dec_bc,
    [0x0C] = op_inc_c,
    [0x0D] = op_dec_c,
    [0x0E] = op_ld_c_n8,
    [0x0F] = op_rrca,
    [0x10] = op_stop,
    [0x11] = op_ld_de_n16,
    [0x12] = op_ld_de_ind_a,
    [0x13] = op_inc_de,
    [0x14] = op_inc_d,
    [0x15] = op_dec_d,
    [0x16] = op_ld_d_n8,
    [0x17] = op_rla,
    [0x18] = op_jr_n8,
    [0x19] = op_add_hl_de,
    [0x1A] = op_ld_a_de_ind,
    [0x1B] = op_dec_de,
    [0x1C] = op_inc_e,
    [0x1D] = op_dec_e,
    [0x1E] = op_ld_e_n8,
    [0x1F] = op_rra,
    [0x20] = op_jr_nz_n8,
    [0x21] = op_ld_hl_n16,
    [0x22] = op_ld_hli_ind_a,
    [0x23] = op_inc_hl,
    [0x24] = op_inc_h,
    [0x25] = op_dec_h,
    [0x26] = op_ld_h_n8,
    [0x27] = op_daa,
    [0x28] = op_jr_z_n8,
    [0x29] = op_add_hl_hl,
    [0x2A] = op_ld_a_hli_ind,
    [0x2B] = op_dec_hl,
    [0x2C] = op_inc_l,
    [0x2D] = op_dec_l,
    [0x2E] = op_ld_l_n8,
    [0x2F] = op_cpl,
    [0x30] = op_jr_nc_n8,
    [0x31] = op_ld_sp_n16,
    [0x32] = op_ld_hld_ind_a,
    [0x33] = op_inc_sp,
    [0x34] = op_inc_hl_ind,
    [0x35] = op_dec_hl_ind,
    [0x36] = op_ld_hl_ind_n8,
    [0x37] = op_scf,
    [0x38] = op_jr_c_n8,
    [0x39] = op_add_hl_sp,
    [0x3A] = op_ld_a_hld_ind,
    [0x3B] = op_dec_sp,
    [0x3C] = op_inc_a,
    [0x3D] = op_dec_a,
    [0x3E] = op_ld_a_n8,
    [0x3F] = op_ccf,
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
    [0x8E] = op_adc_a_hl_ind,
    [0x8F] = op_adc_a_a,
    [0x90] = op_sub_a_b,
    [0x91] = op_sub_a_c,
    [0x92] = op_sub_a_d,
    [0x93] = op_sub_a_e,
    [0x94] = op_sub_a_h,
    [0x95] = op_sub_a_l,
    [0x96] = op_sub_a_hl_ind,
    [0x97] = op_sub_a_a,
    [0x98] = op_sbc_a_b,
    [0x99] = op_sbc_a_c,
    [0x9A] = op_sbc_a_d,
    [0x9B] = op_sbc_a_e,
    [0x9C] = op_sbc_a_h,
    [0x9D] = op_sbc_a_l,
    [0x9E] = op_sbc_a_hl_ind,
    [0x9F] = op_sbc_a_a,
    [0xA0] = op_and_a_b,
    [0xA1] = op_and_a_c,
    [0xA2] = op_and_a_d,
    [0xA3] = op_and_a_e,
    [0xA4] = op_and_a_h,
    [0xA5] = op_and_a_l,
    [0xA6] = op_and_a_hl_ind,
    [0xA7] = op_and_a_a,
    [0xA8] = op_xor_a_b,
    [0xA9] = op_xor_a_c,
    [0xAA] = op_xor_a_d,
    [0xAB] = op_xor_a_e,
    [0xAC] = op_xor_a_h,
    [0xAD] = op_xor_a_l,
    [0xAE] = op_xor_a_hl_ind,
    [0xAF] = op_xor_a_a,
    [0xB0] = op_or_a_b,
    [0xB1] = op_or_a_c,
    [0xB2] = op_or_a_d,
    [0xB3] = op_or_a_e,
    [0xB4] = op_or_a_h,
    [0xB5] = op_or_a_l,
    [0xB6] = op_or_a_hl_ind,
    [0xB7] = op_or_a_a,
    [0xB8] = op_cp_a_b,
    [0xB9] = op_cp_a_c,
    [0xBA] = op_cp_a_d,
    [0xBB] = op_cp_a_e,
    [0xBC] = op_cp_a_h,
    [0xBD] = op_cp_a_l,
    [0xBE] = op_cp_a_hl_ind,
    [0xBF] = op_cp_a_a,
    [0xC0] = op_ret_nz,
    [0xC1] = op_pop_bc,
    [0xC2] = op_jp_nz_n16,
    [0xC3] = op_jp_n16,
    [0xC4] = op_call_nz_n16,
    [0xC5] = op_push_bc,
    [0xC6] = op_add_a_n8,
    [0xC7] = op_rst_00,
    [0xC8] = op_ret_z,
    [0xC9] = op_ret,
    [0xCA] = op_jp_z_n16,
    [0xCC] = op_call_z_n16,
    [0xCD] = op_call_n16,
    [0xCE] = op_adc_a_n8,
    [0xCF] = op_rst_08,
    [0xD0] = op_ret_nc,
    [0xD1] = op_pop_de,
    [0xD2] = op_jp_nc_n16,
    [0xD4] = op_call_nc_n16,
    [0xD5] = op_push_de,
    [0xD6] = op_sub_a_n8,
    [0xD7] = op_rst_10,
    [0xD8] = op_ret_c,
    [0xD9] = op_reti,
    [0xDA] = op_jp_c_n16,
    [0xDC] = op_call_c_n16,
    [0xDE] = op_sbc_a_n8,
    [0xDF] = op_rst_18,
    [0xE0] = op_ldh_n8_ind_a,
    [0xE1] = op_pop_hl,
    [0xE2] = op_ldh_c_ind_a,
    [0xE5] = op_push_hl,
    [0xE6] = op_and_a_n8,
    [0xE7] = op_rst_20,
    [0xE8] = op_add_sp_n8,
    [0xE9] = op_jp_hl,
    [0xEA] = op_ld_n16_ind_a,
    [0xEE] = op_xor_a_n8,
    [0xEF] = op_rst_28,
    [0xF0] = op_ldh_a_n8_ind,
    [0xF1] = op_pop_af,
    [0xF2] = op_ldh_a_c_ind,
    [0xF3] = op_di,
    [0xF5] = op_push_af,
    [0xF6] = op_or_a_n8,
    [0xF7] = op_rst_30,
    [0xF8] = op_ld_hl_sp_n8,
    [0xF9] = op_ld_sp_hl,
    [0xFA] = op_ld_a_n16_ind,
    [0xFB] = op_ei,
    [0xFE] = op_cp_a_n8,
    [0xFF] = op_rst_38,
};

const Instruction instructions_cb[256] = {
    [0x00] = op_cb_rlc_b,
    [0x01] = op_cb_rlc_c,
    [0x02] = op_cb_rlc_d,
    [0x03] = op_cb_rlc_e,
    [0x04] = op_cb_rlc_h,
    [0x05] = op_cb_rlc_l,
    [0x06] = op_cb_rlc_hl_ind,
    [0x07] = op_cb_rlc_a,
    [0x08] = op_cb_rrc_b,
    [0x09] = op_cb_rrc_c,
    [0x0A] = op_cb_rrc_d,
    [0x0B] = op_cb_rrc_e,
    [0x0C] = op_cb_rrc_h,
    [0x0D] = op_cb_rrc_l,
    [0x0E] = op_cb_rrc_hl_ind,
    [0x0F] = op_cb_rrc_a,
    [0x10] = op_cb_rl_b,
    [0x11] = op_cb_rl_c,
    [0x12] = op_cb_rl_d,
    [0x13] = op_cb_rl_e,
    [0x14] = op_cb_rl_h,
    [0x15] = op_cb_rl_l,
    [0x16] = op_cb_rl_hl_ind,
    [0x17] = op_cb_rl_a,
    [0x18] = op_cb_rr_b,
    [0x19] = op_cb_rr_c,
    [0x1A] = op_cb_rr_d,
    [0x1B] = op_cb_rr_e,
    [0x1C] = op_cb_rr_h,
    [0x1D] = op_cb_rr_l,
    [0x1E] = op_cb_rr_hl_ind,
    [0x1F] = op_cb_rr_a,
    [0x20] = op_cb_sla_b,
    [0x21] = op_cb_sla_c,
    [0x22] = op_cb_sla_d,
    [0x23] = op_cb_sla_e,
    [0x24] = op_cb_sla_h,
    [0x25] = op_cb_sla_l,
    [0x26] = op_cb_sla_hl_ind,
    [0x27] = op_cb_sla_a,
    [0x28] = op_cb_sra_b,
    [0x29] = op_cb_sra_c,
    [0x2A] = op_cb_sra_d,
    [0x2B] = op_cb_sra_e,
    [0x2C] = op_cb_sra_h,
    [0x2D] = op_cb_sra_l,
    [0x2E] = op_cb_sra_hl_ind,
    [0x2F] = op_cb_sra_a,
    [0x30] = op_cb_swap_b,
    [0x31] = op_cb_swap_c,
    [0x32] = op_cb_swap_d,
    [0x33] = op_cb_swap_e,
    [0x34] = op_cb_swap_h,
    [0x35] = op_cb_swap_l,
    [0x36] = op_cb_swap_hl_ind,
    [0x37] = op_cb_swap_a,
    [0x38] = op_cb_srl_b,
    [0x39] = op_cb_srl_c,
    [0x3A] = op_cb_srl_d,
    [0x3B] = op_cb_srl_e,
    [0x3C] = op_cb_srl_h,
    [0x3D] = op_cb_srl_l,
    [0x3E] = op_cb_srl_hl_ind,
    [0x3F] = op_cb_srl_a,
    [0x40] = op_cb_bit_0_b,
    [0x41] = op_cb_bit_0_c,
    [0x42] = op_cb_bit_0_d,
    [0x43] = op_cb_bit_0_e,
    [0x44] = op_cb_bit_0_h,
    [0x45] = op_cb_bit_0_l,
    [0x46] = op_cb_bit_0_hl_ind,
    [0x47] = op_cb_bit_0_a,
    [0x48] = op_cb_bit_1_b,
    [0x49] = op_cb_bit_1_c,
    [0x4A] = op_cb_bit_1_d,
    [0x4B] = op_cb_bit_1_e,
    [0x4C] = op_cb_bit_1_h,
    [0x4D] = op_cb_bit_1_l,
    [0x4E] = op_cb_bit_1_hl_ind,
    [0x4F] = op_cb_bit_1_a,
    [0x50] = op_cb_bit_2_b,
    [0x51] = op_cb_bit_2_c,
    [0x52] = op_cb_bit_2_d,
    [0x53] = op_cb_bit_2_e,
    [0x54] = op_cb_bit_2_h,
    [0x55] = op_cb_bit_2_l,
    [0x56] = op_cb_bit_2_hl_ind,
    [0x57] = op_cb_bit_2_a,
    [0x58] = op_cb_bit_3_b,
    [0x59] = op_cb_bit_3_c,
    [0x5A] = op_cb_bit_3_d,
    [0x5B] = op_cb_bit_3_e,
    [0x5C] = op_cb_bit_3_h,
    [0x5D] = op_cb_bit_3_l,
    [0x5E] = op_cb_bit_3_hl_ind,
    [0x5F] = op_cb_bit_3_a,
    [0x60] = op_cb_bit_4_b,
    [0x61] = op_cb_bit_4_c,
    [0x62] = op_cb_bit_4_d,
    [0x63] = op_cb_bit_4_e,
    [0x64] = op_cb_bit_4_h,
    [0x65] = op_cb_bit_4_l,
    [0x66] = op_cb_bit_4_hl_ind,
    [0x67] = op_cb_bit_4_a,
    [0x68] = op_cb_bit_5_b,
    [0x69] = op_cb_bit_5_c,
    [0x6A] = op_cb_bit_5_d,
    [0x6B] = op_cb_bit_5_e,
    [0x6C] = op_cb_bit_5_h,
    [0x6D] = op_cb_bit_5_l,
    [0x6E] = op_cb_bit_5_hl_ind,
    [0x6F] = op_cb_bit_5_a,
    [0x70] = op_cb_bit_6_b,
    [0x71] = op_cb_bit_6_c,
    [0x72] = op_cb_bit_6_d,
    [0x73] = op_cb_bit_6_e,
    [0x74] = op_cb_bit_6_h,
    [0x75] = op_cb_bit_6_l,
    [0x76] = op_cb_bit_6_hl_ind,
    [0x77] = op_cb_bit_6_a,
    [0x78] = op_cb_bit_7_b,
    [0x79] = op_cb_bit_7_c,
    [0x7A] = op_cb_bit_7_d,
    [0x7B] = op_cb_bit_7_e,
    [0x7C] = op_cb_bit_7_h,
    [0x7D] = op_cb_bit_7_l,
    [0x7E] = op_cb_bit_7_hl_ind,
    [0x7F] = op_cb_bit_7_a,
    [0x80] = op_cb_res_0_b,
    [0x81] = op_cb_res_0_c,
    [0x82] = op_cb_res_0_d,
    [0x83] = op_cb_res_0_e,
    [0x84] = op_cb_res_0_h,
    [0x85] = op_cb_res_0_l,
    [0x86] = op_cb_res_0_hl_ind,
    [0x87] = op_cb_res_0_a,
    [0x88] = op_cb_res_1_b,
    [0x89] = op_cb_res_1_c,
    [0x8A] = op_cb_res_1_d,
    [0x8B] = op_cb_res_1_e,
    [0x8C] = op_cb_res_1_h,
    [0x8D] = op_cb_res_1_l,
    [0x8E] = op_cb_res_1_hl_ind,
    [0x8F] = op_cb_res_1_a,
    [0x90] = op_cb_res_2_b,
    [0x91] = op_cb_res_2_c,
    [0x92] = op_cb_res_2_d,
    [0x93] = op_cb_res_2_e,
    [0x94] = op_cb_res_2_h,
    [0x95] = op_cb_res_2_l,
    [0x96] = op_cb_res_2_hl_ind,
    [0x97] = op_cb_res_2_a,
    [0x98] = op_cb_res_3_b,
    [0x99] = op_cb_res_3_c,
    [0x9A] = op_cb_res_3_d,
    [0x9B] = op_cb_res_3_e,
    [0x9C] = op_cb_res_3_h,
    [0x9D] = op_cb_res_3_l,
    [0x9E] = op_cb_res_3_hl_ind,
    [0x9F] = op_cb_res_3_a,
    [0xA0] = op_cb_res_4_b,
    [0xA1] = op_cb_res_4_c,
    [0xA2] = op_cb_res_4_d,
    [0xA3] = op_cb_res_4_e,
    [0xA4] = op_cb_res_4_h,
    [0xA5] = op_cb_res_4_l,
    [0xA6] = op_cb_res_4_hl_ind,
    [0xA7] = op_cb_res_4_a,
    [0xA8] = op_cb_res_5_b,
    [0xA9] = op_cb_res_5_c,
    [0xAA] = op_cb_res_5_d,
    [0xAB] = op_cb_res_5_e,
    [0xAC] = op_cb_res_5_h,
    [0xAD] = op_cb_res_5_l,
    [0xAE] = op_cb_res_5_hl_ind,
    [0xAF] = op_cb_res_5_a,
    [0xB0] = op_cb_res_6_b,
    [0xB1] = op_cb_res_6_c,
    [0xB2] = op_cb_res_6_d,
    [0xB3] = op_cb_res_6_e,
    [0xB4] = op_cb_res_6_h,
    [0xB5] = op_cb_res_6_l,
    [0xB6] = op_cb_res_6_hl_ind,
    [0xB7] = op_cb_res_6_a,
    [0xB8] = op_cb_res_7_b,
    [0xB9] = op_cb_res_7_c,
    [0xBA] = op_cb_res_7_d,
    [0xBB] = op_cb_res_7_e,
    [0xBC] = op_cb_res_7_h,
    [0xBD] = op_cb_res_7_l,
    [0xBE] = op_cb_res_7_hl_ind,
    [0xBF] = op_cb_res_7_a,
    [0xC0] = op_cb_set_0_b,
    [0xC1] = op_cb_set_0_c,
    [0xC2] = op_cb_set_0_d,
    [0xC3] = op_cb_set_0_e,
    [0xC4] = op_cb_set_0_h,
    [0xC5] = op_cb_set_0_l,
    [0xC6] = op_cb_set_0_hl_ind,
    [0xC7] = op_cb_set_0_a,
    [0xC8] = op_cb_set_1_b,
    [0xC9] = op_cb_set_1_c,
    [0xCA] = op_cb_set_1_d,
    [0xCB] = op_cb_set_1_e,
    [0xCC] = op_cb_set_1_h,
    [0xCD] = op_cb_set_1_l,
    [0xCE] = op_cb_set_1_hl_ind,
    [0xCF] = op_cb_set_1_a,
    [0xD0] = op_cb_set_2_b,
    [0xD1] = op_cb_set_2_c,
    [0xD2] = op_cb_set_2_d,
    [0xD3] = op_cb_set_2_e,
    [0xD4] = op_cb_set_2_h,
    [0xD5] = op_cb_set_2_l,
    [0xD6] = op_cb_set_2_hl_ind,
    [0xD7] = op_cb_set_2_a,
    [0xD8] = op_cb_set_3_b,
    [0xD9] = op_cb_set_3_c,
    [0xDA] = op_cb_set_3_d,
    [0xDB] = op_cb_set_3_e,
    [0xDC] = op_cb_set_3_h,
    [0xDD] = op_cb_set_3_l,
    [0xDE] = op_cb_set_3_hl_ind,
    [0xDF] = op_cb_set_3_a,
    [0xE0] = op_cb_set_4_b,
    [0xE1] = op_cb_set_4_c,
    [0xE2] = op_cb_set_4_d,
    [0xE3] = op_cb_set_4_e,
    [0xE4] = op_cb_set_4_h,
    [0xE5] = op_cb_set_4_l,
    [0xE6] = op_cb_set_4_hl_ind,
    [0xE7] = op_cb_set_4_a,
    [0xE8] = op_cb_set_5_b,
    [0xE9] = op_cb_set_5_c,
    [0xEA] = op_cb_set_5_d,
    [0xEB] = op_cb_set_5_e,
    [0xEC] = op_cb_set_5_h,
    [0xED] = op_cb_set_5_l,
    [0xEE] = op_cb_set_5_hl_ind,
    [0xEF] = op_cb_set_5_a,
    [0xF0] = op_cb_set_6_b,
    [0xF1] = op_cb_set_6_c,
    [0xF2] = op_cb_set_6_d,
    [0xF3] = op_cb_set_6_e,
    [0xF4] = op_cb_set_6_h,
    [0xF5] = op_cb_set_6_l,
    [0xF6] = op_cb_set_6_hl_ind,
    [0xF7] = op_cb_set_6_a,
    [0xF8] = op_cb_set_7_b,
    [0xF9] = op_cb_set_7_c,
    [0xFA] = op_cb_set_7_d,
    [0xFB] = op_cb_set_7_e,
    [0xFC] = op_cb_set_7_h,
    [0xFD] = op_cb_set_7_l,
    [0xFE] = op_cb_set_7_hl_ind,
    [0xFF] = op_cb_set_7_a,
};
