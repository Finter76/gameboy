#ifndef BUS_H
#define BUS_H

#include "types.h"
#include "mmu.h"
#include "cpu.h"

typedef struct Bus {
    MMU *mmu;
    CPU *cpu;
} Bus;

u8 bus_read(Bus *bus, u16 address);
void bus_write(Bus *bus, u16 address, u8 value);

#endif 
