#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cpu/6502.h"

typedef struct {
    int size;
    unsigned char* data;
} rom;

bool load_rom(const char* fn, rom* rom) {
    FILE* fp = 0;
    int fs = 0;
    bool error = false;

    fp = fopen(fn, "r");

    if(!fp) {
        printf("File couldn't open.\n");
        error = true;
    } else {
        fseek(fp, 0, SEEK_END);
        (*rom).size = ftell(fp);
        rewind(fp);
        
        (*rom).data = malloc((*rom).size * sizeof(unsigned char));
        fread((*rom).data, sizeof(unsigned char), (*rom).size, fp);
    }

    return !error;
}

int main(int argc, char* argv[]) {
    rom rom = {0, 0};
    bool error = false;

    if(argc != 2) {
        printf("ROM file needed!\n");
        error = true;
    }

    if(!error) {
        error = load_rom(argv[1], &rom);
        cpu_tick(rom.data[0]);
    }
    
    free(rom.data);

    return !error;
}