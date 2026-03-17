#ifndef CPU_H
#define CPU_H

#include "types.h"

typedef struct Bus Bus;

#define FLAG_Z 0x80 // 7 z Zero flag
#define FLAG_N 0x40 // 6 n Subtraction flag (BCD)
#define FLAG_H 0x20 // 5 h Half Carry flag (BCD)
#define FLAG_C 0x10 // 4 c Carry flag

typedef enum { ALU_ADD, ALU_ADC, ALU_SUB, ALU_SBC, 
    ALU_AND, ALU_OR, ALU_XOR, ALU_CP } ALU_OP;

typedef union {
    u16 full;
    struct {
        u8 lo;
        u8 hi;
    } bytes;
} Reg16;

typedef struct{
    // Accumalator & Flags, parte alta A
    // Parte bassa:
    Reg16 AF;    
    // Parte alta B, parte bassa C
    Reg16 BC;
    // Parte alta D, parte bassa E
    Reg16 DE;
    // Parte alta H, parte bassa L
    Reg16 HL;
    // Stack Pointer
    u16 SP;
    // Program Counter
    u16 PC;

    u64 cycles;
    
    // Flag di Stato, informazioni interne dell'emulatore
    u8 IME; // Interrupt Master Enable, interrutore globale per le interruzioni
    u8 halted; // halt
    u8 stopped; // per il risparmio energetico
} CPU;

void cpu_init(CPU *cpu);

// Fetch + Decode + Execute
void cpu_step(CPU *cpu, Bus *bus);

// Read
u8 cpu_read(CPU *cpu, Bus *bus, u16 address);
// Write
void cpu_write(CPU *cpu, Bus *bus, u16 address, u8 value);

u16 cpu_read_word(CPU *cpu, Bus *bus);

u8 cpu_alu(CPU *cpu, u8 a, u8 b, ALU_OP op);
#endif
