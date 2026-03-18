#include "bus.h"
#include "types.h"
#include <stdio.h>

u8 bus_read(Bus *bus, u16 address){
    if(!bus) return 0xFF; //Convenzione errori

    if (address <= 0x7FFF){
        // ROM cartuccia
        return bus->mmu->memory[address];
    } else if (address <= 0x9FFF){
        return bus->mmu->memory[address];
        // VRAM
    } else if(address >= 0xA000 && address <= 0xBFFF){
        // External RAM
        return bus->mmu->memory[address];
    } else if(address >= 0xC000 && address <= 0xDFFF){
        //WRAM
        return bus->mmu->memory[address];
    } else if(address >= 0xE000 && address <= 0xFDFF){
        // Echo RAM - mirror di 0x2000
        address -= 0x2000;
        return bus->mmu->memory[address];
    } else if(address >= 0xFE00 && address <= 0xFE9F){
        //OAM
        return bus->mmu->memory[address];
    } else if(address >= 0xFEA0 && address <= 0xFEFF){ 
        //Zona proibita
        return 0xFF;
    } else if(address >= 0xFF00 && address <= 0xFF7F){ 
        //Spazio di I/O da gestire a parte 
        return bus->mmu->memory[address];
    } else if(address >= 0xFF80 && address <= 0xFFFE){
        //HRAM
        return bus->mmu->memory[address];
    } else if(address == 0xFFFF){
        //Interrupt Enable
        return bus->mmu->memory[address];
    }
    return bus->mmu->memory[address];
}

void bus_write(Bus *bus, u16 address, u8 value){
    if(!bus) return;
    
    if(address <= 0x7FFF){
        // ROM - sola lettura, ignora
        return;
    } else if(address <= 0x9FFF){
        // VRAM
        bus->mmu->memory[address] = value;
    } else if(address >= 0xA000 && address <= 0xBFFF){
        // External RAM
        bus->mmu->memory[address] = value;
    } else if(address >= 0xC000 && address <= 0xDFFF){
        // WRAM
        bus->mmu->memory[address] = value;
    } else if(address >= 0xE000 && address <= 0xFDFF){
        // Echo RAM - mirror di 0xC000
        address -= 0x2000;
        bus->mmu->memory[address] = value;
    } else if(address >= 0xFE00 && address <= 0xFE9F){
        // OAM
        bus->mmu->memory[address] = value;
    } else if(address >= 0xFEA0 && address <= 0xFEFF){
        // Zona proibita - ignora
        return;
    } else if(address >= 0xFF00 && address <= 0xFF7F){
        // I/O registers - da gestire a parte
        bus->mmu->memory[address] = value;
        // Intercetta output seriale blargg
        if(address == 0xFF02 && value == 0x81){
            char c = (char)bus->mmu->memory[0xFF01];
            printf("%c", c);
            fflush(stdout);
        }
    } else if(address >= 0xFF80 && address <= 0xFFFE){
        // HRAM
        bus->mmu->memory[address] = value;
    } else if(address == 0xFFFF){
        // Interrupt Enable
        bus->mmu->memory[address] = value;
    }
}
