#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// WorksVKM Emulator (ported from Python)

#define malloc_max 1024

#define stack_max 1000

#define program_size 512


int main(int argc, char **argv) {
    printf("Works Virtual Machine\n");
    printf("(C) 2024-2025 DonnyWorks Software\n");
    uint16_t memory[malloc_max];
    uint16_t b0 = 0;
    uint16_t b1 = 0;
    uint16_t b2 = 0;
    uint16_t b3 = 0;
    uint16_t t = 0;
    uint16_t program_alloc = malloc_max - program_size;
    uint16_t pc = program_alloc;
    uint16_t stack[stack_max] = {program_alloc};
    int count = 0;
    int stackIndex = 0;
    unsigned char *program;
    if (argc < 2) {
        printf("Usage: emulator [program bin]");
        return -1;
    }
    long filelen;
    //printf(argv[1]);
    FILE *fptr = fopen(argv[1], "rb");
    if (fptr == NULL) {
        perror("Error opening file");
        return 1;
    }

    fseek(fptr, 0, SEEK_END);
    filelen = ftell(fptr);
    rewind(fptr);

    // Allocate memory for the program
    program = (char *)malloc(filelen * sizeof(char));
    if (program == NULL) {
        perror("Error allocating memory");
        fclose(fptr);
        return 1;
    }

    // Read the file into the program
    fread(program, filelen, 1, fptr);

    // Close the file
    fclose(fptr);
    //for (int i = 0; i < (filelen < 10 ? filelen : 10); i++) {
    //    printf("%d ", program[i]);
    //}
    for (int n = 0; n < filelen; n++) {
        //printf("hi");
        memory[program_alloc + n] = program[n];
    }
    while (pc < malloc_max) {
        int op = memory[pc];
        int param;
        //printf("PC: %i\n", program_alloc - pc);
        //printf("OP: %i\n", op);
        //todo: emulator bullshit
        //printf("todo: emulator\n");
        if (op == 0) {
        } else if (op == 0x01) {
            param = (memory[pc + 1]*16) + memory[pc + 2];
            pc += 2;
            t += param;
        } else if (op == 0xF1) {
            param = (memory[pc + 1]*16) + memory[pc + 2];
            pc += 2;
            t += memory[param];
        } else if (op == 0x02) {
            param = (memory[pc + 1]*16) - memory[pc + 2];
            pc += 2;
            t += param;
        } else if (op == 0xF2) {
            param = (memory[pc + 1]*16) - memory[pc + 2];
            pc += 2;
            t += memory[param];
        } else if (op == 0x03) {
            param = (memory[pc + 1]*16) * memory[pc + 2];
            pc += 2;
            t += param;
        } else if (op == 0xF3) {
            param = (memory[pc + 1]*16) * memory[pc + 2];
            pc += 2;
            t += memory[param];
        } else if (op == 0x04) {
            param = (memory[pc + 1]*16) / memory[pc + 2];
            pc += 2;
            t += param;
        } else if (op == 0xF4) {
            param = (memory[pc + 1]*16) / memory[pc + 2];
            pc += 2;
            t += memory[param];
        } else if (op == 0x05) {
            param = (memory[pc + 1]*16) + memory[pc + 2];
            //printf("Lightspeed to %i!\n",param);
            //printf("Localspace jump\n");
            stackIndex++;
            stack[stackIndex] = pc + 2;
            pc = program_alloc + param - 1;
            //printf("%i\n", pc - program_alloc);
        } else if (op == 0xF5) {
            param = (memory[pc + 1]*16) + memory[pc + 2];
            //printf("Lightspeed to %i!\n",param);
            stackIndex++;
            stack[stackIndex] = pc + 2;
            pc = param - 1;
        } else if (op == 0x06) {
            param = (memory[pc + 1]*16) + memory[pc + 2];
            int param2 = (memory[pc + 3]*16) + memory[pc + 4];
            if (param == t) {
                stackIndex++;
                stack[stackIndex] = pc + 2;
                pc = program_alloc - 1 + param2;
            }
            pc += 2;
        } else if (op == 0xE6) {
            param = memory[(memory[pc + 1]*16) + memory[pc + 2]];
            int param2 = (memory[pc + 3]*16) + memory[pc + 4];
            if (param == t) {
                stackIndex++;
                stack[stackIndex] = pc + 2;
                pc = param2 - 1;
            }
            pc += 2;
        } else if (op == 0x07) {
            param = (memory[pc + 1]*16) + memory[pc + 2];
            b0 = param;
            pc += 2;
        } else if (op == 0xF7) {
            param = memory[(memory[pc + 1]*16) + memory[pc + 2]];
            b0 = param;
            pc += 2;
        } else if (op == 0x08) {
            param = (memory[pc + 1]*16) + memory[pc + 2];
            b1 = param;
            pc += 2;
        } else if (op == 0xF8) {
            param = memory[(memory[pc + 1]*16) + memory[pc + 2]];
            b1 = param;
            pc += 2;
        } else if (op == 0x09) {
            param = (memory[pc + 1]*16) + memory[pc + 2];
            b2 = param;
            pc += 2;
        } else if (op == 0xF9) {
            param = memory[(memory[pc + 1]*16) + memory[pc + 2]];
            b2 = param;
            pc += 2;
        } else if (op == 0x0A) {
            param = (memory[pc + 1]*16) + memory[pc + 2];
            b3 = param;
            pc += 2;
        } else if (op == 0xFA) {
            param = memory[(memory[pc + 1]*16) + memory[pc + 2]];
            b3 = param;
            pc += 2;
        } else if (op == 0x0B) {
            memory[(memory[pc + 1]*16) + memory[pc + 2]] = b0;
            //b0 = param;
            pc += 2;
        } else if (op == 0xFB) {
            memory[memory[(memory[pc + 1]*16) + memory[pc + 2]]] = b0;
            //b0 = param;
            pc += 2;
        } else if (op == 0x0C) {
            memory[(memory[pc + 1]*16) + memory[pc + 2]] = b1;
            //b0 = param;
            pc += 2;
        } else if (op == 0xFC) {
            memory[memory[(memory[pc + 1]*16) + memory[pc + 2]]] = b1;
            //b0 = param;
            pc += 2;
        } else if (op == 0x0D) {
            memory[(memory[pc + 1]*16) + memory[pc + 2]] = b2;
            //b0 = param;
            pc += 2;
        } else if (op == 0xFD) {
            memory[memory[(memory[pc + 1]*16) + memory[pc + 2]]] = b2;
            //b0 = param;
            pc += 2;
        } else if (op == 0x0E) {
            memory[(memory[pc + 1]*16) + memory[pc + 2]] = b3;
            //b0 = param;
            pc += 2;
        } else if (op == 0xFE) {
            memory[memory[(memory[pc + 1]*16) + memory[pc + 2]]] = b3;
            //b0 = param;
            pc += 2;
        } else if (op == 0x0F) {
            pc = malloc_max;
        } else if (op == 0x10 || op == 0xFF) {
            if (op == 0x10) {
                param = (memory[pc + 1]*16) + memory[pc + 2];
                pc += 2;
            }
            if (op == 0xFF) {
                param = memory[(memory[pc + 1]*16) + memory[pc + 2]];
                pc += 2;
            }
            //printf("Stupidity detected.");
            if (param == 0x01) {
                //printf("%i",b0);
                putchar((char)b0);
            }
        } else if (op == 0x11) {
            pc = stack[stackIndex];
            stack[stackIndex] = 0;
            stackIndex--;
        } else if (op == 0x12 || op == 0xE1) {
            if (op == 0x12) {
                param = (memory[pc + 1]*16) + memory[pc + 2];
                pc += 2;
            }
            if (op == 0xE1) {
                param = memory[(memory[pc + 1]*16) + memory[pc + 2]];
                pc += 2;
            }
            memory[param] = (t - round(t/16)*16);
            memory[param + 1] = round(t/16);
            pc += 2;
        }
        pc++;
    }
    //fclose(fptr);
    return 0;
}
