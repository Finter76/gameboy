#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "cpu.h"
#include "bus.h"
#include "mmu.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./build/gameboy <rom.gb>\n");
        return 1;
    }

    // Inizializza i componenti
    MMU mmu;
    memset(&mmu, 0, sizeof(MMU));

    Bus bus;
    bus.mmu = &mmu;

    CPU cpu;
    cpu_init(&cpu);

    // 1. Controlla che sia un file regolare
    struct stat st;
    if (stat(argv[1], &st) != 0 || !S_ISREG(st.st_mode)) {
        printf("Error: %s is not a regular file\n", argv[1]);
        return 1;
    }

    // 2. Controlla estensione .gb
    size_t len = strlen(argv[1]);
    if (len < 3 || strcmp(argv[1] + len - 3, ".gb") != 0) {
        printf("Error: file must have .gb extension\n");
        return 1;
    }

    // 3. Apri il file
    // Carica la ROM in memoria
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        printf("Error opening: %s\n", argv[1]);
        return 1;
    }
    fread(mmu.memory, 1, 0x8000, f);
    fclose(f);

    // Esegui qualche istruzione e stampa i registri
    for (int i = 0; i < 100; i++) {
        printf("PC:%04X OP:%02X AF:%04X BC:%04X DE:%04X HL:%04X SP:%04X\n",
            cpu.PC, bus_read(&bus, cpu.PC),cpu.AF.full, cpu.BC.full,
            cpu.DE.full, cpu.HL.full, cpu.SP);
        cpu_step(&cpu, &bus);
    }

    return 0;
}
