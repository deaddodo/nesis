#ifndef __MEMORY_H__
#define __MEMORY_H__
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../shared/utils.h"

typedef struct __attribute__((__packed__)) {
    unsigned int marker;
    unsigned char prg;
    unsigned char chr;
    unsigned char flg6;
    unsigned char flg7;
    unsigned char ram;
    unsigned char flg9;
    unsigned char flg10;
    unsigned char nil[5];
} header;

typedef struct {
    header header;
    unsigned char iram[2048];
    unsigned char* prg;
    unsigned char* chr;
    unsigned char* ram;
    unsigned char bank;
} memory;

bool memory_init(memory*, const char *);
void memory_destroy(memory*);
unsigned char read_byte(memory*, unsigned short);
void write_byte(memory*, unsigned short, unsigned char);
#endif