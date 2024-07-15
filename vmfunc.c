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
    update_flags(r0);
}

void bitwise_and(uint16_t instr)
{
    // destination registry
    uint16_t r0 = (instr >> 9) & 0x7;
    // first operand
    uint16_t r1 = (instr >> 6) & 0x7;
    // check flag for imediate mode
    uint16_t imm_flag = (instr >> 5) & 0x1;

    if (imm_flag)
    {
        uint16_t imm5 = sign_extend(instr & 0x1F, 5);
        reg[r0] = reg[r1] & imm5;
    }
    else 
    {
        uint16_t r2 = instr & 0x7;
        reg[r0] = reg[r1] & reg[r2];
    }
    update_flags(r0);
}

void branch(uint16_t instr)
{
    // flag for n, z, and p condition codes
    uint16_t cond_flag = (instr >> 9) & 0x7;

    if (cond_flag & reg[R_COND])
    {
        uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
        reg[R_PC] += pc_offset;
    }
}

void jump(uint16_t instr)
{
    // base register
    uint16_t r0 = (instr >> 6) & 0x7;

    reg[R_PC] = reg[r0];
}

void jump_sr(uint16_t instr)
{
    // store current counter address at r7
    reg[R_R7] = reg[R_PC];
    // flag to distinguish between JSR and JSRR
    uint16_t mode_flag = (instr >> 11) & 0x1;

    if (mode_flag)
    {
        // JSR
        uint16_t pc_offset = sign_extend((instr & 0x7FF), 11);
        reg[R_PC] += pc_offset;
    }
    else 
    {
        // JSRR
        uint16_t r0 = (instr >> 6) & 0x7;
        reg[R_PC] = reg[r0];
    }
}

void load(uint16_t instr) {
    // offset from PC to load data from
    uint16_t pc_offset = sign_extend((instr & 0x1FF), 9);
    // destination registry
    uint16_t r0 = (instr >> 9) & 0x7;

    reg[r0] = mem_read(reg[R_PC] + pc_offset);
    update_flags(r0);
}

void load_indirect(uint16_t instr)
{
    // destination registry
    uint16_t r0 = (instr >> 9) & 0x7;
    // offset from PC of the location storing the desired data's address
    uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);

    reg[r0] = mem_read(mem_read(reg[R_PC] + pc_offset));
    update_flags(r0);
}

void load_baseoffset(uint16_t instr)
{
    // destination registry
    uint16_t r0 = (instr >> 9) & 0x7;
    // base registry
    uint16_t r1 = (instr >> 6) & 0x7;
    // offset from address stored at r1
    uint16_t r_offset = sign_extend((instr & 0x3F), 6);

    reg[r0] = mem_read(reg[r1] + r_offset);
    update_flags(r0);
}

void load_ea(uint16_t instr)
{
    // destination registry
    uint16_t r0 = (instr >> 9) & 0x7;
    // offset to address at PC
    uint16_t pc_offset = sign_extend((instr & 0x1FF), 9);

    reg[r0] = reg[R_PC] + pc_offset;
    update_flags(r0);
}

void bitwise_complement(uint16_t instr)
{
    // destination registry
    uint16_t r0 = (instr >> 9) & 0x7;
    // operand
    uint16_t r1 = (instr >> 6) & 0x7;

    reg[r0] = ~reg[r1];
    update_flags(r0);
}

void store(uint16_t instr)
{
    // source registry
    uint16_t r0 = (instr >> 9) & 0x7;
    // offset to address at PC
    uint16_t pc_offset = sign_extend((instr & 0x1FF), 9);

    mem_write(reg[R_PC] + pc_offset, reg[r0]);
}

void store_indirect(uint16_t instr)
{
    // source registry
    uint16_t r0 = (instr >> 9) & 0x7;
    // offset from PC of the location storing the desired data's address 
    uint16_t pc_offset = sign_extend((instr & 0x1FF), 9);

    mem_write(mem_read(reg[R_PC] + pc_offset), reg[r0]);
}

void store_baseoffset(uint16_t instr)
{
    // source registry
    uint16_t r0 = (instr >> 9) & 0x7;
    // base registry
    uint16_t r1 = (instr >> 6) & 0x7;
    // offset from address stored at r1
    uint16_t r_offset = sign_extend((instr & 0x3F), 6);

    mem_write(reg[r1] + r_offset, reg[r0]);
}