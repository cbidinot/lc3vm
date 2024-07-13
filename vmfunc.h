#include <stdint.h>
#define MEMORY_MAX (1 << 16)

// Registers
enum
{
    R_R0 = 0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_PC, // Program counter
    R_COND,
    R_COUNT
};

// Condition Flags
enum
{
    FL_POS = 1, // P
    FL_ZRO = 1 << 1, // Z
    FL_NEG = 1 << 2 // N
};

// Operation Codes
enum
{
    OP_BR = 0, // branch
    OP_ADD, // add
    OP_LD, // load
    OP_ST, // store
    OP_JSR, // jump register
    OP_AND, // bitwise and
    OP_LDR, // load register
    OP_STR, // store register
    OP_RTI, // unused
    OP_NOT, // bitwise not
    OP_LDI, // load indirect
    OP_STI, // store indirect
    OP_JMP, // jump
    OP_RES, // reserved (unused)
    OP_LEA, // load effective address
    OP_TRAP // execute trap
};

enum {PC_START = 0x3000};

extern uint16_t memory[MEMORY_MAX];
extern uint16_t reg[R_COUNT];