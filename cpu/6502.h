#ifndef __6502_H__
#define __6502_H__
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../shared/utils.h"

typedef struct cpu cpu; // forward declare cpu struct for circular reference on cpu_op

typedef enum {
    NEXT_INSTRUCTION,
    BUS_REQUEST,
    HALT,
    STOPPED
} cpu_status;

typedef enum {
    NIL,
    WRITE,
    READ
} cpu_bus_req_type;

typedef struct {
    cpu_bus_req_type type;
    bool set;
    unsigned short addr;
    unsigned char data;
    bool single_page;
    bool direct;
} cpu_bus_req;

typedef struct {
    unsigned short sp;
    unsigned short pc;
    unsigned char a, x, y;
    unsigned char status;
} cpu_registers;

typedef struct {
    unsigned char cycles, length;
    bool ready;
    unsigned char data;
    void (*call)(cpu*);
    char lhs[4], rhs[8];
} cpu_op;

typedef struct cpu {
    // operation variables
    cpu_status status;
    cpu_bus_req bus_request;
    cpu_op op;
    // data buffer to minimize bus requests
    unsigned char buffer[3];
    // registers
    cpu_registers reg;
    // interrupt vectors
    unsigned short nmi, reset, irq;
} cpu;

typedef enum {
    SUCCESS,
    BUS,
    FAILED
} cpu_op_status;

void cpu_init(cpu*);
unsigned char cpu_tick(cpu *);
#endif