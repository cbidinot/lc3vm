#ifndef VMFUNC
#define VMFUNC

#include <stdint.h>
#include <stdio.h>
#define MEMORY_MAX (1 << 16)
#define PC_START 0x3000

// Registers
typedef enum
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
} registers;

// Condition Flags
typedef enum
{
    FL_POS = 1, // P
    FL_ZRO = 1 << 1, // Z
    FL_NEG = 1 << 2 // N
} cond_flags;

// Operation Codes
typedef enum
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
} op_code;

// Trap codes
typedef enum
{
   TRAP_GETC = 0x20, // get char from keyboard (not echoed on terminal)
   TRAP_OUT = 0x21, // output char
   TRAP_PUTS = 0x22, // output word string
   TRAP_IN = 0x23,  // get char from keyboard (echoed in terminal)
   TRAP_PUTSP = 0x24,   // output byte string
   TRAP_HALT = 0x25 // halt the program
} trap_code;

// Memory mapped registers
typedef enum
{
    MR_KBSR = 0xFE00, // keyboard status
    MR_KBDR = 0xFE02  // keyboard data
} memory_register;

extern uint16_t memory[MEMORY_MAX];
extern uint16_t reg[R_COUNT];
extern int running;
extern struct termios original_tio;


void add(uint16_t instr);
void bitwise_and(uint16_t instr);
void branch(uint16_t instr);
void jump(uint16_t instr);
void jump_sr(uint16_t instr);
void load(uint16_t instr);
void load_indirect(uint16_t instr);
void load_baseoffset(uint16_t instr);
void load_ea(uint16_t instr);
void bitwise_complement(uint16_t instr);
void store(uint16_t instr);
void store_indirect(uint16_t instr);
void store_baseoffset(uint16_t instr);
void trap(uint16_t instr);
void read_image_file(FILE* file);
int read_image(const char* image_path);
uint16_t swap16(uint16_t x);
void mem_write(uint16_t address, uint16_t val);
uint16_t mem_read(uint16_t address);

void disable_input_buffering();
void restore_input_buffering();
uint16_t check_key();
void handle_interrupt(int signal);

#endif