#include "vmfunc.h"

#include <stdio.h>
#include <stdlib.h>


int main (int argc, char **argv) 
{
    int i;

    // Load programs
    if (argc < 2)
    {
        puts("lc3 [image-file1] ...");
        exit(2);
    } 
   
    for (i = 1; i < argc; i++)
    {
        if (!read_image(argv[i]))
        {
            printf("Failed to load image: %s\n", argv[i]);
            exit(1);
        }
    }

    // TODO: Setup

    // Initialize condition flag with Z
    reg[R_COND] = FL_ZRO;

    // Initialize PC with default value
    reg[R_PC] = PC_START;

    int running = 1;

    while (running)
    {
        // Fetch instruction
        uint16_t instr = mem_read(reg[R_PC]++);
        uint16_t op = instr >> 12; // Last 4 bits

        switch(op)
        {
            case OP_ADD:
                break;
            case OP_AND:
                break;
            case OP_NOT:
                break;
            case OP_BR:
                break;
            case OP_JMP:
                break;
            case OP_JSR:
                break;
            case OP_LD:
                break;
            case OP_LDI:
                break;
            case OP_LDR:
                break;
            case OP_LEA:
                break;
            case OP_ST:     
                break;
            case OP_STI:
                break;
            case OP_STR:
                break;
            case OP_TRAP:
                break;
            // unused opcodes
            case OP_RTI:
            case OP_RES:
            default:
                break;
        }
    }


    // TODO: shutdown

}

