#ifndef IO_MAP_H
#define IO_MAP_H

// I/O Registers (0xFF00 - 0xFF7F)
#define IO_JOYPAD_INPUT         0xFF00
#define IO_SERIAL_TRANSFER      0xFF01  // fino a 0xFF02
#define IO_TIMER_DIVIDER        0xFF04  // fino a 0xFF07
#define IO_INTERRUPTS           0xFF0F
#define IO_AUDIO                0xFF10  // fino a 0xFF26
#define IO_WAVE_PATTERN         0xFF30  // fino a 0xFF3F
#define IO_LCD_CONTROL          0xFF40  // fino a 0xFF4B
#define IO_OAM_DMA_TRANSFER     0xFF46
#define IO_KEY0                 0xFF4C
#define IO_KEY1                 0xFF4D
#define IO_VRAM_BANK            0xFF4F
#define IO_ROM_MAPPING          0xFF50
#define IO_VRAM_DMA             0xFF51  // fino a 0xFF55
#define IO_IR_PORT              0xFF56
#define IO_BG_PALETTES          0xFF68  // fino a 0xFF6B
#define IO_OBJECT_PRIORITY_MODE 0xFF6C
#define IO_WRAM_BANK            0xFF70  

#endif
