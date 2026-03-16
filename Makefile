all:
	mkdir -p build
	gcc -W -Wall -pedantic -O2 -std=c99 -I./include main.c src/cpu.c src/mmu.c src/bus.c src/ppu.c src/apu.c src/timer.c src/joypad.c src/cartridge.c -o build/gameboy -lSDL2

clean:
	rm -rf build

.PHONY: all clean
