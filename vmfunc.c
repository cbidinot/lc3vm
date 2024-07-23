#include "vmfunc.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/termios.h>

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

void trap(uint16_t instr)
{
    // store address at PC in r7
    reg[R_R7] = reg[R_PC];

    switch (instr & 0xFF)
    {
        case TRAP_GETC:
            reg[R_R0] = getc(stdin) & 0xFF;
            update_flags(R_R0);
            break;
        case TRAP_OUT:
            putc((char) reg[R_R0], stdout);
            fflush(stdout);
            break;
        case TRAP_PUTS:
            // one character per memory location
            {
                uint16_t *c = memory + reg[R_R0];
                while (*c)
                {
                    putc((char)*c, stdout);
                    ++c;
                }
                fflush(stdout);
                break;
            }
        case TRAP_IN:
            printf("Enter a character: ");
            reg[R_R0] = getc(stdin) & 0xFF;
            update_flags(R_R0);
            putc((char)reg[R_R0], stdout);
            fflush(stdout);
            break;
        case TRAP_PUTSP:
            {
                // two characters per memory location 
                uint16_t *c = memory + reg[R_R0];
                while (*c)
                {
                    putc(*c & 0xFF, stdout);
                    if (*c >> 8)
                        putc(*c >> 8, stdout);
                    ++c;
                }
                fflush(stdout);
                break;
            }
        case TRAP_HALT:
            puts("HALT");
            fflush(stdout);
            running = 0;
            break;
        default:
            abort();
            break; 
    }

}

void read_image_file(FILE* file)
{
    // origin dictates where to place the image in memory
    uint16_t origin;
    fread(&origin, sizeof(origin), 1, file);
    origin = swap16(origin);

    uint16_t max_read = MEMORY_MAX - origin;
    uint16_t *p = memory + origin;
    size_t read = fread(p, sizeof(uint16_t), max_read, file);

    // swap LC-3 big-endian to more common little-endian
    while (read-- > 0)
    {
        *p = swap16(*p);
        ++p;
    }
}

int read_image(const char* image_path)
{
    FILE* fp = fopen(image_path, "rb");
    if (!fp)
        return 0;
    read_image_file(fp);
    fclose(fp);
    return 1;
}

uint16_t swap16(uint16_t x)
{
    return (x << 8) | (x >> 8); 
}

void mem_write(uint16_t address, uint16_t val)
{
    memory[address] = val;
}

uint16_t mem_read(uint16_t address)
{
    if (address == MR_KBSR)
    {
        if (check_key())
        {
            memory[MR_KBSR] = (1 << 15);
            memory[MR_KBDR] = getchar();
        }
        else
        {
            memory[MR_KBSR] = 0;
        }
    }

    return memory[address];
}

void disable_input_buffering()
{
    tcgetattr(STDIN_FILENO, &original_tio);
    struct termios new_tio = original_tio;
    new_tio.c_lflag &= ~ICANON & ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

void restore_input_buffering()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
}

uint16_t check_key()
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    return select(1, &readfds, NULL, NULL, &timeout) != 0;
}

void handle_interrupt(int signal)
{
    restore_input_buffering();
    printf("\n");
    exit(-2);
}