#include "vmfunc.h"
#include <stdint.h>

uint16_t sign_extend(uint16_t x, int bit_count)
{
    if (x >> (bit_count - 1))
        x |= (0xFFFF << bit_count);

    return x;
}

void update_flags(uint16_t r)
{
    if (!reg[r])
        reg[R_COND] = FL_ZRO;
    else if (reg[r] >> 15)
        reg[R_COND] = FL_NEG;
    else
        reg[R_COND] = FL_POS;
}

void add(uint16_t instr)
{
    // destination registry
    uint16_t r0 = (instr >> 9) & 0x7;
    // first operand
    uint16_t r1 = (instr >> 6) & 0x7;
    // check flag for immediate mode
    uint16_t imm_flag = (instr >> 5) & 0x1;

    if (imm_flag)
    {
        uint16_t imm5 = sign_extend(instr & 0x1F, 5);
        reg[r0] = reg[r1] + imm5;
    }
    else
    {
        uint16_t r2 = instr & 0x7;
        reg[r0] = reg[r1] + reg[r2];
    }
}