#ifndef GAMEBOY_H
#define GAMEBOY_H

#include "types.h"
#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "timer.h"
#include "joypad.h"
#include "cartridge.h"

typedef struct {
    CPU cpu;
    PPU ppu;
    APU apu;
    Timer timer;
    Joypad joypad;
    Cartridge cartridge;
} GameBoy;

#endif
