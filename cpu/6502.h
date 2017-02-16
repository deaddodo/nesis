#ifndef __6502_H__
#define __6502_H__
#include <stdio.h>
#include <stdbool.h>

#include "../shared/utils.h"

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
} cpu_bus_req;

typedef struct {
    unsigned short sp;
    unsigned short pc;
    unsigned char a, x, y;
    unsigned char status;
} cpu_registers;

typedef struct {
    // operation variables
    cpu_status status;
    cpu_bus_req bus_request;
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