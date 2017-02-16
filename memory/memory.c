// TODO: We only support UxROM and NROM...add mappers later

#include "memory.h"

bool load_rom(const char* fn, memory* memory) {
    FILE* fp = 0;
    bool error = false;
    long size;
    unsigned char mapper;

    fp = fopen(fn, "r");

    if(!fp) {
        printf("File couldn't open.\n");
        error = true;
    } else {
        fread(&memory->header, sizeof(header), 1, fp);
        mapper = (memory->header.flg6 & 0xF0) | (memory->header.flg7 & 0x0F);
        printf("Mapper ID: %hhu\n", mapper);

        // stupid as shit, but x86 reads values in as little endian.
        // let's assume the target arch is at least 32-bit little or big endian and compare against those
        if(!(memory->header.marker == 0x4e45531a || memory->header.marker == 0x1a53454e)) {
            error = true;
            printf("Only iNES files are supported\n");
            printf("Header marker: %08x\n", memory->header.marker);
        } else {
            // we only call the loader once...we can waste some instance vars
            size_t prg_size = (memory->header.prg * 16384) * sizeof(unsigned char);
            size_t chr_size = (memory->header.chr * 8192) * sizeof(unsigned char);
            size_t ram_size = (memory->header.ram * 8192) * sizeof(unsigned char);

            memory->prg = malloc(prg_size);
            fread(memory->prg, sizeof(unsigned char), prg_size, fp);
            memory->chr = malloc(chr_size);
            fread(memory->chr, sizeof(unsigned char), chr_size, fp);
            memory->ram = malloc(ram_size);
            memory->bank = 0;
        }
    }

    return !error;
}

bool memory_init(memory* memory, const char* rom_file) {
    bool error = false;

    if(!load_rom(rom_file, memory)) {
        printf("Failed to load ROM file\n");
        error = true;
    }

    return error;
}

void memory_destroy(memory* memory) {
    free(memory->prg);
    free(memory->chr);
    free(memory->ram);
}

unsigned char read_byte(memory* memory, unsigned short addr) {
    unsigned char data;

    switch(addr) {
        case 0x0000 ... 0x7fff: {
            data = memory->iram[addr];
        } break;
        case 0x8000 ... 0xbfff: { // banked rom
            unsigned int rom_addr = (unsigned int)((addr - 0x8000) + (16384 * memory->bank));
            data = memory->prg[rom_addr];
        } break;
        case 0xc000 ... 0xffff: { // unbanked rom
            unsigned int rom_addr = (unsigned int)((addr - 0xc000) + (16384 * (memory->header.prg - 1)));
            data = memory->prg[rom_addr];
        } break;
        default:{ // odd bus request
            printf("Unmapped memory request: %04x\n", addr);
            data = 0x00;
        } break;
    }
    return data;
}

void write_byte(memory* memory, unsigned short addr, unsigned char data) {
    // TODO: write something
}