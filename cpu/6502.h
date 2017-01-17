#include <stdio.h>

typedef enum {
    PASS,
    BUS_REQUEST,
    HALT,
    STOPPED
} cpu_status;

typedef struct {
    cpu_status status;
    int PC;
} cpu;

void cpu_init();
unsigned int cpu_tick(unsigned char);