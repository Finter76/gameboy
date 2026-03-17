#include "cpu.h"
#include "types.h"
#include "bus.h"
#include "opcodes.h"
#include <string.h>

void cpu_init(CPU *cpu){
    memset(cpu, 0, sizeof(CPU));
    
    // Valori al reset di DMG (gameboy originale)
    cpu->AF.full = 0x01B0;
    cpu->BC.full = 0x0013;
    cpu->DE.full = 0x00D8;
    cpu->HL.full = 0x014D;
    cpu->SP      = 0xFFFE;
    cpu->PC      = 0x0100;
}

// Fetch + Decode + Execute
void cpu_step(CPU *cpu, Bus *bus){
    // Leggo il primo byte -> opcode
    u8 opcode = cpu_read(cpu, bus, cpu->PC);
    
    // Capisco cosa richiede l'opcode
    if(opcode == 0xCB){
        u8 opcode_cb = cpu_read(cpu, bus, cpu->PC);

        if(!instructions_cb[opcode_cb]) return;

        instructions_cb[opcode_cb](cpu, bus);
    } else {
        if(!instructions[opcode]) return;

        instructions[opcode](cpu, bus);
    }
}

// Read
u8 cpu_read(CPU *cpu, Bus *bus, u16 address){
    cpu->PC++;
    return bus_read(bus, address);
}

// Write
void cpu_write(CPU *cpu, Bus *bus, u16 address, u8 value){
    bus_write(bus, address, value);
}

u16 cpu_read_word(CPU *cpu ,Bus *bus){
    u8 val_low = cpu_read(cpu, bus, cpu->PC);
    u8 val_high = cpu_read(cpu, bus, cpu->PC);
   
    return (val_high << 8) | val_low;
}

u8 cpu_alu(CPU *cpu, u8 a, u8 b, ALU_OP op) {
    u16 result16;
    u8 f = 0;
    u8 carry_in = 0;

    if(op == ALU_ADC || op == ALU_SBC)
        carry_in = (cpu->AF.bytes.lo & FLAG_C) ? 1 : 0;

    switch(op) {
        case ALU_ADD:
            result16 = a + b;
            if ((result16 & 0xFF) == 0) f |= FLAG_Z;
            if (((a & 0xF) + (b & 0xF)) > 0xF) f |= FLAG_H;
            if (result16 > 0xFF) f |= FLAG_C;
            break;

        case ALU_ADC:
            result16 = a + b + carry_in;
            if ((result16 & 0xFF) == 0) f |= FLAG_Z;
            if (((a & 0xF) + (b & 0xF) + carry_in) > 0xF) f |= FLAG_H;
            if (result16 > 0xFF) f |= FLAG_C;
            break;

        case ALU_SUB:
            result16 = a - b;
            if ((result16 & 0xFF) == 0) f |= FLAG_Z;
            f |= FLAG_N;
            if ((a & 0xF) < (b & 0xF)) f |= FLAG_H;
            if (a < b) f |= FLAG_C;
            break;

        case ALU_SBC:
            result16 = a - b - carry_in;
            if ((result16 & 0xFF) == 0) f |= FLAG_Z;
            f |= FLAG_N;
            if ((a & 0xF) < ((b & 0xF) + carry_in)) f |= FLAG_H;
            if (a < (b + carry_in)) f |= FLAG_C;
            break;

        case ALU_AND:
            result16 = a & b;
            if ((result16 & 0xFF) == 0) f |= FLAG_Z;
            f |= FLAG_H;
            // N=0, C=0
            f &= ~(FLAG_N | FLAG_C);
            break;

        case ALU_OR:
            result16 = a | b;
            if ((result16 & 0xFF) == 0) f |= FLAG_Z;
            f &= ~(FLAG_N | FLAG_H | FLAG_C);
            break;

        case ALU_XOR:
            result16 = a ^ b;
            if ((result16 & 0xFF) == 0) f |= FLAG_Z;
            f &= ~(FLAG_N | FLAG_H | FLAG_C);
            break;

        case ALU_CP:
            result16 = a - b;
            if ((result16 & 0xFF) == 0) f |= FLAG_Z;
            f |= FLAG_N;
            if ((a & 0xF) < (b & 0xF)) f |= FLAG_H;
            if (a < b) f |= FLAG_C;
            cpu->AF.bytes.lo = f;
            return 0; // CP non modifica A
    }

    cpu->AF.bytes.lo = f;
    return (u8)result16;
}
