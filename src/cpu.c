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
    cpu->PC++;
    // Capisco cosa richiede l'opcode
    if(opcode == 0xCB){
        u8 opcode_cb = cpu_read(cpu, bus, cpu->PC);
        cpu->PC++;

        if(!instructions_cb[opcode_cb]) return;

        instructions_cb[opcode_cb](cpu, bus);
    } else {
        if(!instructions[opcode]) return;

        instructions[opcode](cpu, bus);
    }
}

// Read
u8 cpu_read(CPU *cpu, Bus *bus, u16 address){
    return bus_read(bus, address);
}

// Write
void cpu_write(CPU *cpu, Bus *bus, u16 address, u8 value){
    bus_write(bus, address, value);
}

u16 cpu_read_word(CPU *cpu ,Bus *bus){
    u8 val_low = cpu_read(cpu, bus, cpu->PC);
    cpu->PC++;

    u8 val_high = cpu_read(cpu, bus, cpu->PC);
    cpu->PC++;

    return (val_high << 8) | val_low;
}

